#include "../inc/word_counter.h"

static void print_item(t_item item)
{
  printf("%s\t %ld\n", item.word, item.cnt_word);
}

void show_words(const t_tree *ptr)
{
    if (tr_is_empty(ptr))
        puts("there is no words in tree!");
    else
        tr_traverse(ptr, print_item);
}

static char *check_tokens(char *word_tmp)
{
    while (*word_tmp && !isalpha(*word_tmp))
        *word_tmp++;

    return word_tmp;
}

static void add_words(char *word_tmp, t_tree *ptr)
{
    char *pword;

    if (tr_is_full(ptr)) {
        puts("there is no а free place in a tree!");
    }
    else {
        t_item item;

        while (*word_tmp) {
            word_tmp = check_tokens(word_tmp);
            if (*word_tmp) {
                pword = item.word;
                while (*word_tmp && isalpha(*word_tmp)) {
                    *pword++ = *word_tmp++;
                }
                *pword = '\0';
                tr_add_item(&item, ptr);
            }
            word_tmp = check_tokens(word_tmp);
        }
    }
}

static Boolean open_gz_file(const char *patchname, t_tree *ptr)
{
    char cmd[4096] = "zcat /usr/share/man/man1/";//static?
    char word_tmp[1024];
    FILE *in;


    memcpy(cmd + 25, patchname, strlen(patchname) + 1);
    in = popen(cmd, "r");
    if (!in) {
        fprintf(stderr, "%s: popen: %s\n", patchname, strerror(errno));
        return FALSE;
    }
    while (fscanf(in, "%s", word_tmp) == 1)
        add_words(word_tmp, ptr);
    pclose(in);//errr
    return TRUE;
}

Boolean open_file(const char *patchname, t_tree *ptr)
{
    if (strstr(patchname, ".gz")) {
        open_gz_file(patchname, ptr);//TRUE/FALSE
    }
}