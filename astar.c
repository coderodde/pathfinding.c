#include "astar.h"
#include "directed_graph_node.h"
#include "weight_function.h"
#include "unordered_map.h"
#include "heap.h"
#include "list.h"
#include "utils.h"

static double heuristic_cost(unordered_map_t* p_location_map,
                             directed_graph_node_t* p_node_a,
                             directed_graph_node_t* p_node_b)
{
    point_3d_t* p_point_a = unordered_map_t_get(p_location_map, p_node_a);
    point_3d_t* p_point_b = unordered_map_t_get(p_location_map, p_node_b);
    return point_3d_t_distance(p_point_a, p_point_b);
}

list_t* astar(directed_graph_node_t* p_source,
              directed_graph_node_t* p_target,
              directed_graph_weight_function_t* p_weight_function,
              unordered_map_t* p_location_map_arg)
{
    search_state_t            state;

    list_t*                   p_list;
    heap_t*                   p_open_set;
    unordered_set_t*          p_closed_set;
    unordered_map_t*          p_parent_map;
    unordered_map_t*          p_cost_map;
    unordered_map_t*          p_location_map;
    directed_graph_node_t*    p_current;
    directed_graph_node_t*    p_child;
    unordered_set_iterator_t* p_child_iterator;

    /* Cannot pack a double into a void*, so use these simple structures. */
    weight_t*                 p_weight;
    weight_t*                 p_weight_f;
    list_t*                   p_weight_list;
    size_t                    i;

    if (!p_source)           return NULL;
    if (!p_target)           return NULL;
    if (!p_weight_function)  return NULL;
    if (!p_location_map_arg) return NULL;

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

    p_location_map = p_location_map_arg;

    p_weight = malloc(sizeof(*p_weight));
    p_weight->weight = 0.0;

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

            tmp_cost += *directed_graph_weight_function_t_get(p_weight_function, 
                                                              p_current, 
                                                              p_child);

            double estimate = heuristic_cost(p_location_map, p_child, p_target);

            if (!unordered_map_t_contains_key(p_parent_map, p_child)) 
            {
                /* Prepare the distance so far to 'p_child'. */
                p_weight = malloc(sizeof(*p_weight));
                p_weight->weight = tmp_cost;

                /* Prepare the f-distance of 'p_child'. */
                p_weight_f = malloc(sizeof(*p_weight_f));
                p_weight_f->weight = tmp_cost + estimate;

                heap_t_add(p_open_set, p_child, p_weight_f);
                unordered_map_t_put(p_parent_map, p_child, p_current);
                unordered_map_t_put(p_cost_map, p_child, p_weight);

                /**********************************************************
                * Store the weight objects so that we can free them after *
                * the search.                                             *
                **********************************************************/
                list_t_push_back(p_weight_list, p_weight);
                list_t_push_back(p_weight_list, p_weight_f);
            }
            else if (tmp_cost < 
                    ((weight_t*) unordered_map_t_get(p_cost_map, 
                                                     p_child))->weight)
            {
                /* Prepare the distance so far to 'p_child'. */
                p_weight = malloc(sizeof(*p_weight));
                p_weight->weight = tmp_cost;

                /* Prepare the f-distance of 'p_child'. */
                p_weight_f = malloc(sizeof(*p_weight));
                p_weight_f->weight = tmp_cost + estimate;

                heap_t_decrease_key(p_open_set, p_child, p_weight_f);
                unordered_map_t_put(p_parent_map, p_child, p_current);
                unordered_map_t_put(p_cost_map, p_child, p_weight);

                /**********************************************************
                * Store the weight objects so that we can free them after *
                * the search.                                             *
                **********************************************************/
                list_t_push_back(p_weight_list, p_weight);
                list_t_push_back(p_weight_list, p_weight_f);
            }
        }

        unordered_set_iterator_t_free(p_child_iterator);
    }

    /* Once here, return a empty path in order to denote the fact that the 
       target node is not reachable from source node. */
    p_list = list_t_alloc(10);
    search_state_t_free(&state);

    /* Deallocate the weights. */
    for (i = 0; i < list_t_size(p_weight_list); ++i) 
    {
        free(list_t_get(p_weight_list, i));
    }

    return p_list;
}
