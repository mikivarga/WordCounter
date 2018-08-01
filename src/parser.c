#include "../inc/word_counter.h"

static FILE *fp;

static void print_item(t_item item)
{
    fprintf(fp, " %s %d\n", item.word, item.cnt_word);
}

void show_words(const t_tree *ptr)
{
    if ((fp = fopen("resolt.res", "w")) == NULL) {
        HANDLE_ERROR("Cannot open resolt.res\n");
    }
    if (tr_is_empty(ptr)) {
        puts("there is no words in tree!");
    } else {
        tr_traverse(ptr, print_item);
    }
    fclose(fp);
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

static void open_reg_file(char *patchname, t_tree *ptr)
{
    FILE *in;
    char tmp[BUF];

    if ((in = fopen(patchname, "r")) == NULL) {
        HANDLE_ERROR("Cannot open resolt.res\n");
    }
    while (fgets(tmp, BUF, in) != NULL) {
        if (!tr_is_full(ptr)) {
            add_words(tmp, ptr);
        } else {
            break;
        }
    }
    pclose(in);
}

static void open_gz_file(char *patchname, t_tree *ptr)
{
    FILE *in;
    char cmd[BUF] = "zcat ";
    char tmp[BUF];

    memcpy(cmd + 5, patchname, strlen(patchname) + 1);
    if ((in = popen(cmd, "r")) == NULL) {
        HANDLE_ERROR("popen() failed\n");
    }
    while (fgets(tmp, BUF, in) != NULL) {
        if (!tr_is_full(ptr))
            add_words(tmp, ptr);
        else
            break;
    }
    pclose(in);
}

Boolean open_file(char *patchname, t_tree *ptr)
{   
    if (strstr(patchname, ".gz"))
        open_gz_file(patchname, ptr);
    else
        open_reg_file(patchname, ptr);
}