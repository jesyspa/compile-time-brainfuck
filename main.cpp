#include "ctbf.hpp"
#include <iostream>

template<typename T>
struct print {
    static void invoke() {
        print <typename T::tail>::invoke();
        std::cout << T::value << '\n';
    }
};

template<>
struct print<tail> {
    static void invoke() {}
};

int main() {
    typedef flat_bf<inc,
            flat_bf<lps,
            flat_bf<inc,
            flat_bf<mvr,
            flat_bf<dec,
            flat_bf<out,
            flat_bf<mvl,
            flat_bf<out,
            flat_bf<lpe, flat_bf_tail> > > > > > > > > flat_code;
    typedef typename nest<flat_code>::type nested_code;
    typedef execute<nested_code> result;
    print<typename result::output>::invoke();
}
