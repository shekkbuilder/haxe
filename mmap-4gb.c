#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>

int main(void)
{
    uint64_t* rr_addr = 0;
    uint64_t i = 17179869184;

    printf("\nsizeof(size_t): %lu\n", sizeof(size_t));

    printf("(uint64_t)0x100000000: %lx\n", (uint64_t)0x100000000);
    printf("1L << 33: %lx\n", 1L << 33);

    char cmd[20];

    sprintf(cmd, "pmap -x %i", getpid());
    printf("%s\n", cmd);
    system(cmd);

    rr_addr = mmap((void*)i, (1UL << 33), PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);

    printf("%s\n", cmd);
    system(cmd);


    printf("rr_addr: %p, %lu \n", rr_addr, rr_addr);
    if (rr_addr == MAP_FAILED) {
        perror("mmap error");
    }

    return 0;
}
