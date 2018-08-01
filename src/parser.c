#include "../inc/word_counter.h"

static void print_item(t_item item)
{
  printf(" %s %d\n", item.word, item.cnt_word);
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
    } else {
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

static void open_gz_file(char *words_tmp, char *patchname, t_tree *ptr)
{
    char cmd[BUF] = "zcat ";   
    FILE *in;

    memcpy(cmd + 5, patchname, strlen(patchname) + 1);
    in = popen(cmd, "r");
    if (!in)
        fprintf(stderr, "%s: popen: %s\n", patchname, strerror(errno));
    while (fgets(words_tmp, BUF, in) != NULL) {
        if (!tr_is_full(ptr))
            add_words(words_tmp, ptr);
        else
            break ;
    }
    pclose(in);
}

static void open_reg_file(char *words_tmp, char *patchname, t_tree *ptr)
{
    FILE *in;

    in = fopen(patchname, "r");
    if (!in)
        fprintf(stderr, "%s: popen: %s\n", patchname, strerror(errno));
    while (fgets(words_tmp, BUF, in) != NULL) {
        if (!tr_is_full(ptr))
            add_words(words_tmp, ptr);
        else
            break;
    }
    pclose(in);
}

Boolean open_file(char *patchname, t_tree *ptr)
{
    char tmp[BUF];

    if (strstr(patchname, ".gz"))
        open_gz_file(tmp, patchname, ptr);
    else
        open_reg_file(tmp, patchname, ptr);
}