#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

#include <pthread.h>
#include <sys/types.h>
#include <fnmatch.h>
#include <fts.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define LEN 100
#define BUF_SIZE 1024
#define MAXITEMS 200000
#define NUM_THREADS 10
#define DEFAULT_PATH "/usr/share/man/"

typedef enum { FALSE, TRUE } Boolean;

#define HANDLE_ERROR(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
#define HANDLE_ERROR_EN(en, msg) do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

enum tstate {
    TS_ALIVE,
    TS_TERMINATED,
    TS_JOINED
};

struct thread_info {
    pthread_t thread_id;
    enum tstate state;
    char argv_string[BUF_SIZE];
    FILE *fp;
    int num;
};

typedef struct		s_item {
  char word[LEN];
  unsigned int cnt_word;
}			t_item;

typedef struct		s_trnode {
  t_item item;
  struct s_trnode	*left;
  struct s_trnode	*right;
}			t_trnode;

typedef struct		s_tree {
  t_trnode *root;
  unsigned int size;
}			t_tree;

void tr_initialize(t_tree *ptr);
Boolean	tr_is_empty(const t_tree *ptr);
Boolean	tr_is_full(const t_tree *ptr);
unsigned int tr_item_count(const t_tree *ptr);
Boolean	tr_add_item(const t_item  *pi, t_tree *ptr);
void tr_traverse(const t_tree *ptr, void (*pfun)(t_item item));
void tr_delete_all(t_tree *ptree);
void parse_words(char *word_tmp, t_tree *ptr);
void save_to_the_file(const t_tree *ptr);


#endif /*WORD_COUNTER_H*/