#ifndef MULTIFARIOUS_MACROS_H
#define MULTIFARIOUS_MACROS_H

// Shorthand macro for perfect forwarding
#define MULTIFARIOUS_FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
// Shorthand macro for capturing a perfect forwarded object in a lambda
// The object must be retrieved using `std::get<0>`
#define MULTIFARIOUS_FWD_CAPTURE(...) \
    std::tuple { std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__) }
// Just a fancy `auto` that carries meaning that it is specifically a lambda type
#define MULTIFARIOUS_LAMBDA_T auto

#endif//MULTIFARIOUS_MACROS_H
