#ifndef BIDIR_ASTAR_H
#define	BIDIR_ASTAR_H

#ifdef	__cplusplus
extern "C" {
#endif

    list_t* bidirectional_astar
        (directed_graph_node_t* p_source,
         directed_graph_node_t* p_target,
         directed_graph_weight_function_t* p_weight_function,
         unordered_map_t*       p_location_map);    


#ifdef	__cplusplus
}
#endif

#endif	/* BIDIR_ASTAR_H */

