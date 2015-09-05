#ifndef DIRECTED_GRAPH_NODE_H
#define	DIRECTED_GRAPH_NODE_H

#include "unordered_set.h"
#include <stdbool.h>
#include <string.h>

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct directed_graph_node_t directed_graph_node_t;

    /***************************************************************************
    * The function for testing node equality.                                  *
    ***************************************************************************/  
    bool equals_function(void* a, void* b);

    /***************************************************************************
    * The function for computing the hash values for nodes.                    *
    ***************************************************************************/  
    size_t hash_function(void* v);

    /***************************************************************************
    * Allocates a new directed graph node with given name.                     *
    ***************************************************************************/  
    directed_graph_node_t* directed_graph_node_t_alloc(char* name);

    /***************************************************************************
    * Creates an arc (p_tail, p_head) and returns true if the arc is actually  *
    * created. 'p_tail' is called a "parent" of 'p_head', and 'p_head' is      *
    * called a "child" of 'p_tail'.                                            *  
    ***************************************************************************/  
    bool                   
    directed_graph_node_t_add_arc(directed_graph_node_t* p_tail,
                                  directed_graph_node_t* p_head);

    /***************************************************************************
    * Returns true if 'p_node' has a child 'p_child_candidate'.                *
    ***************************************************************************/  
    bool directed_graph_node_t_has_child
    (directed_graph_node_t* p_node, directed_graph_node_t* p_child_candidate);

    /***************************************************************************
    * Removes the arc (p_tail, p_node) between the two nodes.                  *
    ***************************************************************************/  
    bool directed_graph_node_t_remove_arc(directed_graph_node_t* p_tail,
                                          directed_graph_node_t* p_head);

    /***************************************************************************
    * Returns the textual representation of the node.                          *
    ***************************************************************************/  
    char* directed_graph_node_t_to_string(directed_graph_node_t* p_node);

    /***************************************************************************
    * Returns the set containing all the child nodes of the given node.        *
    ***************************************************************************/  
    unordered_set_t* 
    directed_graph_node_t_children_set(directed_graph_node_t* p_node);

    /***************************************************************************
    * Returns the set containing all the parent nodes of the given node.       *
    ***************************************************************************/  
    unordered_set_t*
    directed_graph_node_t_parent_set(directed_graph_node_t* p_node);

    /***************************************************************************
    * Removes all the arcs involving the input node.                           *
    ***************************************************************************/  
    void directed_graph_node_t_clear(directed_graph_node_t* p_node);

    /***************************************************************************
    * Deallocates the node.                                                    *
    ***************************************************************************/  
    void directed_graph_node_t_free(directed_graph_node_t* p_node);

#ifdef	__cplusplus
}
#endif

#endif	/* DIRECTED_GRAPH_NODE_H */

