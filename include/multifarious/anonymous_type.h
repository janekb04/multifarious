#ifndef MULTIFARIOUS_ANONYMOUS_TYPE_H
#define MULTIFARIOUS_ANONYMOUS_TYPE_H

#include <tuple>
#include <type_traits>

// Customization:
//  MULTIFARIOUS_ANONYMOUS_TYPE_USE_WRAPPER
//  MULTIFARIOUS_ANONYMOUS_TYPE_DEFINE_ABBREVIATIONS

namespace multifarious::anonymous_type::detail {
    // Returns a sort of "instantiation" of a generic lambda
    // Actually is similar to `std::bind` except that it "binds" template arguments
    // rather then regular function arguments to a lambda's `operator()`
    template<typename... Ts, typename Lambda>
    static constexpr auto get_lambda_instatiation(Lambda lambda) {
        return [&](auto&&... args) {
            auto member = &Lambda::template operator()<Ts...>;
            return (lambda.*member)(std::forward<decltype(args)>(args)...);
        };
    }

    // The dummy type used by lambda_holder::make_lambda_template_parameter_tuple
    // SHALL NOT be used under threat of a nasal demon invasion
    struct tag {};

    // Type trait that gives the type of a concatenation of two tuples
    template<typename Tuple1, typename Tuple2>
    struct tuple_type_cat {
    };
    template<typename... Ts1, typename... Ts2>
    struct tuple_type_cat<std::tuple<Ts1...>, std::tuple<Ts2...>> {
        using type = std::tuple<Ts1..., Ts2...>;
    };
    template<typename Tuple1, typename Tuple2>
    using tuple_type_cat_t = typename tuple_type_cat<Tuple1, Tuple2>::type;

    // Index sequence that allows to unpack `Tuple`. Shorthand.
    template<typename Tuple>
    using tuple_indices = std::make_index_sequence<std::tuple_size_v<Tuple>>;

    template<auto Val>
    class lambda_holder {
    private:
        using Lambda = decltype(Val);

        // Returns the type given to `type_`. Used in the extraction of it
        // Has to be a function as alias templates can't really unpack tuple
        // using an index sequence (because it couldn't be deduced)
        template<typename Tuple, size_t... I>
        static constexpr std::remove_pointer_t<std::invoke_result_t<decltype(get_lambda_instatiation<std::tuple_element_t<I, Tuple>...>(std::declval<Lambda>()))>>
        return_type_extractor(Tuple, std::index_sequence<I...>) noexcept {
        }

        // Extracted from `type` as it would be repetitive otherwise
        template<typename Tuple>
        using extractor_tuple_provider = decltype(return_type_extractor(
            std::declval<Tuple>(),
            std::declval<tuple_indices<Tuple>>()));

        // The lambda inside `type_` has a first dummmy template argument as
        // "the template parameter list cannot be empty" in a generic lambda
        // If the user provides their own template arguments, they must skip this first dummy parameter
        // To do this a special `tag` type is used - the user SHALL NOT use it
        template<typename T, typename... Ts>
        using make_lambda_template_parameter_tuple = tuple_type_cat_t<
            std::conditional_t<std::is_same_v<T, tag>, std::tuple<tag>, std::tuple<tag, T>>,
            std::tuple<Ts...>>;

    public:
        // This alias templates stores the anonymous type
        // It is the exact same type given to the `type_` macro
        // Used if ANONYMOUS_TYPE_USE_WRAPPER is not defined
        template<typename T = tag, typename... Ts>
        using type = extractor_tuple_provider<
            make_lambda_template_parameter_tuple<T, Ts...>>;

        // See ANONYMOUS_TYPE_USE_WRAPPER
        template<typename T = tag, typename... Ts>
        struct wrapper : public type<T, Ts...> {
            template<typename... Args>
            constexpr wrapper(Args&&... args) noexcept(std::is_nothrow_constructible_v<type<T, Ts...>, Args...>)
                : type<T, Ts...>{std::forward<Args>(args)...} {}
        };
    };
}// namespace multifarious::anonymous_type::detail

// Some compilers don't support alias template argument deduction
// This makes it impossible to use `type_(/* empty */)(whatever)` without trailing `<>`
// even though there are no template parameters provided
// This macro definition makes it possible (at least for some compilers)
// This type is not the same as the one given to the `type_` macro but is should be equivalent
// Problems should arise only if the type given to `type_` is marked as `final`
// To use, define the configuration macro MULTIFARIOUS_ANONYMOUS_TYPE_USE_WRAPPER
#ifdef MULTIFARIOUS_ANONYMOUS_TYPE_USE_WRAPPER
#define _MULTIFARIOUS_ANONYMOUS_TYPE_MEMBER wrapper
#else
#define _MULTIFARIOUS_ANONYMOUS_TYPE_MEMBER type
#endif

// In order to be able to `type_` as if it was returning a closure (so multiple parenthesis can be chained),
// it has to be split into two macros, this one being the second one
#define _MULTIFARIOUS_ANONYMOUS_TYPE_IMPL(...) \
    __VA_ARGS__* _;                            \
    return static_cast<decltype(_)>(nullptr);  \
    }                                          \
    > ::_MULTIFARIOUS_ANONYMOUS_TYPE_MEMBER

// The most important thing
// Usage: ANONYMOUS_TYPE(tparams)(type)
//      tparams     - template parameters of the type. Empty if the type is not to be a template
//      type        - the anonymous type itself. Must include the appropriate
//                    type keyword (`class`, `struct`, `enum` or `union`)
//                    as well as brackets. The type MUST NOT be followed by a semicolon.
#define MULTIFARIOUS_ANONYMOUS_TYPE(...) ::multifarious::anonymous_type::detail::lambda_holder < []<typename __VA_OPT__(, ) __VA_ARGS__>() { _MULTIFARIOUS_ANONYMOUS_TYPE_IMPL

// It can be tedious to write out MULTIFARIOUS_ANONYMOUS_TYPE(tparams)(keyword { body }) every time
// As such abbreviations can be provided
// They are not provided by default, as they are macros and pollute the global namespace
#ifdef MULTIFARIOUS_ANONYMOUS_TYPE_DEFINE_ABBREVIATIONS
#define type_ MULTIFARIOUS_ANONYMOUS_TYPE
#define struct_(...) type_()(struct {__VA_ARGS__})<>
#define class_(...) type_()(class {__VA_ARGS__})<>
#define enum_(...) type_()(enum {__VA_ARGS__})<>
#define union_(...) type_()(union {__VA_ARGS__})<>
#endif

#endif//MULTIFARIOUS_ANONYMOUS_TYPE_H
