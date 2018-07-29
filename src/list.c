#include "../inc/word_counter"

void lst_initialize(t_list **plst)
{
    *plst = NULL;
}

Boolean lst_is_empty(const t_list **plst)
{
    if (*plst = NULL)
        return TRUE;
    return FALSE;
}

unsigned int lst_item_count(const t_list **plst)
{
    unsigned int count = 0;
    
    t_list * list_tmp = *plst;
    while (list_tmp != NULL) {
        ++count;
        list_tmp = list_tmp->next;
    }
    return count;
}

unsigned int lst_add_thread(t_thread thread, t_list **plist)
{
    t_list *pnew;
    t_list *scan = *plist;

    pnew = (t_list *)malloc(sizeof(t_list));
    if (pnew == NULL)
        return false;
    copy_to_node(item, pnew);
    pnew->next = NULL;
    if (scan == NULL) {
        *plist = pnew;
    } else {
        while(scan->next != NULL)
            scan = scan->next;
        scan->next = pnew;
    }
    return true;
}

void lst_traverse(const t_list, void (* pfun)(t_item item))
{

}

void lst_delete(t_list *plist)
{

}