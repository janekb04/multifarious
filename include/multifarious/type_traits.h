#ifndef MULTIFARIOUS_TYPE_TRAITS_H
#define MULTIFARIOUS_TYPE_TRAITS_H

// Type utilities somewhat similar to standard type_traits
namespace multifarious::type_traits {
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
    }// namespace detail

    // Type of an array of `T` of size `Size`, if it is provided, or an unbounded array of `T` if it isn't
    template<typename T, size_t... Size>
    using array_t = typename detail::array_t_helper<T, Size...>::type;
}// namespace multifarious::type_traits

#endif//MULTIFARIOUS_TYPE_TRAITS_H
