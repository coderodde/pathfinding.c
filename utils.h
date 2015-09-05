#ifndef GRAPH_UTILS_H
#define	GRAPH_UTILS_H

#include "directed_graph_node.h"
#include "unordered_map.h"

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct point_3d_t {
        double x;
        double y;
        double z;
    } point_3d_t;
    
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
    
    typedef struct graph_data_t {
        directed_graph_node_t**           p_node_array;
        directed_graph_weight_function_t* p_weight_function;
        unordered_map_t*                  p_point_map;
    } graph_data_t;
    
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
#ifdef	__cplusplus
}
#endif

#endif	/* GRAPH_UTILS_H */

