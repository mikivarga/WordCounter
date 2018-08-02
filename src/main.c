#include <fnmatch.h>
#include <fts.h>
#include <err.h>
#include "../inc/word_counter.h"

#define DEBUG 0

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;
static int tot_threads = 0;
static int num_live = 0;
static int num_unjoined = 0;
static int dir_is_empty = 0;
static t_tree words;

static void *thread_start(void *arg)
{
    struct thread_info *t_info = arg;;
    int s;

#if DEBUG
    printf("thread terminating %d\n", t_info->num);
#endif
    
    s = pthread_mutex_lock(&threadMutex);
    if (s != 0)
        HANDLE_ERROR_EN(s, "mutex_lock");
    num_unjoined++;

#if DEBUG
    printf("num unjoined threads %d\n", t_info->num);
#endif
    
    t_info->state = TS_TERMINATED;
    open_file(t_info->argv_string, &words);
    s = pthread_mutex_unlock(&threadMutex);
    if (s != 0)
        HANDLE_ERROR_EN(s, "mutex_unlock");
    s = pthread_cond_signal(&threadDied);
    if (s != 0)
        HANDLE_ERROR_EN(s, "cond_sign");

    return t_info->argv_string;
}

static int entcmp(const FTSENT **a, const FTSENT **b)
{
	return strcmp((*a)->fts_name, (*b)->fts_name);
}

static void
save_thread_info(int tnum, char *path, Boolean state, struct thread_info *tinfo)
{
    tinfo[tnum].num = tnum;
    tinfo[tnum].argv_string = strdup(path);
    tinfo[tnum].state = state;
}

static void
waiting_thtreads(int s, int tnum, void *res, struct thread_info *tinfo)
{
    while (num_live > 0) {
        s = pthread_mutex_lock(&threadMutex);
        if (s != 0)
            HANDLE_ERROR_EN(s, "mutex_lock");
        while (num_unjoined == 0) {
            s = pthread_cond_wait(&threadDied, &threadMutex);
            if (s != 0)
                HANDLE_ERROR_EN(s, "pthread_cond_wait");
        }
        for (tnum = 0; tnum < tot_threads; tnum++) {
            if (tinfo[tnum].state == TS_TERMINATED) {
                s = pthread_join(tinfo[tnum].thread_id, &res);
                if (s != 0)
                    HANDLE_ERROR_EN(s, "pthread_join");
                tinfo[tnum].state = TS_JOINED;
                num_live--;
                num_unjoined--;
                free(res);
#if DEBUG
    printf("Reaped thread %d  (num live=%d)\n", tnum, num_live);
#endif
            }
        }
        s = pthread_mutex_unlock(&threadMutex);
        if (s != 0)
            HANDLE_ERROR_EN(s, "mutex_unlock");
    }
}

static void dir_tree(char *dir, const char *pattern)
{
    FTS *tree;
	FTSENT *f;
	char *argv[] = { dir, NULL };
    struct thread_info *tinfo;
    int s, tnum;
    void *res;
    
	tree = fts_open(argv, FTS_LOGICAL | FTS_NOSTAT, entcmp);
	if (tree == NULL)
		HANDLE_ERROR("fts_open");
    while(1) {
        tot_threads = num_live = num_unjoined = 0;
        tinfo = calloc(NUM_THREADS, sizeof(struct thread_info));
        for(tnum = 0; tnum < NUM_THREADS; tnum++) {
	        if (!(f = fts_read(tree))) {
                dir_is_empty = 1;
                break;
            }
            if (fnmatch(pattern, f->fts_name, FNM_PERIOD) == 0) {
                tot_threads++;
                save_thread_info(tnum, f->fts_path, TS_ALIVE, tinfo);
                s = pthread_create(&tinfo[tnum].thread_id, NULL, &thread_start, &tinfo[tnum]);
                if (s != 0)
                    HANDLE_ERROR_EN(s, "pthread_create");
            } else {
                tnum--;
            }
        }
        num_live = tot_threads;
        waiting_thtreads(s, tnum, res, tinfo);
#if DEBUG
    printf("\n");
#endif
        if (dir_is_empty)
            break ;
    }
	fts_close(tree);
}

int main(int argc, char **argv)
{
    if (argc > 2)
        fprintf(stderr, "Usage: %s [directory-path]\n", argv[0]);
    tr_initialize(&words);
    dir_tree(argc == 1 ? DEFAULT_PATH : argv[1], "*.gz");
    show_words(&words);
    tr_delete_all(&words);
    return 0;
}