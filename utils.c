#include "directed_graph_node.h"
#include "unordered_map.h"
#include "utils.h"
#include "list.h"

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
                                  const double maxz)
{
    size_t i;
    char text[80];

    directed_graph_node_t*              p_tail;
    directed_graph_node_t*              p_head;
    directed_graph_weight_function_t*   p_weight_function;
    unordered_map_t*                    p_point_map;
    point_3d_t*                         p_a;
    point_3d_t*                         p_b;
    graph_data_t*                       p_ret;

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
        sprintf(text, "%d", i);
        p_node_array[i] = directed_graph_node_t_alloc(text);
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

        directed_graph_weight_function_t_put(
                p_weight_function,
                p_tail,
                p_head,
                1.2 * point_3d_t_distance(p_a, p_b));

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
