#define _XOPEN_SOURCE 600

#include <ftw.h>
#include "../inc/word_counter.h"

#define DEFAULT_PATH "/usr/share/man/man1"

t_tree words;

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
  (void)type;
  switch(sbuf->st_mode & S_IFMT) {
  case S_IFREG: open_file(&pathname[ftwb->base], &words);break;
  case S_IFDIR:  break;
  default: printf("?");break;
  }
  return 0;
}

int main(int argc, char **argv)
{
  if (argc > 2)
    usage_error(argv[0], NULL);
  tr_initialize(&words);
  if (nftw(argc == 1 ? DEFAULT_PATH : argv[1], dir_tree, 10, FTW_DEPTH) == -1) {
    perror("nftw");
    usage_error(argv[0], NULL);
  }
  show_words(&words);
  printf("\n");
  return 0;
}
