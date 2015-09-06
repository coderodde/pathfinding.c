#ifndef BIDIR_DIJKSTRA_H
#define	BIDIR_DIJKSTRA_H

#include "directed_graph_node.h"
#include "list.h"
#include "weight_function.h"

#ifdef	__cplusplus
extern "C" {
#endif

    list_t* bidirectional_dijkstra
        (directed_graph_node_t* p_source,
         directed_graph_node_t* p_target,
         directed_graph_weight_function_t* p_weight_function);    

#ifdef	__cplusplus
}
#endif

#endif	/* BIDIR_DIJKSTRA_H */
