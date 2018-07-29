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

#define BUF 1000
#define MAXITEMS 1000000

typedef struct		s_item
{
  char word[BUF];
  int cnt_word;
}			t_item;

typedef struct		s_trnode
{
  t_item item;
  struct s_trnode	*left;
  struct s_trnode	*right;
}			t_trnode;

typedef struct		s_tree
{
  t_trnode *root;
  int size;
}			t_tree;

void		tr_initialize(t_tree *ptr);
Boolean		tr_is_empty(const t_tree *ptr);
Boolean		tr_is_full(const t_tree *ptr);
unsigned int	tr_item_count(const t_tree *ptr);
Boolean		tr_add_item(const t_item  *pi, t_tree *ptr);


void tr_traverse(const t_tree *ptr, void (*pfun)(t_item item));

Boolean open_file(const char *patchname, t_tree * ptr);
void show_words(const t_tree *ptr);


#endif /*WORD_COUNTER_H*/
