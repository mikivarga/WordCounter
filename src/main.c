#define _XOPEN_SOURCE 600

#include <ftw.h>
#include <pthread.h>
#include "../inc/word_counter.h"

#define DEFAULT_PATH "/usr/share/man/man1"

static void usage_error(const char *prog_name, char *msg)
{
  if (msg != NULL)
    fprintf(stderr, "%s\n", msg);
  fprintf(stderr, "Usage: %s [directory-path]\n", prog_name);
  exit(EXIT_FAILURE);
}





static int dir_tree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
  int static i = 1;
  //pthread create
  //pthread_mutecs_lock(popen)
  //pthread_mutecs_unloc
  (void)type;
  switch(sbuf->st_mode & S_IFMT) {
  case S_IFREG: /*printf("-  ")*/; break;
  case S_IFDIR: printf("-d "); break;
  default: printf("?");break;
    }
  if (sbuf->st_mode & S_IFDIR)
    printf("%d ", i++);
  //printf("%s\n", &pathname[ftwb->base]);
  //printf("%d ", i++);
  

    return 0;
}

int main(int argc, char **argv)
{
  if (argc > 2)
    usage_error(argv[0], NULL);
  if (nftw(argc == 1 ? DEFAULT_PATH : argv[1], dir_tree, 10, FTW_DEPTH) == -1) {
    perror("nftw");
    usage_error(argv[0], NULL);
  }
  printf("\n");
  return 0;
}
