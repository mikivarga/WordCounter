#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

typedef enum { FALSE, TRUE } Boolean;

#define BUF 25

typedef struct		s_word
{
  char			word[BUF];
  unsigned int		cnt_word;	
}			t_word;

typedef struct		s_tree_node
{
  t_word		word;
  struct s_tree_node	*left;
  struct s_tree_node	*right;
}			t_tree_node;

typedef struct		s_pair
{
  t_tree_node		*parent;
  t_tree_node		*child;
}			t_pair;

typedef struct		s_tree
{
  t_tree_node		*root;
  unsigned int		size;
}			t_tree;

void		tw_initialize(t_tree *p_tr_wrd);
Boolean		tw_is_empty(const t_tree *p_tr_wrd);
Boolean		tw_is_fool(const t_tree *p_tr_wrd);
unsigned int	tw_item_count(const t_tree *p_tr_wrd);
Boolean		tw_to_right(const t_word *p_wrd1, const t_word *p_wrd2);
Boolean		tw_to_left(const t_word *p_wrd1, const t_word *p_wrd2);
t_pair		tw_seek_item(const t_word *p_wrd, const t_tree *p_tr_wrd);
Boolean		tw_add_item(const t_word  *p_wrd, t_tree *p_tr_wrd);
Boolean		tw_delete_data(t_word *p_wrd, t_tree *p_tr_wrd);
void		tw_delete_all(t_tree *p_tr_wrd);
void		ft_traverse\
(int order, t_tree *p_tr_wrd, void (*pfun)(t_word word, t_tree *));

#endif /*WORD_COUNTER_H*/
