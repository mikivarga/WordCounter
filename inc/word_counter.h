#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

typedef enum { FALSE, TRUE } Boolean;

#define BUF 100
#define MAXITEMS 1000000


char *str111[256];
/*
enum tstate {
    TS_ALIVE,
    TS_TERMINATED,
    TS_JOINED
};

typedef struct s_thread {
    pthread_t tid;
    enum tstate state;
    char patchname[BUF];
} t_thread;

typedef struct s_list {
  t_thread thread;
  struct s_list *next;
} t_list;*/

typedef struct		s_item {
  char word[BUF];
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

typedef struct s_data {
  t_tree tree;
  /*t_list list;*/
} t_data;

/*void lst_initialize(t_list **plst);
Boolean lst_is_empty(const t_list **plst);
unsigned int lst_item_count(t_list **plst);
Boolean lst_add_thread(t_thread thread, t_list **plst);
void lst_traverse(t_list **plst, void ( pfun)(t_thread thread));
void lst_delete(t_list **plst);*/

void		tr_initialize(t_tree *ptr);
Boolean		tr_is_empty(const t_tree *ptr);
Boolean		tr_is_full(const t_tree *ptr);
unsigned int	tr_item_count(const t_tree *ptr);
Boolean		tr_add_item(const t_item  *pi, t_tree *ptr);
void tr_traverse(const t_tree *ptr, void (*pfun)(t_item item));

Boolean open_file(char *patchname, t_tree * ptr);
void show_words(const t_tree *ptr);


#endif /*WORD_COUNTER_H*/
