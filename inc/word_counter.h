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
#define MAXITEMS 200000
#define DEFAULT_PATH "/usr/share/man/man1"

typedef struct		s_item {
  char word[BUF];
  unsigned int cnt_word;
}			t_item;

typedef struct		s_trnode {
  t_item item;
  struct s_trnode	*left;
  struct s_trnode	*right;
  pthread_mutex_t mutex;
}			t_trnode;

typedef struct		s_tree {
  t_trnode *root;
  unsigned int size;
}			t_tree;

void		tr_initialize(t_tree *ptr);
Boolean		tr_is_empty(const t_tree *ptr);
Boolean		tr_is_full(const t_tree *ptr);
unsigned int	tr_item_count(const t_tree *ptr);
Boolean		tr_add_item(const t_item  *pi, t_tree *ptr);
void tr_traverse(const t_tree *ptr, void (*pfun)(t_item item));

Boolean open_file(char *patchname, t_tree * ptr);
void show_words(const t_tree *ptr);


#endif /*WORD_COUNTER_H*/
