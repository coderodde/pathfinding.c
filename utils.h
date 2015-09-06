#ifndef GRAPH_UTILS_H
#define	GRAPH_UTILS_H

#include "directed_graph_node.h"
#include "unordered_map.h"
#include "weight_function.h"
#include "list.h"
#include "heap.h"

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

    typedef struct search_state_t {
        list_t*          p_weight_list;
        heap_t*          p_open_set;
        unordered_set_t* p_closed_set;
        unordered_map_t* p_parent_map;
        unordered_map_t* p_cost_map;
    } search_state_t;
    
    typedef struct weight_t {
        double weight;
    } weight_t;

    int priority_cmp(void* pa, void* pb);
    
    static const size_t INITIAL_CAPACITY = 16;
    static const float  LOAD_FACTOR = 1.0f;
    
    void search_state_t_alloc(search_state_t* p_state);
    bool search_state_t_is_ready(search_state_t* p_state);
    void search_state_t_free(search_state_t* p_state);
    
    point_3d_t* random_point(double maxx, double maxy, double maxz);

    double point_3d_t_distance(point_3d_t* p_a, point_3d_t* p_b);

    directed_graph_node_t* choose(directed_graph_node_t** p_table,
                                  const size_t size);

    graph_data_t* create_random_graph(const size_t nodes, 
                                      size_t edges,
                                      const double maxx,
                                      const double maxy,
                                      const double maxz,
                                      const double max_distance);

    list_t* traceback_path(directed_graph_node_t* p_target,
                           unordered_map_t* p_parent_map);

    list_t* traceback_bidirectional_path(directed_graph_node_t* p_middle_node,
                                         unordered_map_t* p_parent_map_a,
                                         unordered_map_t* p_parent_map_b);
    
    bool is_valid_path(list_t* p_path);

    double compute_path_cost(
    list_t* p_path, directed_graph_weight_function_t* p_weight_function);

#ifdef	__cplusplus
}
#endif

#endif	/* GRAPH_UTILS_H */

