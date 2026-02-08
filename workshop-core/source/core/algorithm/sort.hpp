
#pragma once

#include <algorithm>

#if __has_include(<execution>) // Checking to see if the <execution> header is there
#include <execution>
#endif

namespace wk {
    template< class RandomIt >
    void sort_parallel( RandomIt first, RandomIt last ) {
        // Some compilers have not implemented execution policies
        #ifdef __cpp_lib_execution
            return std::sort(std::execution::par_unseq, first, last);
        #else
            return std::sort(first, last);
        #endif
    }

    template< class RandomIt, class Compare >
    void sort_parallel( RandomIt first, RandomIt last, Compare comp ) {
        // Some compilers have not implemented execution policies
        #ifdef __cpp_lib_execution
            return std::sort(std::execution::par_unseq, first, last, comp);
        #else
            return std::sort(first, last, comp);
        #endif
    }

    template< class RandomIt, class Compare >
    void stable_sort_parallel( RandomIt first, RandomIt last, Compare comp ) {
        // Some compilers have not implemented execution policies
        #ifdef __cpp_lib_execution
            return std::stable_sort(std::execution::par_unseq, first, last, comp);
        #else
            return std::stable_sort(first, last, comp);
        #endif
    }
}
