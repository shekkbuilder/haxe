#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); \
 } while (0)
 
int
main(int argc, char *argv[])
{
 struct rlimit old, new;
 struct rlimit *newp;
 pid_t pid;
 
 if (!(argc == 2 || argc == 4)) {
 fprintf(stderr, "Usage: %s <pid> [<new-soft-limit> "
 "<new-hard-limit>]\n", argv[0]);
 exit(EXIT_FAILURE);
 }
 
 pid = atoi(argv[1]); /* PID of target process */
 
 newp = NULL;
 if (argc == 4) {
 new.rlim_cur = atoi(argv[2]);
 new.rlim_max = atoi(argv[3]);
 newp = &new;
 }
 
 if (syscall(SYS_prlimit64,pid, RLIMIT_NOFILE, newp, &old) == -1)
 errExit("prlimit-1");
 printf("Previous limits: soft=%lld; hard=%lld\n",
 (long long) old.rlim_cur, (long long) old.rlim_max);
 
 if (syscall(SYS_prlimit64,pid, RLIMIT_NOFILE, NULL, &old) == -1)
 errExit("prlimit-2");
 printf("New limits: soft=%lld; hard=%lld\n",
 (long long) old.rlim_cur, (long long) old.rlim_max);
 
 exit(EXIT_FAILURE);
}

