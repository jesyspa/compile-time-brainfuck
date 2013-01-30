#include "ctbf.hpp"

// Inspired by this answer: http://stackoverflow.com/a/14589567/559931
// Requires a drop of C++11 to be impressive.

template<typename T>
struct is_tail {
    enum { value = false };
};

template<>
struct is_tail<tail> {
    enum { value = true };
};

template<bool b>
struct fuck_you {
    enum { magic = 5 };
};

template<>
struct fuck_you<true> {
    template<int>
    using magic = int;
};

int main() {
    typedef flat_bf<out, flat_bf_tail> flat_code;
    typedef typename nest<flat_code>::type nested_code;
    typedef typename execute<nested_code>::output result;
    fuck_you<is_tail<result>::value>::magic<3>(0);
}
