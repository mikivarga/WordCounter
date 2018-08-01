#include "../inc/word_counter.h"

typedef struct pair {
	t_trnode *parent;
	t_trnode *child;
} t_pair;

static Boolean tr_to_right(const t_item *pi1, const t_item *pi2);
static Boolean tr_to_left(const t_item *pi1, const t_item *pi2);
static t_pair tr_seek_item(const t_item *pi, const t_tree *ptr);
static t_trnode *tr_make_node(const t_item *pi);
static void tr_add_node(t_trnode *new_node, t_trnode *root);
static void tr_in_order(const t_trnode * root, void (* pfun)(t_item item));
static void	tr_delete_all_nodes(t_trnode *root);

void tr_initialize(t_tree *ptr)
{
	ptr->root = NULL;
	ptr->size = 0;
}

Boolean	tr_is_empty(const t_tree *ptr)
{
	if (ptr->root == NULL)
		return TRUE;
	return FALSE;
}

Boolean	tr_is_full(const t_tree *ptr)
{
	if (ptr->size == MAXITEMS)
		return TRUE;
	return FALSE;
}

unsigned int tr_item_count(const t_tree *ptr)
{
	return ptr->size;
}

Boolean	tr_add_item(const t_item  *pi, t_tree *ptr)
{
    t_trnode *new_node;
	t_pair seek;

	if (tr_is_full(ptr)) {
		fprintf(stderr, "tr_is_full");
		return FALSE;
	}
	if ((seek = tr_seek_item(pi, ptr)).child != NULL) {
		seek.child->item.cnt_word++;
		return TRUE;
	}
	if ((new_node = tr_make_node(pi)) == NULL) {
		fprintf(stderr, "tr_make_node");
		return FALSE;
	}
	ptr->size++;
	if (ptr->root == NULL)
		ptr->root = new_node;
	else
		tr_add_node(new_node, ptr->root);
	return TRUE;
}

static Boolean tr_to_right(const t_item *pi1, const t_item *pi2)
{
    if (strcmp(pi1->word, pi2->word) > 0)
		return TRUE;
    return FALSE;
}

static Boolean tr_to_left(const t_item *pi1, const t_item *pi2)
{
    if (strcmp(pi1->word, pi2->word) < 0)
		return TRUE;
    return FALSE;
}

static t_pair tr_seek_item(const t_item *pi, const t_tree *ptr)
{
	t_pair look;

	look.parent = NULL;
	look.child = ptr->root;
	if (look.child == NULL)
		return look;
	while (look.child != NULL) {
		if (tr_to_left(pi, &(look.child->item))) {
			look.parent = look.child;
			look.child = look.child->left;
		}
		else if (tr_to_right(pi, &(look.child->item))) {
			look.parent = look.child;
			look.child = look.child->right;
		}
		else {
			break ;
		} 
	}
	return look;
}

static t_trnode *tr_make_node(const t_item *pi)
{
	t_trnode *new_node;

	new_node = (t_trnode *)malloc(sizeof(t_trnode));
	if (new_node != NULL) {
		strcpy(new_node->item.word, pi->word);
		new_node->item.cnt_word = 1;
		new_node->left = NULL;
		new_node->right = NULL;
	}
	return new_node;
}

static void tr_add_node(t_trnode *new_node, t_trnode *root)
{
	if (tr_to_left(&new_node->item, &root->item))
	{
		if (root->left == NULL)
			root->left = new_node;
		else
			tr_add_node(new_node, root->left);
	}
	else if (tr_to_right(&new_node->item, &root->item))
	{
		if (root->right == NULL)
			root->right = new_node;
		else
			tr_add_node(new_node, root->right);
	}
}

void tr_traverse(const t_tree *ptr, void (*pfun)(t_item item)) {
	if (ptr != NULL)
		tr_in_order(ptr->root, pfun);
}

static void tr_in_order(const t_trnode * root, void (*pfun)(t_item item))
{
	if (root != NULL) {
		tr_in_order(root->left, pfun);
		(*pfun)(root->item);
		tr_in_order(root->right, pfun);
	}
}

void tr_delete_all(t_tree *ptr)
{
	if (ptr != NULL)
		tr_delete_all_nodes(ptr->root);
	ptr->root = NULL;
	ptr->size = 0;
}

static void	tr_delete_all_nodes(t_trnode *root)
{
	t_trnode *pright;

	if (root != NULL)
	{
		pright = root->right;
		tr_delete_all_nodes(root->left);
		free(root);
		tr_delete_all_nodes(pright);
	}
}