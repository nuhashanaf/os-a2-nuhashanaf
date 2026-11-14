#include "kernel/types.h"
#include "kernel/pinfo.h"
#include "user/user.h"

int
main(void)
{

  for(int i = 0; i < 3; i++){
    int pid = fork();
    if(pid < 0){
      printf("fork failed\n");
      exit(1);
    }
    if(pid == 0){
      volatile unsigned long x = 0;
      for(unsigned long j = 0; j < 500000000UL; j++)
        x++;
      printf("Child %d finished\n", getpid());
      exit(0);
    }
  }

  for(int i = 0; i < 3; i++)
    wait(0);

  printf("All children finished\n");
  exit(0);
}
