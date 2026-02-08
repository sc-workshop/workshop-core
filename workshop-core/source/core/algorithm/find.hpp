#pragma once

#include <vector>

#if __has_include(<execution>) //checking to see if the <execution> header is there
#include <execution>
#endif

namespace wk {
    template<class InputIt, class T = typename std::iterator_traits<InputIt>::value_type>
    constexpr InputIt  find_parallel(InputIt begin, InputIt end, const T& value) {
    
    // Some compilers have not implemented 	execution policies
    #ifdef __cpp_lib_execution
        return std::find(std::execution::par_unseq, begin, end, value);
    #else
        return std::find(begin, end, value);
    #endif
    }

    template<class InputIt, class UnaryPred>
    constexpr InputIt find_if_parallel(InputIt begin, InputIt end, UnaryPred p) {

    // Some compilers have not implemented     execution policies
    #ifdef __cpp_lib_execution
        return std::find_if(std::execution::par_unseq, begin, end, p);
    #else
        return std::find_if(begin, end, p);
    #endif
    }
}
