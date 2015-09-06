#include "directed_graph_node.h"
#include "unordered_map.h"
#include "utils.h"
#include "list.h"
#include <math.h>

point_3d_t* random_point(double maxx, double maxy, double maxz) 
{
    point_3d_t* p_ret = malloc(sizeof(*p_ret));

    if (!p_ret) return NULL;

    p_ret->x = ((double) rand() / (double) RAND_MAX) * maxx;
    p_ret->y = ((double) rand() / (double) RAND_MAX) * maxy;
    p_ret->z = ((double) rand() / (double) RAND_MAX) * maxz;

    return p_ret;
}

double point_3d_t_distance(point_3d_t* p_a, point_3d_t* p_b) 
{
    double dx = p_a->x - p_b->x;
    double dy = p_a->y - p_b->y;
    double dz = p_a->z - p_b->z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

int priority_cmp(void* pa, void* pb) 
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
    
void search_state_t_alloc(search_state_t* p_state) 
{
    if (!p_state) return;

    p_state->p_weight_list = list_t_alloc(INITIAL_CAPACITY);
    p_state->p_open_set    = heap_t_alloc(4, 
                                          INITIAL_CAPACITY,
                                          LOAD_FACTOR,
                                          hash_function,
                                          equals_function,
                                          priority_cmp);

    p_state->p_closed_set  = unordered_set_t_alloc(INITIAL_CAPACITY,
                                                   LOAD_FACTOR,
                                                   hash_function,
                                                   equals_function);

    p_state->p_parent_map  = unordered_map_t_alloc(INITIAL_CAPACITY,
                                                   LOAD_FACTOR,
                                                   hash_function,
                                                   equals_function);

    p_state->p_cost_map  = unordered_map_t_alloc(INITIAL_CAPACITY,
                                                 LOAD_FACTOR,
                                                 hash_function,
                                                 equals_function);
}

bool search_state_t_is_ready(search_state_t* p_state) 
{
    return p_state->p_weight_list 
            && p_state->p_open_set
            && p_state->p_closed_set
            && p_state->p_parent_map
            && p_state->p_cost_map;
}

void search_state_t_free(search_state_t* p_state) 
{
    if (p_state->p_weight_list) list_t_free(p_state->p_weight_list);
    if (p_state->p_open_set)    heap_t_free(p_state->p_open_set);
    if (p_state->p_closed_set)  unordered_set_t_free(p_state->p_closed_set);
    if (p_state->p_parent_map)  unordered_map_t_free(p_state->p_parent_map);
    if (p_state->p_cost_map)    unordered_map_t_free(p_state->p_cost_map);
}
    
directed_graph_node_t* choose(directed_graph_node_t** p_table,
                                     const size_t size)
{
    size_t index = rand() % size;
    return p_table[index];
}

graph_data_t* create_random_graph(const size_t nodes, 
                                  size_t edges,
                                  const double maxx,
                                  const double maxy,
                                  const double maxz,
                                  const double max_distance)
{
    size_t i;
    char* p_name;

    directed_graph_node_t*              p_tail;
    directed_graph_node_t*              p_head;
    directed_graph_weight_function_t*   p_weight_function;
    unordered_map_t*                    p_point_map;
    point_3d_t*                         p_a;
    point_3d_t*                         p_b;
    graph_data_t*                       p_ret;
    double                              distance;
    
    p_ret = malloc(sizeof(*p_ret));

    if (!p_ret) return NULL;

    directed_graph_node_t** p_node_array = 
            malloc(sizeof(directed_graph_node_t*) * nodes);

    if (!p_ret) 
    {
        free(p_ret);
        return NULL;
    }

    if (!(p_weight_function = 
            directed_graph_weight_function_t_alloc(hash_function,
                                                   equals_function)))
    {
        free(p_ret);
        free(p_node_array);
        return NULL;
    }

    if (!(p_point_map = unordered_map_t_alloc(16, 
                                              1.0f, 
                                              hash_function,
                                              equals_function)))  
    {
        directed_graph_weight_function_t_free(p_weight_function);
        free(p_ret);
        free(p_node_array);
        return NULL;
    }

    for (i = 0; i < nodes; ++i) 
    {
        p_name = malloc(sizeof(char) * 20);
        sprintf(p_name, "%d", i);
        p_node_array[i] = directed_graph_node_t_alloc(p_name);
        unordered_map_t_put(p_point_map, 
                            p_node_array[i], 
                            random_point(maxx, maxy, maxz));
    }

    while (edges > 0)
    {
        p_tail = choose(p_node_array, nodes);
        p_head = choose(p_node_array, nodes);

        p_a = unordered_map_t_get(p_point_map, p_tail);
        p_b = unordered_map_t_get(p_point_map, p_head);

        distance = point_3d_t_distance(p_a, p_b);
        
        if (distance >= max_distance) 
        {
            continue;
        }
        
        directed_graph_node_t_add_arc(p_tail, p_head);

        directed_graph_weight_function_t_put(
                p_weight_function,
                p_tail,
                p_head,
                1.5 * distance);

        --edges;
    }

    p_ret->p_node_array      = p_node_array;
    p_ret->p_weight_function = p_weight_function;
    p_ret->p_point_map       = p_point_map;

    return p_ret;
}

list_t* traceback_path(directed_graph_node_t* p_target,
                       unordered_map_t* p_parent_map)
{
    list_t* p_ret;
    directed_graph_node_t* p_current;

    if (!p_target)     return NULL;
    if (!p_parent_map) return NULL;

    p_ret = list_t_alloc(10);

    if (!p_ret)        return NULL;

    p_current = p_target;

    while (p_current) 
    {
        list_t_push_front(p_ret, p_current);
        p_current = unordered_map_t_get(p_parent_map, p_current);
    }

    return p_ret;
}

list_t* traceback_bidirectional_path(directed_graph_node_t* p_middle_node,
                                     unordered_map_t* p_parent_map_a,
                                     unordered_map_t* p_parent_map_b)
{
    list_t* p_ret;
    directed_graph_node_t* p_current;
    
    if (!p_middle_node)  return NULL;
    if (!p_parent_map_a) return NULL;
    if (!p_parent_map_b) return NULL;
    
    p_ret = list_t_alloc(INITIAL_CAPACITY);
    
    if (!p_ret) return NULL;
    
    p_current = p_middle_node;
    
    while (p_current) 
    {
        list_t_push_front(p_ret, p_current);
        p_current = unordered_map_t_get(p_parent_map_a, p_current);
    }
    
    p_current = unordered_map_t_get(p_parent_map_b, p_middle_node);
    
    while (p_current)
    {
        list_t_push_back(p_ret, p_current);
        p_current = unordered_map_t_get(p_parent_map_b, p_current);
    }
    
    return p_ret;
}

bool is_valid_path(list_t* p_path)
{
    size_t i;
    size_t sz;

    if (!p_path) return false;

    /* A empty path is defined to be valid. */
    if ((sz = list_t_size(p_path)) == 0) return true;

    for (i = 0; i < sz - 1; ++i) 
    {
        if (!directed_graph_node_t_has_child(list_t_get(p_path, i),
                                             list_t_get(p_path, i + 1))) 
        {
            return false;
        }
    }

    return true;
}

double compute_path_cost(list_t* p_path, 
                         directed_graph_weight_function_t* p_weight_function)
{
    size_t i;
    size_t sz;
    double cost = 0.0;

    if (!p_path) return 0.0;

    /* A empty path is defined to be valid. */
    if ((sz = list_t_size(p_path)) == 0) return 0.0;

    for (i = 0; i < sz - 1; ++i) 
    {
        cost += *directed_graph_weight_function_t_get(p_weight_function,
                                                      list_t_get(p_path, i),
                                                      list_t_get(p_path, i + 1));
    }

    return cost;
}
