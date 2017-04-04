/* 
 * alsrtools.c 
 * (c) 2015 Aris Adamantiadis
 * Measures the effectiveness of ASLR on compiler settings/OS
 * Usage: gcc -o aslrtools aslrtools.c -ldl [-fPIC -fpie -pie]
 * License: you must send Aris a 0day before being allowed to use it/
 * change it.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <stdint.h>
#include <dlfcn.h>

struct measure_struct {
    void *text;
    void *stack;
    void *libc;
    void *smallheap;
    void *bigheap;
};

void measure(int fd){
    struct measure_struct m;
    m.text = (void *)measure;
    m.stack = (void *)&m;
    m.libc = dlsym(RTLD_DEFAULT, "free");
    m.smallheap = malloc(100);
    m.bigheap = malloc(2*1024*1024);
    write(fd, &m, sizeof(m));
}

const char *itoa(int v){
    static char buffer[64];
    snprintf(buffer, sizeof(buffer),"%d",v);
    return buffer;
}

int get_measure(const char *progname, struct measure_struct *m){
    int fds[2];
    int rc;
    
    rc = pipe(fds);
    if (rc < 0){
        perror("pipe");
        return -1;
    }
    rc = fork();
    if (rc < 0){
        perror("fork");
	return -1;
    }
    if (rc == 0){
        /* child */
	close(fds[0]);
	rc = execl(progname, progname, "-pipe", itoa(fds[1]), NULL);
	perror("execl");
	exit(1);
    } else {
        /* parent */
	close(fds[1]);
	rc = read(fds[0], m, sizeof(*m));
	if (rc != sizeof(*m)){
	    perror("read");
	    return -1;
	}
	close(fds[0]);
	wait(NULL);
    }
    return 0;
}

struct measure_struct aslr = {
    NULL, NULL, NULL, NULL, NULL
};

void print_measure(const char *name, struct measure_struct *m){
    printf("%s: txt %p stack %p libc %p smallheap %p bigheap %p\n",name, 
        m->text, m->stack, m->libc, m->smallheap, m->bigheap);
}

int getbits(void *p){
    int bits=0;
    intptr_t v = (intptr_t)p;
    intptr_t i = 1;
    while(i != 0){
        bits += (v & i)?1:0;
        i <<= 1;
    }
    return bits;
}

void print_bits(struct measure_struct *m){
    printf("ASLR depth: txt %d, stack %d, libc %d, smallheap %d, bigheap %d\n",
        getbits(m->text), getbits(m->stack), getbits(m->libc),
        getbits(m->smallheap), getbits(m->bigheap));
}

void analyze(struct measure_struct *ref, struct measure_struct *m){
    intptr_t *ref_i = (intptr_t *)ref;
    intptr_t *m_i = (intptr_t *)m;
    intptr_t *aslr_i = (intptr_t *)&aslr;
    int i;
    for (i=0;i<5;++i){
        aslr_i[i] |= m_i[i] ^ ref_i[i];
    }
//    print_measure("ref ",ref);
//    print_measure("mes ", m);
//    print_measure("aslr", &aslr);
}

int main(int argc, char **argv){
    struct measure_struct m;
    struct measure_struct ref;
    int rc;
    int i;
    if(argc == 3 && strcmp(argv[1], "-pipe") == 0){
        measure(atoi(argv[2]));
	return 0;
    }
    rc = get_measure(argv[0], &ref);
    if(rc != 0){
        return 1;
    }
    print_measure("reference", &ref);
    for (i=0;i<100;++i){
        rc = get_measure(argv[0], &m);
        if (rc == 0){
            analyze(&ref, &m);
       }
    }
    print_measure("ASLR",&aslr);
    print_bits(&aslr);
    return 0;
}
