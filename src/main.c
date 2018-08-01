#define _XOPEN_SOURCE 600

#include <ftw.h>
#include "../inc/word_counter.h"

t_tree words;

static int dir_tree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    char *patch;

    patch = (char *)pathname;
    if (((sbuf->st_mode & __S_IFMT) == S_IFREG)) {
        open_file(patch, &words);
    }
  return 0;
}

int main(int argc, char **argv)
{
    if (argc > 2)
        fprintf(stderr, "Usage: %s [directory-path]\n", argv[0]);
    tr_initialize(&words);
    if (nftw(argc == 1 ? DEFAULT_PATH : argv[1], dir_tree, 20, FTW_CHDIR) == -1) {
        perror("nftw");
        fprintf(stderr, "Usage: %s [directory-path]\n", argv[0]);
    }
    show_words(&words);
    tr_delete_all(&words);
    return 0;
}