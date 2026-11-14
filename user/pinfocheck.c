#include "kernel/types.h"
#include "kernel/pinfo.h"
#include "user/user.h"

int
main(void)
{
  struct pinfo info[64];
  int n = getpinfo(info, 64);

  printf("getpinfo returned: %d\n", n);

  for(int i = 0; i < n; i++){
    printf("PID=%d Level=%d Runtime=%d Schedules=%d\n",
           info[i].pid, info[i].level,
           info[i].runticks, info[i].nsched);
  }

  exit(0);
}
