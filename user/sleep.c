#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc != 2){
    fprintf(2, "usage: sleep <ticks>\n");
    exit(1);
  }

  char *s = argv[1];

  if(*s == '\0'){
    fprintf(2, "usage: sleep <ticks>\n");
    exit(1);
  }

  for(char *p = s; *p; p++){
    if(*p < '0' || *p > '9'){
      fprintf(2, "usage: sleep <ticks>\n");
      exit(1);
    }
  }

  int ticks = atoi(s);
  pause(ticks);

  exit(0);
}
