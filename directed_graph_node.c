#include "directed_graph_node.h"
#include "unordered_set.h"
#include <stdbool.h>
#include <string.h>

typedef struct directed_graph_node_t {
    char* p_name;
    char* p_text;
    unordered_set_t* p_parent_node_set;
    unordered_set_t* p_child_node_set;
} directed_graph_node_t;

static const size_t INITIAL_CAPACITY = 16;
static const size_t MAXIMUM_NAME_STRING_LEN = 80;
static const float  LOAD_FACTOR = 1.0f;

bool equals_function(void* a, void* b)
{
    if (!a || !b) return false;

    return strcmp(((directed_graph_node_t*) a)->p_name,
                  ((directed_graph_node_t*) b)->p_name) == 0;
}

size_t hash_function(void* v) 
{
    size_t ret;
    size_t i;
    char* pc;

    if (!v) return 0;

    ret = 0;
    i = 1;
    pc = ((directed_graph_node_t*) v)->p_name;

    while (*pc) 
    {
        ret += *pc * i;
        ++i;
        ++pc;
    }

    return ret;
}
    
directed_graph_node_t* directed_graph_node_t_alloc(char* name)
{
    directed_graph_node_t* p_node = malloc(sizeof(*p_node));
    char* p_text;
    
    if (!p_node) return NULL;
    
    p_text = malloc(sizeof(char) * MAXIMUM_NAME_STRING_LEN); 
    
    if (!p_text)
    { 
        free(p_node);
        return NULL;
    }
    
    p_node->p_child_node_set = unordered_set_t_alloc(INITIAL_CAPACITY,
                                                     LOAD_FACTOR,
                                                     hash_function,
                                                     equals_function);
    if (!p_node->p_child_node_set)
    {
        free(p_text);
        free(p_node);
        return NULL;
    }
    
    p_node->p_parent_node_set = unordered_set_t_alloc(INITIAL_CAPACITY,
                                                      LOAD_FACTOR,
                                                      hash_function,
                                                      equals_function);
    if (!p_node->p_parent_node_set) 
    {
        unordered_set_t_free(p_node->p_child_node_set);
        free(p_text);
        free(p_node);
        return NULL;
    }
    
    snprintf(p_text, 
             MAXIMUM_NAME_STRING_LEN, 
             "[directed_graph_node_t: id = %s]",
             name);
    
    p_node->p_name = name;
    p_node->p_text = p_text;
    return p_node;
}

bool                   
directed_graph_node_t_add_arc(directed_graph_node_t* p_tail,
                              directed_graph_node_t* p_head)
{
    if (!p_tail || !p_head) return false;
    
    if (!unordered_set_t_add(p_tail->p_child_node_set, p_head)) 
    {
        return false;
    }
    
    if (!unordered_set_t_add(p_head->p_parent_node_set, p_tail))
    {
        unordered_set_t_remove(p_tail->p_child_node_set, p_head);
        return false;
    }
    
    return true;
}

bool directed_graph_node_t_has_child
(directed_graph_node_t* p_node, directed_graph_node_t* p_child_candidate)
{
    if (!p_node || !p_child_candidate) return false;
    
    return unordered_set_t_contains(p_node->p_child_node_set, 
                                    p_child_candidate);
}

bool directed_graph_node_t_remove_arc(directed_graph_node_t* p_tail,
                                      directed_graph_node_t* p_head)
{
    if (!p_tail || !p_head) return false;
    
    unordered_set_t_remove(p_tail->p_child_node_set, p_head);
    unordered_set_t_remove(p_head->p_parent_node_set, p_tail);
    return true;
}

char* directed_graph_node_t_to_string(directed_graph_node_t* p_node)
{
    if (!p_node) return "NULL node";
    
    return p_node->p_text;
}

unordered_set_t* 
directed_graph_node_t_children_set(directed_graph_node_t* p_node)
{
    return p_node ? p_node->p_child_node_set : NULL;
}

unordered_set_t* 
directed_graph_node_t_parent_set(directed_graph_node_t* p_node)
{
    return p_node ? p_node->p_parent_node_set : NULL;
}

void directed_graph_node_t_clear(directed_graph_node_t* p_node)
{
    unordered_set_iterator_t* p_iterator;
    directed_graph_node_t*    p_tmp_node;
    
    if (!p_node) return;
    
    p_iterator = unordered_set_iterator_t_alloc(p_node->p_child_node_set);
    
    while (unordered_set_iterator_t_has_next(p_iterator)) 
    {
        unordered_set_iterator_t_next(p_iterator, &p_tmp_node);
        
        if (strcmp(p_node->p_name, p_tmp_node->p_name) != 0) 
        {
            unordered_set_t_remove(p_tmp_node->p_parent_node_set, p_node);
        }
    }
    
    p_iterator = unordered_set_iterator_t_alloc(p_node->p_parent_node_set);
    
    while (unordered_set_iterator_t_has_next(p_iterator))
    {
        unordered_set_iterator_t_next(p_iterator, &p_tmp_node);
        
        if (strcmp(p_node->p_name, p_tmp_node->p_name) != 0) 
        {
            unordered_set_t_remove(p_tmp_node->p_child_node_set, p_node);
        }
    }
    
    unordered_set_t_clear(p_node->p_parent_node_set);
    unordered_set_t_clear(p_node->p_child_node_set);
}

void directed_graph_node_t_free(directed_graph_node_t* p_node) 
{
    unordered_set_iterator_t* p_iterator;
    directed_graph_node_t* p_tmp_node;
    
    if (!p_node) return;
    
    directed_graph_node_t_clear(p_node);
    unordered_set_t_free(p_node->p_child_node_set);
    unordered_set_t_free(p_node->p_parent_node_set);
    free(p_node);
}
