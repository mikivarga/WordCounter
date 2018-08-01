#include "../inc/word_counter.h"

#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

static FILE *fp;

extern t_tree words;

#define DEBUG 1

static void print_item(t_item item)
{
    fprintf(fp, " %s %d\n", item.word, item.cnt_word);
}

void show_words(const t_tree *ptr)
{
    #if DEBUG
    if ((fp = fopen("2.txt", "w")) == NULL){
        printf("Cannot open %s\n", "res.txt");
        exit(EXIT_FAILURE);
    }
    #else
if ((fp = fopen("1.txt", "w")) == NULL){
        printf("Cannot open %s\n", "res.txt");
        exit(EXIT_FAILURE);
    }
    #endif
    if (tr_is_empty(ptr)) {
        puts("there is no words in tree!");
    } else {
        tr_traverse(ptr, print_item);
    }
    fclose(fp);
}



#define NUM_THREADS 10
static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;
/* Protects all of the following global variables */

static int totThreads = 0;      /* Total number of threads created */
static int numLive = 0;         /* Total number of threads still alive or
                                   terminated but not yet joined */
static int numUnjoined = 0;     /* Number of terminated threads that*/

enum tstate {                   /* Thread states */
    TS_ALIVE,                   /* Thread is alive */
    TS_TERMINATED,              /* Thread terminated, not yet joined */
    TS_JOINED                   /* Thread terminated, and joined */
};

struct thread_info {
    pthread_t thread_id;
    enum tstate state;
    char *argv_string;
} ;

static char *check_tokens(char *word_tmp)
{
    while (*word_tmp && !isalpha(*word_tmp))
        *word_tmp++;

    return word_tmp;
}

static void add_words(char *word_tmp, t_tree *ptr)
{
    char *pword;
;

        t_item item;

        while (*word_tmp) {
            word_tmp = check_tokens(word_tmp);
            if (*word_tmp) {
                pword = item.word;
                while (*word_tmp && isalpha(*word_tmp)) {
                    *pword++ = *word_tmp++;
                }
                *pword = '\0';
                
                // /write(1, item.word, strlen(item.word));
                tr_add_item(&item, ptr);
            }
            word_tmp = check_tokens(word_tmp);
        }
}



static void *thread_start(void *arg)
{
    struct thread_info *t_info = arg;
    char *uargv;
        int s;
        char *p;

    if ((uargv = strdup(t_info->argv_string)) == NULL) {
        handle_error("strdup");
    }
    s = pthread_mutex_lock(&threadMutex);
    if (s != 0)
        handle_error_en(s, "mutex_lock");
        numUnjoined++;
        t_info->state = TS_TERMINATED;
    add_words(uargv, &words);
    s = pthread_mutex_unlock(&threadMutex);
    if (s != 0)
        handle_error_en(s, "mutex_unlock");
    s = pthread_cond_signal(&threadDied);
    if (s != 0)
        handle_error_en(s, "cond_sign");

    return uargv;
}
int tnum;
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

#if DEBUG
static void open_gz_file(char *patchname, t_tree *ptr)
{
    char cmd[BUF] = "zcat ";
    char tmp[BUF];   
    FILE *in;
    struct thread_info tinfo[NUM_THREADS];
    int tnum, s, fl;
    void *res;

    fl = 1;
    memcpy(cmd + 5, patchname, strlen(patchname) + 1);
    if ((in = popen(cmd, "r")) == NULL) {
        printf("Cannot open %s\n", patchname);
        exit(EXIT_FAILURE);
    }
    //tinfo = calloc(NUM_THREADS, sizeof(struct thread_info));
    while(1)
    {
      
        tnum = 0;
        totThreads =0;
        for(tnum = 0; tnum < NUM_THREADS; tnum++, totThreads++) {
            if (fgets(tmp, BUF, in) == NULL) {
                fl = 0;
                break ;
        }
        tinfo[tnum].argv_string = tmp;
        tinfo[tnum].state = TS_ALIVE;
           
        s = pthread_create(&tinfo[tnum].thread_id, NULL, &thread_start, &tinfo[tnum]);
        if (s != 0)
            handle_error_en(s, "pthread_create");
        }
        numLive = --totThreads;
            while (numLive > 0) {
                s = pthread_mutex_lock(&threadMutex);
                if (s != 0)
                handle_error_en(s, "mutex_lock");
            
                while (numUnjoined == 0) {
                    s = pthread_cond_wait(&threadDied, &threadMutex);
                    if (s != 0)
                        handle_error_en(s, "pthread_cond_wait");
                }

            for (tnum = 0; tnum < totThreads; tnum++) {
                if (tinfo[tnum].state == TS_TERMINATED) {
                    s = pthread_join(tinfo[tnum].thread_id, &res);
                    if (s!= 0)
                        handle_error_en(s, "pthread_join");
                    tinfo[tnum].state = TS_JOINED;
                    numLive--;
                    numUnjoined;
                    printf("%s\n", (char *) res);
                    free(res);
                    }
                }
            s = pthread_mutex_unlock(&threadMutex);
             if (s != 0)
                 handle_error_en(s, "mutex_unlock");

            }
        //}
        if (!fl) { 
            break ;
        } 
    }
    //free(tinfo);
    pclose(in);
}
#else
static void open_gz_file(char *patchname, t_tree *ptr)
{
    FILE *in;
     char cmd[BUF] = "zcat ";
    char tmp[BUF];

    memcpy(cmd + 5, patchname, strlen(patchname) + 1);
    if ((in = popen(cmd, "r")) == NULL) {
        printf("Cannot open %s\n", patchname);
        //exit(EXIT_FAILURE);
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