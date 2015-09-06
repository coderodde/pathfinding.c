#ifndef ASTAR_H
#define	ASTAR_H

#include "directed_graph_node.h"
#include "weight_function.h"
#include "unordered_map.h"
#include "list.h"

#ifdef	__cplusplus
extern "C" {
#endif

    list_t* astar(directed_graph_node_t* p_source,
                  directed_graph_node_t* p_target,
                  directed_graph_weight_function_t* p_weight_function,
                  unordered_map_t* p_location_map);    

#ifdef	__cplusplus
}
#endif

#endif	/* ASTAR_H */

