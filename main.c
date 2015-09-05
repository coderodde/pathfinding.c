#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dijkstra.h"
#include "directed_graph_node.h"
#include "weight_function.h"
#include "utils.h"

#define ASSERT(CONDITION) assert(CONDITION, #CONDITION, __FILE__, __LINE__)

static bool assert(bool cond, char* err_msg, char* file_name, int line)
{
    if (!cond)
        fprintf(stderr, 
                "'%s' is not true in file '%s' at line %d.\n", 
                err_msg,
                file_name,
                line);
    
    return cond;
}

static void test_directed_graph_node_correctness()
{
    directed_graph_node_t* p_node_a;
    directed_graph_node_t* p_node_b;
    directed_graph_node_t* p_node_c;
    directed_graph_node_t* p_node_d;
    
    p_node_a = directed_graph_node_t_alloc("Node A");
    p_node_b = directed_graph_node_t_alloc("Node B");
    p_node_c = directed_graph_node_t_alloc("Node C");
    p_node_d = directed_graph_node_t_alloc("Node D");
    
    ASSERT(strcmp(directed_graph_node_t_to_string(p_node_a), 
           "[directed_graph_node_t: id = Node A]") == 0);
    
    ASSERT(strcmp(directed_graph_node_t_to_string(p_node_b), 
           "[directed_graph_node_t: id = Node B]") == 0);
    
    ASSERT(strcmp(directed_graph_node_t_to_string(p_node_c), 
           "[directed_graph_node_t: id = Node C]") == 0);
    
    ASSERT(strcmp(directed_graph_node_t_to_string(p_node_d), 
           "[directed_graph_node_t: id = Node D]") == 0);
    
    
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_a) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_b) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_c) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_d) == false);
    
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_a) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_b) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_c) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_d) == false);
    
    ASSERT(directed_graph_node_t_has_child(p_node_c, p_node_a) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_c, p_node_b) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_c, p_node_c) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_c, p_node_d) == false);
    
    ASSERT(directed_graph_node_t_has_child(p_node_d, p_node_a) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_d, p_node_b) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_d, p_node_c) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_d, p_node_d) == false);
    
    ASSERT(directed_graph_node_t_add_arc(p_node_a, p_node_b));
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_b));
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_a) == false);
    ASSERT(directed_graph_node_t_add_arc(p_node_b, p_node_a));
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_b));
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_a));
    /* a <-> b */
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_a) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_b) == false);
    
    ASSERT(directed_graph_node_t_add_arc(p_node_a, p_node_a));
    /* c a <-> b */
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_a));
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_b) == false);
    
    /* c a <-> b o*/
    ASSERT(directed_graph_node_t_add_arc(p_node_b, p_node_b));
    
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_a));
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_b));
    
    /* c a <-> b o -> d */
    ASSERT(directed_graph_node_t_add_arc(p_node_b, p_node_d));
    directed_graph_node_t_clear(p_node_a);
    
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_a) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_b) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_c) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_d) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_d));
}

static void test_weight_function_correctness()
{
    directed_graph_weight_function_t* p_weight_function;
    
    directed_graph_node_t* p_node_a;
    directed_graph_node_t* p_node_b;
    directed_graph_node_t* p_node_c;
    directed_graph_node_t* p_node_d;
    
    double weight;
    
    p_node_a = directed_graph_node_t_alloc("Node A");
    p_node_b = directed_graph_node_t_alloc("Node B");
    p_node_c = directed_graph_node_t_alloc("Node C");
    p_node_d = directed_graph_node_t_alloc("Node D");
    
    ASSERT(p_weight_function = 
            directed_graph_weight_function_t_alloc(hash_function, 
                                                   equals_function));
    
    ASSERT(directed_graph_weight_function_t_get(p_weight_function, 
                                                p_node_a, 
                                                p_node_c) == NULL);
    
    weight = 2.0;
    
    ASSERT(directed_graph_weight_function_t_put(p_weight_function,
                                                p_node_a,
                                                p_node_c,
                                                weight));
    
    ASSERT(*directed_graph_weight_function_t_get(p_weight_function, 
                                                 p_node_a, 
                                                 p_node_c) == 2.0);
    
    weight = 4.5;
    
    ASSERT(directed_graph_weight_function_t_put(p_weight_function,
                                                p_node_c,
                                                p_node_a,
                                                weight));
    
    ASSERT(*directed_graph_weight_function_t_get(p_weight_function, 
                                                p_node_c, 
                                                p_node_a) == 4.5);
    
    weight = 7.5;
    
    ASSERT(directed_graph_weight_function_t_put(p_weight_function,
                                                p_node_a,
                                                p_node_a,
                                                weight));
    
    ASSERT(*directed_graph_weight_function_t_get(p_weight_function, 
                                                 p_node_a, 
                                                 p_node_a) == 7.5);
    ASSERT(directed_graph_weight_function_t_get(p_weight_function,
                                                p_node_c,
                                                p_node_c) == NULL);
    
    directed_graph_weight_function_t_free(p_weight_function);
}

static void test_dijkstra_correctness()
{
    
}

static const size_t NODES = 100;
static const size_t EDGES = NODES * 5;
static const double MAXX = 1000.0;
static const double MAXY = 500.0;
static const double MAXZ = 100.0;

int main(int argc, char** argv) {
    graph_data_t* p_data;
    clock_t       c;
    int           seed = time(NULL);
    double        duration = 0.0;
    
    directed_graph_node_t* p_source;
    directed_graph_node_t* p_target;
    
    printf("%d\n", sizeof(void*));
    printf("Seed: %d\n", seed);
    srand(seed);
    
    test_directed_graph_node_correctness();
    test_weight_function_correctness();
    
    c = clock();
    p_data = create_random_graph(NODES, EDGES, MAXX, MAXY, MAXZ);
    
    duration += ((double) clock() - c);
    printf("Built the graph in %f seconds.\n", duration / CLOCKS_PER_SEC);
    
    p_source = choose(p_data->p_node_array, NODES);
    p_target = choose(p_data->p_node_array, NODES);
    
    printf("Source: %s\n", directed_graph_node_t_to_string(p_source));
    printf("Target: %s\n", directed_graph_node_t_to_string(p_target));
    
    dijkstra(p_source, p_target, p_data->p_weight_function);
    
    return (EXIT_SUCCESS);
}

