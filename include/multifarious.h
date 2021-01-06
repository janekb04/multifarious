#ifndef MULTIFARIOUS_MULTIFARIOUS_H
#define MULTIFARIOUS_MULTIFARIOUS_H

#include <type_traits>
#include <new>
#include <memory>
#include <tuple>

// Shorthand macro for perfect forwarding
#define MULTIFARIOUS_FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
// Shorthand macro for capturing a perfect forwarded object in a lambda
// The object must be retrieved using `std::get<0>`
#define MULTIFARIOUS_FWD_CAPTURE(...) std::tuple{std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)}
// Just a fancy `auto` that carries meaning that it is specifically a lambda type
#define MULTIFARIOUS_LAMBDA_T auto

namespace multifarious {
    // Type utilities somewhat similar to standard type_traits
    namespace type_traits {
        // Type of a pointer to a member of type `Member` of class `Class`
        template<typename Class, typename Member>
        using member_pointer_t = Member Class::*;

        namespace detail {
            template<typename T, size_t... Dummy>
            struct array_t_helper {
            };

            template<typename T, size_t Size>
            struct array_t_helper<T, Size> {
                using type = T[Size];
            };

            template<typename T>
            struct array_t_helper<T> {
                using type = T[];
            };
        }

        // Type of an array of `T` of size `Size`, if it is provided, or an unbounded array of `T` if it isn't
        template<typename T, size_t... Size>
        using array_t = typename detail::array_t_helper<T, Size...>::type;

    }
    // Actually useful things
    namespace util {
        // Interprets the bit-pattern of an object as if it were an object of type `To`.
        // Not constexpr as reinterpret_cast is neither
        template<typename To, typename From>
        [[nodiscard]] decltype(auto) bit_cast(From&& val) noexcept {
            return *std::launder(reinterpret_cast<std::add_pointer_t<To>>(std::addressof(val)));
        }
    }
    // Things related to functional programming
    namespace functional {
        // Returns such a function `func` that `func(args) == f(g(args))`
        // An implementation of the mathematical Composition Operator
        template<typename Fn1, typename Fn2>
        constexpr MULTIFARIOUS_LAMBDA_T compose(Fn1&& f, Fn2&& g) noexcept {
            return [f{MULTIFARIOUS_FWD_CAPTURE(f)},
                    g{MULTIFARIOUS_FWD_CAPTURE(g)}]
                    (auto&& ... args) constexpr
                    noexcept(std::is_nothrow_invocable_v<Fn2, decltype(args)...> &&
                             std::is_nothrow_invocable_v<Fn1, std::invoke_result_t<Fn2, decltype(args)...>>)
                    -> std::invoke_result_t<Fn1, std::invoke_result_t<Fn2, decltype(args)...>> {
                return std::get<0>(f)(MULTIFARIOUS_FWD(std::get<0>(g)(MULTIFARIOUS_FWD(args)...)));
            };
        }

        // Returns such a function `func` that `func(args) == f(f, args)`
        // Useful for recursion in unnamed lambdas
        // An implementation of the Y-Combinator from untyped lambda calculus
        template<typename Fn>
        constexpr MULTIFARIOUS_LAMBDA_T y_combinator(Fn&& f) noexcept {
            return [f = MULTIFARIOUS_FWD_CAPTURE(f)]
                    (auto&& ... args) constexpr
                    noexcept(std::is_nothrow_invocable_v<Fn, Fn, decltype(args)...>)
                    -> std::invoke_result_t<Fn, Fn, decltype(args)...> {
                return std::get<0>(f)(std::get<0>(f), MULTIFARIOUS_FWD(args)...);
            };
        }
    }
}

// Macros pollute the global namespace, so here's a toggle if you don't want to use them
#ifndef MULTIFARIOUS_CONFIG_EXPORT_MACROS
#undef MULTIFARIOUS_FWD
#undef MULTIFARIOUS_FWD_CAPTURE
#undef MULTIFARIOUS_LAMBDA_T
#endif

#endif // MULTIFARIOUS_MULTIFARIOUS_H
