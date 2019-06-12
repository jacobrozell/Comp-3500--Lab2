#include <stdio.h>#include <stdlib.h>#include <sys/mman.h>#include <sys/types.h>#include <sys/stat.h>#include <fcntl.h>#include <unistd.h>// This struct is shared between child and class so both have accesstypedef struct Peterson {    int turn;    int flag[2];} peterson;int nloop = 50;/**********************************************************\ * Function: increment a counter by some amount one by one * * argument: ptr (address of the counter), increment       * * output  : nothing                                       * **********************************************************/void add_n(int *ptr, int increment) {  int i, j;  for (i=0; i < increment; i++){    *ptr = *ptr + 1;    for (j=0; j < 1000000; j++);  }}// This setups the peterson struct's variablesvoid setup(peterson *v) {    v->flag[0] = 0;    v->flag[1] = 0;}int main(){  int pid;  int *countptr;  int fd;  int zero = 0;  system("rm -f counter");  fd = open("counter", O_RDWR | O_CREAT);  write(fd, &zero, sizeof(int));  countptr = (int *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);  if (!countptr) {    printf("Mapping failed\n");    exit(1);  }      *countptr = 0;  close(fd);  system("rm -f memory");  int mem;  peterson *vars;  mem = open("memory", O_RDWR | O_CREAT);      write(mem, &zero, sizeof(peterson));  vars  = (peterson *) mmap(NULL, sizeof(peterson), PROT_READ | PROT_WRITE,                         MAP_SHARED, mem, 0);  if (!vars) {      printf("Unable to map variables");      exit(1);  }      setup(variables);  close(mem);  pid = fork();  if (pid < 0){    printf("Unable to fork a process\n");    exit(1);  }  if (pid == 0) {    while (*countptr < nloop){      add_n(countptr, 2);      printf("Child process -->> counter = %d\n", *countptr);    }    close(fd);  }  else {    while (*countptr < nloop){      add_n(countptr, 20);      printf("Parent process -->> counter = %d\n", *countptr);    }    close(fd);  }}