#include <stdio.h>
#include <stdlib.h>
#include "directed_graph_node.h"

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
    
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_a) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_b) == false);
    
    ASSERT(directed_graph_node_t_add_arc(p_node_a, p_node_a));
    
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_a));
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_b) == false);
    
    ASSERT(directed_graph_node_t_add_arc(p_node_b, p_node_b));
    
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_a));
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_b));
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_b) == false);
    
    ASSERT(directed_graph_node_t_add_arc(p_node_b, p_node_d));
    directed_graph_node_t_clear(p_node_a);
    
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_a) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_b) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_c) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_a, p_node_d) == false);
    ASSERT(directed_graph_node_t_has_child(p_node_b, p_node_d));
    
    puts("MEGA");
}

int main(int argc, char** argv) {
    test_directed_graph_node_correctness();
    
    return (EXIT_SUCCESS);
}

