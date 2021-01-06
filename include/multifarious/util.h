#ifndef MULTIFARIOUS_UTIL_H
#define MULTIFARIOUS_UTIL_H

// Actually useful things
namespace multifarious::util {
    // Interprets the bit-pattern of an object as if it were an object of type `To`.
    // Not constexpr as reinterpret_cast is neither
    template<typename To, typename From>
    [[nodiscard]] decltype(auto) bit_cast(From&& val) noexcept {
        return *std::launder(reinterpret_cast<std::add_pointer_t<To>>(std::addressof(val)));
    }
}

#endif//MULTIFARIOUS_UTIL_H
