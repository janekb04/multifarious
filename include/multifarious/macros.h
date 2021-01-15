#ifndef MULTIFARIOUS_MACROS_H
#define MULTIFARIOUS_MACROS_H

#include <tuple>
#include <type_traits>

// Shorthand macro for perfect forwarding
#define MULTIFARIOUS_FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
// Shorthand macro for capturing a perfect forwarded object in a lambda
// The object must be retrieved using `std::get<0>`
#define MULTIFARIOUS_FWD_CAPTURE(...) \
    std::tuple { std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__) }
// Just a fancy `auto` that carries meaning that it is specifically a lambda type
#define MULTIFARIOUS_LAMBDA_T auto
// Forces the given expression to be executed at compile time or cause a compilation error
#define MULTIFARIOUS_FORCE_CONSTANT_EVALUATION(...) \
    [&]() constexpr {                               \
        constexpr bool guard = [&]() constexpr {    \
            __VA_ARGS__                             \
            return false;                           \
        }                                           \
        ();                                         \
    }                                               \
    ()

#endif//MULTIFARIOUS_MACROS_H
