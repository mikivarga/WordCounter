#include "../inc/word_counter.h"

extern t_tree words;
/*struct ft 
{
    char* file_name;
    int start_index;
    int end_index;
};

void *thread(void *arg) {
    int i;
    int c;
    struct ft* fi = (struct ft*)arg;

    FILE* file = fopen(fi->file_name);
    fseek (file , fi->start_index, SEEK_SET);
    for(i = 0; i < fi->end_index - fi->start_index; i++)
    {
        c = getc();
        //do something
    }
}*/

FILE *in;



static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_t t[200000];

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

void *
threadFunc(void *arg)
{
    char *word_tmp = ((char *) arg);
    int loc, j, s;
    t_item item;
    char *pword;

    //pthread_mutex_lock(&mtx);
    /*while(fscanf(y, "%s", word_tmp) == 1)*/{
        while (*word_tmp) {
            word_tmp = check_tokens(word_tmp);
            if (*word_tmp) {
                pword = item.word;
                while (*word_tmp && isalpha(*word_tmp)) {
                    *pword++ = *word_tmp++;
                }
               *pword = '\0';
                pthread_mutex_lock(&mtx);
                //printf("%s\n", item.word);
                tr_add_item(&item, &words);
                pthread_mutex_unlock(&mtx);
                // /pthread_cond_signal(&cond);
            }
    //word_tmp = check_tokens(word_tmp);

    /*s = pthread_mutex_lock(&mtx);
    
        add_words(word_tmp, &words);
    s = pthread_mutex_unlock(&mtx);
    s = pthread_cond_signal(&cond);*/
        }
    }
    //s = pthread_mutex_unlock(&mtx);
    
    // /pthread_cond_signal(&cond);
    return NULL;
}


void open_gz_file(char *patchname, t_tree *ptr)
{
    char cmd[4096] = "zcat ";
    char word_tmp[1024];

    memcpy(cmd + 5, patchname, strlen(patchname) + 1);
    in = popen(cmd, "r");
    if (!in) {
        fprintf(stderr, "%s: popen: %s\n", patchname, strerror(errno));
    }

    /*while(fscanf(in, "%s", word_tmp) == 1){
        add_words(word_tmp, ptr);
        
    }*/

//    pthread_t t1;
//    pthread_create(&t1, NULL, threadFunc, word_tmp);
    
    int i = 0;
    while(fscanf(in, "%s", word_tmp) == 1) {
        add_words(word_tmp, ptr);
        //printf("~1~");
        //pthread_create(&t[i++], NULL, threadFunc, word_tmp);
    }
    printf("end_gz_file ");
    //pthread_mutex_lock(&mtx);
    
    //pthread_create(&t2, NULL, threadFunc, word_tmp);
    //while (i > 0)
        //pthread_join(t[--i], NULL);
    //pthread_mutex_unlock(&mtx);
    //pthread_join(t1, NULL);
    pclose(in);
}

Boolean open_file(char *patchname, t_tree *ptr)
{
    if (strstr(patchname, ".gz")) {
        open_gz_file(patchname, ptr);//TRUE/FALSE
    }
    return TRUE;
}