#include "dijkstra.h"
#include "list.h"
#include "directed_graph_node.h"
#include "weight_function.h"
#include "unordered_map.h"
#include "unordered_set.h"
#include "heap.h"
#include "utils.h"

list_t* dijkstra(directed_graph_node_t* p_source,
                 directed_graph_node_t* p_target,
                 directed_graph_weight_function_t* p_weight_function)
{
    search_state_t            state;
    
    list_t*                   p_weight_list;
    heap_t*                   p_open_set;
    unordered_set_t*          p_closed_set;
    unordered_map_t*          p_parent_map;
    unordered_map_t*          p_cost_map;
    
    list_t*                   p_list;
    directed_graph_node_t*    p_current;
    directed_graph_node_t*    p_child;
    unordered_set_iterator_t* p_child_iterator;
    size_t                    i;
    
    /* Cannot pack a double into a void*, so use this simple structure. */
    weight_t*                 p_weight;

    if (!p_source)          return NULL;
    if (!p_target)          return NULL;
    if (!p_weight_function) return NULL;
    
    search_state_t_alloc(&state);
    
    if (!search_state_t_is_ready(&state)) 
    {
        search_state_t_free(&state);
        return NULL;
    }

    p_weight = malloc(sizeof(*p_weight));
    p_weight->weight = 0.0;

    p_open_set    = state.p_open_set;
    p_closed_set  = state.p_closed_set;
    p_weight_list = state.p_weight_list;
    p_cost_map    = state.p_cost_map;
    p_parent_map  = state.p_parent_map;
    
    heap_t_add(p_open_set, p_source, p_weight);
    unordered_map_t_put(p_parent_map, p_source, NULL);
    unordered_map_t_put(p_cost_map, p_source, p_weight);
    list_t_push_back(p_weight_list, p_weight);

    while (heap_t_size(p_open_set) > 0)
    {
        p_current = heap_t_extract_min(p_open_set);

        if (equals_function(p_current, p_target)) 
        {
            p_list = traceback_path(p_target, p_parent_map);
            search_state_t_free(&state);
            return p_list;
        }

        unordered_set_t_add(p_closed_set, p_current);

        p_child_iterator = 
                unordered_set_iterator_t_alloc(
                    directed_graph_node_t_children_set(p_current));

        while (unordered_set_iterator_t_has_next(p_child_iterator)) 
        {
            unordered_set_iterator_t_next(p_child_iterator, &p_child);

            if (unordered_set_t_contains(p_closed_set, p_child)) {
                continue;
            }

            double tmp_cost = 
                ((weight_t*) unordered_map_t_get(p_cost_map, 
                                                 p_current))->weight;

            tmp_cost += *directed_graph_weight_function_t_get(
                    p_weight_function, 
                    p_current, 
                    p_child);

            if (!unordered_map_t_contains_key(p_parent_map, p_child)) 
            {
                p_weight = malloc(sizeof(*p_weight));
                p_weight->weight = tmp_cost;

                heap_t_add(p_open_set, p_child, p_weight);
                unordered_map_t_put(p_parent_map, p_child, p_current);
                unordered_map_t_put(p_cost_map, p_child, p_weight);
                
                list_t_push_back(p_weight_list, p_weight);
            }
            else if (tmp_cost < 
                    ((weight_t*) unordered_map_t_get(p_cost_map, 
                                                     p_child))->weight)
            {
                p_weight = malloc(sizeof(*p_weight));
                p_weight->weight = tmp_cost;

                heap_t_decrease_key(p_open_set, p_child, p_weight);
                unordered_map_t_put(p_parent_map, p_child, p_current);
                unordered_map_t_put(p_cost_map, p_child, p_weight);
                
                list_t_push_back(p_weight_list, p_weight);
            }
        }

        unordered_set_iterator_t_free(p_child_iterator);
    }

    /* Once here, return a empty path in order to denote the fact that the 
       target node is not reachable from source node. */
    search_state_t_free(&state);
    p_list = list_t_alloc(10);

    /* Deallocate the weights. */
    for (i = 0; i < list_t_size(p_weight_list); ++i) 
    {
        free(list_t_get(p_weight_list, i));
    }

    return p_list;
}
