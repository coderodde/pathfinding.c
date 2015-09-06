#include "directed_graph_node.h"
#include "list.h"
#include "weight_function.h"
#include "utils.h"
#include <float.h>
#include <stdlib.h>

static void free_weight_list(list_t* p_list) 
{
    size_t i;
    
    for (i = 0; i < list_t_size(p_list); ++i) 
    {
        free(list_t_get(p_list, i));
    }
}

list_t* bidirectional_dijkstra(directed_graph_node_t* p_source,
                               directed_graph_node_t* p_target,
                               directed_graph_weight_function_t* 
                                                      p_weight_function)
{
    if (equals_function(p_source, p_target))
    {
        list_t* p_path = list_t_alloc(1);
        list_t_push_back(p_path, p_source);
        return p_path;
    }
    
    search_state_t forward_search_state;
    search_state_t backward_search_state;
    
    heap_t* p_open_set_a;
    heap_t* p_open_set_b;
    
    unordered_set_t* p_closed_set_a;
    unordered_set_t* p_closed_set_b;
    
    unordered_map_t* p_parent_map_a;
    unordered_map_t* p_parent_map_b;
    
    unordered_map_t* p_cost_map_a;
    unordered_map_t* p_cost_map_b;
    
    list_t* p_weight_list;
    
    weight_t* p_weight;
    size_t i;
    
    directed_graph_node_t* p_touch_node = NULL;
    double best_path_cost = DBL_MAX;
    
    search_state_t_alloc(&forward_search_state);
    
    if (!search_state_t_is_ready(&forward_search_state)) 
    {
        search_state_t_free(&forward_search_state);
        return NULL;
    }
    
    search_state_t_alloc(&backward_search_state);
    
    if (!search_state_t_is_ready(&backward_search_state)) 
    {
        search_state_t_free(&forward_search_state);
        search_state_t_free(&backward_search_state);
    }
    
    p_weight_list = list_t_alloc(INITIAL_CAPACITY);
    
    if (!p_weight_list) 
    {
        search_state_t_free(&forward_search_state);
        search_state_t_free(&backward_search_state);
    }
    
    p_open_set_a   = forward_search_state.p_open_set;
    p_closed_set_a = forward_search_state.p_closed_set;
    p_parent_map_a = forward_search_state.p_parent_map;
    p_cost_map_a   = forward_search_state.p_cost_map;
    p_weight_list  = forward_search_state.p_weight_list;
    
    p_open_set_b   = backward_search_state.p_open_set;
    p_closed_set_b = backward_search_state.p_closed_set;
    p_parent_map_b = backward_search_state.p_parent_map;
    p_cost_map_b   = backward_search_state.p_cost_map;
    
    /* Initialize the forward search structures. */
    p_weight = malloc(sizeof(*p_weight));
    p_weight->weight = 0.0;
    
    list_t_push_back(p_weight_list, p_weight);
    heap_t_add(p_open_set_a, p_source, p_weight);
    unordered_map_t_put(p_cost_map_a, p_source, p_weight);
    
    /* Initialize the backward search structures. */
    p_weight = malloc(sizeof(*p_weight));
    p_weight->weight = 0.0;
    
    list_t_push_back(p_weight_list, p_weight);
    heap_t_add(p_open_set_b, p_target, p_weight);
    unordered_map_t_put(p_cost_map_b, p_target, p_weight);
    
    /* Initialize the parent maps. */
    unordered_map_t_put(p_parent_map_a, p_source, NULL);
    unordered_map_t_put(p_parent_map_b, p_target, NULL);
    
    while (heap_t_size(p_open_set_a) > 0 && heap_t_size(p_open_set_b) > 0) 
    {
        double top_a_cost = 
        ((weight_t*) unordered_map_t_get(p_cost_map_a, 
                                         heap_t_min(p_open_set_a)))->weight;
        
        double top_b_cost =
        ((weight_t*) unordered_map_t_get(p_cost_map_b,
                                         heap_t_min(p_open_set_b)))->weight;
        
        double top_sum = top_a_cost + top_b_cost;
        
        if (top_sum > best_path_cost)
        {
            list_t* p_path = traceback_bidirectional_path(p_touch_node,
                                                          p_parent_map_a,
                                                          p_parent_map_b);
            free_weight_list(p_weight_list);
            search_state_t_free(&forward_search_state);
            search_state_t_free(&backward_search_state);
            return p_path;
        }
        
        if (heap_t_size(p_open_set_a) <= heap_t_size(p_open_set_b))
        {
            directed_graph_node_t* p_current = heap_t_extract_min(p_open_set_a);
            
            unordered_set_t_add(p_closed_set_a, p_current);
            
            unordered_set_iterator_t* p_child_iterator = 
                    unordered_set_iterator_t_alloc(
                        directed_graph_node_t_children_set(p_current));
            
            while (unordered_set_iterator_t_has_next(p_child_iterator))
            {
                directed_graph_node_t* p_child;
                unordered_set_iterator_t_next(p_child_iterator, &p_child);
                
                if (unordered_set_t_contains(p_closed_set_a, p_child)) 
                {
                    continue;
                }
                
                double tmp_g_score = 
                    ((weight_t*) unordered_map_t_get(p_cost_map_a,
                                                     p_current))->weight +
                    *directed_graph_weight_function_t_get(p_weight_function, 
                                                          p_current, 
                                                          p_child);
                
                if (!unordered_map_t_contains_key(p_cost_map_a, p_child)) 
                {
                    p_weight = malloc(sizeof(*p_weight));
                    p_weight->weight = tmp_g_score;
                    heap_t_add(p_open_set_a, p_child, p_weight);
                    unordered_map_t_put(p_cost_map_a, p_child, p_weight);
                    unordered_map_t_put(p_parent_map_a, p_child, p_current);
                    
                    // Check whether improvement of the path is possible.
                    if (unordered_set_t_contains(p_closed_set_b, p_child)) 
                    {
                        double path_cost = tmp_g_score + 
                        ((weight_t*) unordered_map_t_get(p_cost_map_b, 
                                                         p_child))->weight;
                        
                        if (best_path_cost > path_cost)
                        {
                            best_path_cost = path_cost;
                            p_touch_node = p_child;
                        }
                    }
                }
                else
                {
                    double old_score = 
                    ((weight_t*) unordered_map_t_get(p_cost_map_a,
                                                     p_child))->weight;
                    if (old_score > tmp_g_score)
                    {
                        p_weight = malloc(sizeof(*p_weight));
                        p_weight->weight = tmp_g_score;
                        
                        list_t_push_back(p_weight_list, p_weight);
                        heap_t_decrease_key(p_open_set_a, p_child, p_weight);
                        unordered_map_t_put(p_cost_map_a, p_child, p_weight);
                        unordered_map_t_put(p_parent_map_a, p_child, p_current);
                        
                        if (unordered_set_t_contains(p_closed_set_b, p_child)) 
                        {
                            double path_cost = tmp_g_score + 
                            ((weight_t*) unordered_map_t_get(p_cost_map_b, 
                                                             p_child))->weight;

                            if (best_path_cost > path_cost)
                            {
                                best_path_cost = path_cost;
                                p_touch_node = p_child;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            directed_graph_node_t* p_current = heap_t_extract_min(p_open_set_b);
            
            unordered_set_t_add(p_closed_set_b, p_current);
            
            unordered_set_iterator_t* p_parent_iterator = 
                    unordered_set_iterator_t_alloc(
                        directed_graph_node_t_parent_set(p_current));
            
            while (unordered_set_iterator_t_has_next(p_parent_iterator)) 
            {
                directed_graph_node_t* p_parent;
                unordered_set_iterator_t_next(p_parent_iterator, &p_parent);
                
                if (unordered_set_t_contains(p_closed_set_b, p_parent)) 
                {
                    continue;
                }
                
                double tmp_g_score = 
                    ((weight_t*) unordered_map_t_get(p_cost_map_b,
                                                     p_current))->weight +
                    *directed_graph_weight_function_t_get(p_weight_function, 
                                                          p_parent, 
                                                          p_current);
                
                if (!unordered_map_t_contains_key(p_cost_map_b, p_parent)) 
                {
                    p_weight = malloc(sizeof(*p_weight));
                    p_weight->weight = tmp_g_score;
                    heap_t_add(p_open_set_b, p_parent, p_weight);
                    unordered_map_t_put(p_cost_map_b, p_parent, p_weight);
                    unordered_map_t_put(p_parent_map_b, p_parent, p_current);
                    
                    // Check whether improvement of the path is possible.
                    if (unordered_set_t_contains(p_closed_set_a, p_parent)) 
                    {
                        double path_cost = tmp_g_score + 
                        ((weight_t*) unordered_map_t_get(p_cost_map_a, 
                                                         p_parent))->weight;
                        
                        if (best_path_cost > path_cost)
                        {
                            best_path_cost = path_cost;
                            p_touch_node = p_parent;
                        }
                    }
                }
                else
                {
                    double old_score = 
                    ((weight_t*) unordered_map_t_get(p_cost_map_b,
                                                     p_parent))->weight;
                    if (old_score > tmp_g_score)
                    {
                        p_weight = malloc(sizeof(*p_weight));
                        p_weight->weight = tmp_g_score;
                        
                        list_t_push_back(p_weight_list, 
                                         p_weight);
                        
                        heap_t_decrease_key(p_open_set_b, 
                                            p_parent, 
                                            p_weight);
                        
                        unordered_map_t_put(p_cost_map_b, 
                                            p_parent, 
                                            p_weight);
                        
                        unordered_map_t_put(p_parent_map_b, 
                                            p_parent, 
                                            p_current);
                        
                        if (unordered_set_t_contains(p_closed_set_a, p_parent)) 
                        {
                            double path_cost = tmp_g_score + 
                            ((weight_t*) unordered_map_t_get(p_cost_map_a, 
                                                             p_parent))->weight;

                            if (best_path_cost > path_cost)
                            {
                                best_path_cost = path_cost;
                                p_touch_node = p_parent;
                            }
                        }
                    }
                }
            }
        }
    }
    
    free_weight_list(p_weight_list);
    search_state_t_free(&forward_search_state);
    search_state_t_free(&backward_search_state);
    return list_t_alloc(INITIAL_CAPACITY);
}