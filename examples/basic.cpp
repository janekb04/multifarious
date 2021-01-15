#define MULTIFARIOUS_ANONYMOUS_TYPE_DEFINE_ABBREVIATIONS
#include "multifarious.h"
#include <array>
#include <cassert>
#include <climits>
#include <functional>
#include <iostream>
#include <vector>

namespace mf = multifarious;

int main() {
    {// member_pointer_t
        using namespace mf::type_traits;

        // Pointer to method (non-static member function) of `std::string` having the signature `void()`
        static_assert(std::is_same_v<
                      member_pointer_t<std::string, void()>,
                      void (std::string::*)()>);
    }

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

        MULTIFARIOUS_FORCE_CONSTANT_EVALUATION(
            for (int i = 0; i < 100; ++i) {
                assert(composed(i) == manual(i));
            });
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

    // clang-format off
    {// anonymous_type
        struct derived : public struct_(int x, y;) {
            constexpr int sum() {
                return x + y;
            }
        };

        constexpr int s = derived{2, 3}.sum();

        //Not yet implemented in any known compiler
        //
        //vector<enum_{RED, GREEN, BLUE}> colors;
        //constexpr auto lambda = [](struct_(std::string name; int age;) person) -> struct_(std::string fortune; int luckyNumber; char happyLetter;) {
        //  int luck = person.age * person.name.size() - (person.age ^ person.age * 13);
        //  return { "May the luck be with you.", luck, person.name[luck % person.name.size()] };
        //};
        //
        //constexpr int theMagicConstant = lambda("John Doe", 42).luckyNumber;
    }// clang-format on
}