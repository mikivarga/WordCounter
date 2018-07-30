#include "../inc/word_counter.h"

static void copy_to_node(t_thread thread, t_list *lst);

void lst_initialize(t_list **plst)
{
    (*plst) = NULL;
}

Boolean lst_is_empty(const t_list **plst)
{
    if (*plst = NULL)
        return TRUE;
    return FALSE;
}

unsigned int lst_item_count(t_list **plst)
{
    unsigned int count = 0;
    t_list * list_tmp = *plst;

    while (list_tmp != NULL) {
        ++count;
        list_tmp = list_tmp->next;
    }
    return count;
}

Boolean lst_add_thread(t_thread thread, t_list **plst)
{
    t_list *pnew;
    t_list *scan = *plst;

    pnew = (t_list *)malloc(sizeof(t_list));
    if (pnew == NULL)
        return FALSE;
    copy_to_node(thread, pnew);
    pnew->next = NULL;
    if (scan == NULL) {
        *plst = pnew;
    } else {
        while(scan->next != NULL)
            scan = scan->next;
        scan->next = pnew;
    }
    return TRUE;
}

void lst_traverse(t_list **plst, void (*pfun)(t_thread thread))
{
    t_list *list = *plst;

    while (list != NULL) {
        (*pfun)(list->thread);
        list = list->next;
    }
}

void lst_delete(t_list **plst)
{
    t_list *psave;

    while ((*plst) != NULL) {
        psave = (*plst)->next;
        free(*plst);
        *plst = psave;
    }
}

static void copy_to_node(t_thread thread, t_list *lst)
{
    lst->thread.tid = thread.tid;
    lst->thread.state = thread.state;
    strcpy(lst->thread.patchname, thread.patchname); 
}