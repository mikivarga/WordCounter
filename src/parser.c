#include "../inc/word_counter.h"

#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static FILE *fp;

extern t_tree words;

static void print_item(t_item item)
{
    fprintf(fp, " %s %d\n", item.word, item.cnt_word);
}

void show_words(const t_tree *ptr)
{
    if ((fp = fopen("res.txt", "w")) == NULL){
        printf("Cannot open %s\n", "res.txt");
        exit(EXIT_FAILURE);
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


#define NUM_THREADS 10

struct thread_info {
    pthread_t thread_id;
    char *argv_string;
};

static void *thread_start(void *arg)
{
    struct thread_info *t_info = arg;
    char *uargv;

    if ((uargv = strdup(t_info->argv_string)) == NULL) {
        handle_error("strdup");
    }
    add_words(uargv, &words);

    return uargv;
}

static void open_reg_file(char *patchname, t_tree *ptr)
{
    FILE *in;
    char tmp[BUF];

    if ((in = fopen(patchname, "r")) == NULL) {
        printf("Cannot open %s\n", patchname);
        exit(EXIT_FAILURE);
    }
    while (fgets(tmp, BUF, in) != NULL) {
        if (!tr_is_full(ptr))
            add_words(tmp, ptr);
        else
            break;
    }
    pclose(in);
}

#if 0
static void open_gz_file(char *patchname, t_tree *ptr)
{
    char cmd[BUF] = "zcat ";
    char tmp[BUF];   
    FILE *in;
    struct thread_info *tinfo;
    int tnum, s, fl;
    void *res;

    fl = 1;
    memcpy(cmd + 5, patchname, strlen(patchname) + 1);
    if ((in = popen(cmd, "r")) == NULL) {
        printf("Cannot open %s\n", patchname);
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        tinfo = calloc(NUM_THREADS, sizeof(struct thread_info));
        tnum = 0;
        for(tnum = 0; tnum < NUM_THREADS; tnum++) {
            if (fgets(tmp, BUF, in) == NULL) {
                fl = 0;
                break ;
        }
        tinfo[tnum].argv_string = tmp;
           
        s = pthread_create(&tinfo[tnum].thread_id, NULL, &thread_start, &tinfo[tnum]);
        if (s != 0)
            handle_error_en(s, "pthread_create");

        }
        if (fl)
        for (tnum = 0; tnum < NUM_THREADS; tnum++) {
            s = pthread_join(tinfo[tnum].thread_id, &res);
            if (s!= 0)
                handle_error_en(s, "pthread_join");
            //printf("%s\n", (char *) res);
            free(res);
        }
        free(tinfo);
        if (!fl)
            break ; 
    }
    pclose(in);
}
#else
static void open_gz_file(char *patchname, t_tree *ptr)
{
    FILE *in;
    char tmp[BUF];

    if ((in = fopen(patchname, "r")) == NULL) {
        printf("Cannot open %s\n", patchname);
        exit(EXIT_FAILURE);
    }
    while (fgets(tmp, BUF, in) != NULL) {
        if (!tr_is_full(ptr))
            add_words(tmp, ptr);
        else
            break;
    }
    pclose(in);
}
#endif

Boolean open_file(char *patchname, t_tree *ptr)
{   
    if (strstr(patchname, ".gz"))
        open_gz_file(patchname, ptr);
    else
        open_reg_file(patchname, ptr);
}