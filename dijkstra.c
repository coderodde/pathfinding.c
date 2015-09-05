#include "dijkstra.h"
#include "list.h"
#include "directed_graph_node.h"
#include "weight_function.h"
#include "unordered_map.h"
#include "unordered_set.h"
#include "heap.h"

typedef struct weight_t {
    double weight;
} weight_t;

static int priority_cmp(void* pa, void* pb) 
{
    double da;
    double db;
    
    da = ((weight_t*) pa)->weight;
    db = ((weight_t*) pb)->weight;
    
    if (da < db) 
    {
        return -1;
    }
    else if (da > db) 
    {
        return 1;
    }
    
    return 0;
}

static const size_t INITIAL_CAPACITY = 16;
static const float  LOAD_FACTOR      = 1.0f;

list_t* dijkstra(directed_graph_node_t* p_source,
                 directed_graph_node_t* p_target,
                 directed_graph_weight_function_t* p_weight_function)
{
    list_t*                   p_path_list;
    heap_t*                   p_open_set;
    unordered_set_t*          p_closed_set;
    unordered_map_t*          p_parent_map;
    unordered_map_t*          p_cost_map;
    directed_graph_node_t*    p_current;
    directed_graph_node_t*    p_child;
    unordered_set_iterator_t* p_child_iterator;
    weight_t*                 p_weight;
    list_t*                   p_weight_list;
    
    if (!p_source)          return NULL;
    if (!p_target)          return NULL;
    if (!p_weight_function) return NULL;
    
    p_path_list = list_t_alloc(INITIAL_CAPACITY);
    
    if (!p_path_list) return NULL;
    
    p_open_set = heap_t_alloc(4,
                              INITIAL_CAPACITY,
                              LOAD_FACTOR,
                              hash_function,
                              equals_function,
                              priority_cmp);
    
    if (!p_open_set) 
    {
        list_t_free(p_path_list);
        return NULL;
    }
    
    p_closed_set = unordered_set_t_alloc(INITIAL_CAPACITY,
                                         LOAD_FACTOR,
                                         hash_function,
                                         equals_function);
    
    if (!p_closed_set) 
    {
        list_t_free(p_path_list);
        heap_t_free(p_open_set);
        return NULL;
    }
    
    p_parent_map = unordered_map_t_alloc(INITIAL_CAPACITY,
                                         LOAD_FACTOR,
                                         hash_function,
                                         equals_function);
    
    if (!p_parent_map) 
    {
        list_t_free(p_path_list);
        heap_t_free(p_open_set);
        unordered_set_t_free(p_closed_set);
        return NULL;
    }
    
    p_cost_map = unordered_map_t_alloc(INITIAL_CAPACITY,
                                       LOAD_FACTOR,
                                       hash_function,
                                       equals_function);
    
    if (!p_cost_map)
    {
        list_t_free(p_path_list);
        heap_t_free(p_open_set);
        unordered_set_t_free(p_closed_set);
        unordered_map_t_free(p_parent_map);
        return NULL;
    }
    
    p_weight_list = list_t_alloc(INITIAL_CAPACITY);
    
    if (!p_weight_list) 
    {
        list_t_free(p_path_list);
        heap_t_free(p_open_set);
        unordered_set_t_free(p_closed_set);
        unordered_map_t_free(p_parent_map);
        unordered_map_t_free(p_cost_map);
        return NULL;
    }
     
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
            puts("POOP BETWEEN TWO NODES! ^^");
            return NULL;
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
            }
            else if (tmp_cost < ((weight_t*) unordered_map_t_get(p_cost_map, p_child))->weight)
            {
                p_weight = malloc(sizeof(*p_weight));
                p_weight->weight = tmp_cost;
                
                heap_t_decrease_key(p_open_set, p_child, p_weight);
                unordered_map_t_put(p_parent_map, p_child, p_current);
                unordered_map_t_put(p_cost_map, p_child, p_weight);
            }
        }
    }
    
    /* Once here, return a empty path in order to denote the fact that the 
       target node is not reachable from source node. */
    puts("NOPOOOP");
    return p_path_list;
}
