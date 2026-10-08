#include "kernel/types.h"
#include "user/user.h"

#define MAXARG 32
#define BUFSIZE 4096

int
run_command(char *argv[])
{
  int pid;
  int status;

  pid = fork();

  if(pid < 0){
    fprintf(2, "xargs: fork failed\n");
    return 1;
  }

  if(pid == 0){
    exec(argv[0], argv);
    fprintf(2, "xargs: exec %s failed\n", argv[0]);
    exit(1);
  }

  wait(&status);

  if(status != 0)
    return 1;

  return 0;
}

int
run_words(char *base[], int base_argc, char *words[], int word_count)
{
  char *run_argv[MAXARG + 1];

  if(base_argc + word_count > MAXARG){
    fprintf(2, "xargs: too many arguments\n");
    return 1;
  }

  for(int i = 0; i < base_argc; i++)
    run_argv[i] = base[i];

  for(int i = 0; i < word_count; i++)
    run_argv[base_argc + i] = words[i];

  run_argv[base_argc + word_count] = 0;

  return run_command(run_argv);
}

int
main(int argc, char *argv[])
{
  int n = 0;
  int cmd_start = 1;

  if(argc < 2){
    fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
    exit(1);
  }

  if(strcmp(argv[1], "-n") == 0){
    if(argc < 4){
      fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
      exit(1);
    }

    n = atoi(argv[2]);

    if(n < 1){
      fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
      exit(1);
    }

    cmd_start = 3;
  }

  int base_argc = argc - cmd_start;

  if(base_argc >= MAXARG){
    fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
    exit(1);
  }

  char *base[MAXARG];

  for(int i = 0; i < base_argc; i++)
    base[i] = argv[cmd_start + i];

  static char storage[BUFSIZE];
  char *words[MAXARG];
  int word_count = 0;
  int pos = 0;
  int in_word = 0;
  int failed = 0;
  char c;

  while(read(0, &c, 1) == 1){

    if(c == ' ' || c == '\t' || c == '\n'){

      if(in_word){
        storage[pos++] = '\0';
        in_word = 0;
      }

      /*
       * Without -n, a newline ends the current command.
       */
      if(n == 0 && c == '\n' && word_count > 0){
        if(run_words(base, base_argc, words, word_count))
          failed = 1;

        word_count = 0;
        pos = 0;
      }

      /*
       * With -n, whitespace does not end the command.
       * The command runs only after N words have been collected.
       */
      if(n > 0 && word_count == n){
        if(run_words(base, base_argc, words, word_count))
          failed = 1;

        word_count = 0;
        pos = 0;
      }

    } else {

      if(!in_word){
        if(word_count >= MAXARG - base_argc){
          fprintf(2, "xargs: too many arguments\n");
          exit(1);
        }

        words[word_count++] = &storage[pos];
        in_word = 1;
      }

      if(pos >= BUFSIZE - 1){
        fprintf(2, "xargs: input too long\n");
        exit(1);
      }

      storage[pos++] = c;
    }
  }

 if(in_word)
    storage[pos++] = '\0';

  if(word_count > 0){
    if(run_words(base, base_argc, words, word_count))
      failed = 1;
  }

  exit(failed ? 1 : 0);
}

