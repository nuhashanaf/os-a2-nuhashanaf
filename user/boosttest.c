#include "kernel/types.h"
#include "kernel/pinfo.h"
#include "user/user.h"

int
main(void)
{
  int pid = getpid();
  struct pinfo info[64];
  int last_level = -1;

  for(volatile unsigned long j = 0; j < 3000000000UL; j++){
    if(j % 10000000UL == 0){
      int n = getpinfo(info, 64);
      for(int i = 0; i < n; i++){
        if(info[i].pid == pid && info[i].level != last_level){
          printf("Level=%d Runtime=%d\n",
                 info[i].level, info[i].runticks);
          last_level = info[i].level;
        }
      }
    }
  }

  exit(0);
}
