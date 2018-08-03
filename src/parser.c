#include "../inc/word_counter.h"

static FILE *fp;

static void print_item(t_item item)
{
    fprintf(fp, " %s %d\n", item.word, item.cnt_word);
}

void save_to_the_file(const t_tree *ptr)
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
        word_tmp++;

    return word_tmp;
}

void parse_words(char *word_tmp, t_tree *ptr)
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