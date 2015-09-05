#ifndef GRAPH_UTILS_H
#define	GRAPH_UTILS_H

#include "directed_graph_node.h"
#include "unordered_map.h"
#include "weight_function.h"
#include "list.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct point_3d_t {
        double x;
        double y;
        double z;
    } point_3d_t;

    typedef struct graph_data_t {
        directed_graph_node_t**           p_node_array;
        directed_graph_weight_function_t* p_weight_function;
        unordered_map_t*                  p_point_map;
    } graph_data_t;

    point_3d_t* random_point(double maxx, double maxy, double maxz);

    double point_3d_t_distance(point_3d_t* p_a, point_3d_t* p_b);

    directed_graph_node_t* choose(directed_graph_node_t** p_table,
                                  const size_t size);

    graph_data_t* create_random_graph(const size_t nodes, 
                                      size_t edges,
                                      const double maxx,
                                      const double maxy,
                                      const double maxz);

    list_t* traceback_path(directed_graph_node_t* p_target,
                           unordered_map_t* p_parent_map);

    bool is_valid_path(list_t* p_path);

    double compute_path_cost(
    list_t* p_path, directed_graph_weight_function_t* p_weight_function);

#ifdef	__cplusplus
}
#endif

#endif	/* GRAPH_UTILS_H */

