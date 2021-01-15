#ifndef MULTIFARIOUS_FUNCTIONAL_H
#define MULTIFARIOUS_FUNCTIONAL_H

#include <type_traits>

#include "multifarious/macros.h"

// Things related to functional programming
namespace multifarious::functional {
    // Returns such a function `func` that `func(args) == f(g(args))`
    // An implementation of the mathematical Composition Operator
    template<typename Fn1, typename Fn2>
    constexpr MULTIFARIOUS_LAMBDA_T compose(Fn1&& f, Fn2&& g) noexcept {
        return [f{MULTIFARIOUS_FWD_CAPTURE(f)},
                g{MULTIFARIOUS_FWD_CAPTURE(g)}](auto&&... args) constexpr noexcept(std::is_nothrow_invocable_v<Fn2, decltype(args)...> &&
                                                                                   std::is_nothrow_invocable_v<Fn1, std::invoke_result_t<Fn2, decltype(args)...>>)
            ->std::invoke_result_t<Fn1, std::invoke_result_t<Fn2, decltype(args)...>> {
            return std::get<0>(f)(MULTIFARIOUS_FWD(std::get<0>(g)(MULTIFARIOUS_FWD(args)...)));
        };
    }

    // Returns such a function `func` that `func(args) == f(f, args)`
    // Useful for recursion in unnamed lambdas
    // An implementation of the Y-Combinator from untyped lambda calculus
    template<typename Fn>
    constexpr MULTIFARIOUS_LAMBDA_T y_combinator(Fn&& f) noexcept {
        return [f = MULTIFARIOUS_FWD_CAPTURE(f)](auto&&... args) constexpr noexcept(std::is_nothrow_invocable_v<Fn, Fn, decltype(args)...>)
            ->std::invoke_result_t<Fn, Fn, decltype(args)...> {
            return std::get<0>(f)(std::get<0>(f), MULTIFARIOUS_FWD(args)...);
        };
    }
}

#endif//MULTIFARIOUS_FUNCTIONAL_H
