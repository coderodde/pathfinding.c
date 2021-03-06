#ifndef HEAP_H
#define	HEAP_H

#include <stdbool.h>
#include <stdlib.h>

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct heap_t heap_t;

    /***************************************************************************
    * Allocates a new, empty heap with given degree.                           *
    ***************************************************************************/  
    heap_t* heap_t_alloc(size_t degree,
                         size_t initial_capacity,
                         float load_factor,
                         size_t (*p_hash_function)(void*),
                         bool (*p_equals_function)(void*, void*),
                         int (*p_priority_compare_function)(void*, void*));

    /***************************************************************************
    * Adds a new element and its priority to the heap only if it is not        *
    * already present.                                                         *
    ***************************************************************************/  
    bool heap_t_add(heap_t* p_heap, void* p_element, void* p_priority);

    /***************************************************************************
    * Attempts to assign a higher priority to the element. Return true only    *       
    * if the structure of the heap changed due to this call.                   * 
    ***************************************************************************/  
    bool heap_t_decrease_key(heap_t* p_heap, void* p_element, void* p_priority);

    /***************************************************************************
    * Return true only if the element is in the heap.                          * 
    ***************************************************************************/  
    bool heap_t_contains_key(heap_t* p_heap, void* p_element);

    /***************************************************************************
    * Removes the highest priority element and returns it.                     * 
    ***************************************************************************/  
    void* heap_t_extract_min(heap_t* p_heap);

    /***************************************************************************
    * Returns the highest priority element without removing it.                * 
    ***************************************************************************/  
    void* heap_t_min(heap_t* p_heap);

    /***************************************************************************
    * Returns the size of this heap.                                           * 
    ***************************************************************************/  
    int heap_t_size(heap_t* p_heap);

    /***************************************************************************
    * Drops all the contents of the heap. Only internal structures are         *
    * deallocated; the user is responsible for memory-managing the contents.   * 
    ***************************************************************************/  
    void heap_t_clear(heap_t* p_heap);

    /***************************************************************************
    * Checks that the heap maintains the min-heap property.                    *
    ***************************************************************************/  
    bool heap_t_is_healthy(heap_t* p_heap);

    /***************************************************************************
    * Deallocates the entire heap with its internal structures. The client     *
    * programmer must, however, memory-manage the contents.                    * 
    ***************************************************************************/  
    void heap_t_free(heap_t* p_heap);

#ifdef	__cplusplus
}
#endif

#endif	/* HEAP_H */
