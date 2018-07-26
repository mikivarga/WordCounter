#define _XOPEN_SOURCE 600
#include <ftw.h>
#include "../inc/word_counter.h"
/*static void usage_error(const char *prog_name, char *msg)
{
    exit(EXIT_FAILURE);
}*/

static int dir_tree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb)
{
    (void)type;
    switch(sbuf->st_mode & __S_IFMT) {
        case __S_IFREG: printf("-  "); break;
        case __S_IFDIR: printf("d  "); break;
        default: printf("?");break;
    }
    printf("%s\n", &pathname[ftwb->base]);
    return 0;
}

int main(int argc, char argv)
{
    //TODO: getopt
        (void)argc;
        (void)argv;
    /*if (argc != 2)
        usage_error(argv[0], NULL);
    */
   if (nftw("/usr/share/man/man1", dir_tree, 10, FTW_DEPTH) == -1) {
       perror("nftw");
       exit(EXIT_FAILURE);
   }
    

    return 0;
}