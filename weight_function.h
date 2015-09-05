#ifndef WEIGHT_FUNCTION_H
#define	WEIGHT_FUNCTION_H

#include "directed_graph_node.h"
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct directed_graph_weight_function_t 
                   directed_graph_weight_function_t;

    /***************************************************************************
    * Allocates a new, empty weight function.                                  *
    ***************************************************************************/  
    directed_graph_weight_function_t* 
    directed_graph_weight_function_t_alloc(size_t (*p_hash_function)(void*),
                                           bool (*p_equals_function)(void*, 
                                                                     void*));

    /***************************************************************************
    * Associates the weight 'weight' with the arc ('p_tail', 'p_head').        *
    ***************************************************************************/  
    bool directed_graph_weight_function_t_put
        (directed_graph_weight_function_t* p_function,
         directed_graph_node_t* p_tail,
         directed_graph_node_t* p_head,
         double weight);

    /***************************************************************************
    * Reads the weight for the arc ('p_tail', 'p_head').                       *
    ***************************************************************************/  
    double* directed_graph_weight_function_t_get(
            directed_graph_weight_function_t* p_function,
            directed_graph_node_t* p_tail,
            directed_graph_node_t* p_head);

    /***************************************************************************
    * Deallocate the weight function.                                          *
    ***************************************************************************/  
    void directed_graph_weight_function_t_free
        (directed_graph_weight_function_t* p_function);

#ifdef	__cplusplus
}
#endif

#endif	/* WEIGHT_FUNCTION_H */

