#include "multifarious.h"
#include <cassert>
#include <iostream>
#include <numeric>

int main() {
    namespace mf = multifarious;

    {// member_pointer_t
        using namespace mf::type_traits;

        // Pointer to method (non-static member function) of `std::string` having the signature `void()`
        static_assert(std::is_same_v<
                      member_pointer_t<std::string, void()>,
                      void (std::string::*)()>);
    }// namespace ;

    {// array_t
        using namespace mf::type_traits;

        // Array of 5 pointers to functions taking a char* and returning a pointer to an unbounded array of char*
        [[maybe_unused]] char*(*(*foo_1[5])(char*) )[];
        [[maybe_unused]] array_t<std::add_pointer_t<std::add_pointer_t<array_t<char*>>(char*)>, 5> foo_2;
        static_assert(std::is_same_v<decltype(foo_1), decltype(foo_2)>);
    }

    {// bit_cast
        using namespace mf::util;

        float f = 3.0f;
        assert(bit_cast<int>(3.0f) == *(int*) (&f));
    }

    {// compose
        using namespace mf::functional;

        constexpr auto composed = compose([](int x) { return x * x; }, [](int x) { return x + 3; });
        constexpr auto manual = [](int x) { return x * x + 6 * x + 9; };

        for (int i = 0; i < 100; ++i) {
            assert(composed(i) == manual(i));// TODO: change to static_assert
        }
    }

    {// y_combinator
        using namespace mf::functional;

        [[maybe_unused]]// for some unknown reason clang-tidy complains that `fib` is unused
        constexpr auto fib = y_combinator([](auto&& fib, int x) {
            if (x < 2) {
                return 1;
            } else {
                return fib(fib, x - 1) + fib(fib, x - 2);
            }
        });

        static_assert(fib(15) == 987);
    }
}