#include "kernel/types.h"
#include "kernel/pinfo.h"
#include "user/user.h"

int
main(void)
{
  int pid = getpid();
  struct pinfo info[64];

  for(volatile unsigned long i = 0; i < 500000000UL; i++);

  int n = getpinfo(info, 64);
  for(int i = 0; i < n; i++){
    if(info[i].pid == pid)
      printf("Level=%d Runtime=%d Schedules=%d\n",
             info[i].level, info[i].runticks, info[i].nsched);
  }

  exit(0);
}
