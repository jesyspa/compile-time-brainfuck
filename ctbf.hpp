#pragma once

struct inc {};
struct dec {};
struct mvr {};
struct mvl {};
struct out {};
struct lps {};
struct lpe {};
struct loop {};
struct done {};

template<unsigned VALUE, typename TAIL>
struct data_link {
    enum { value = VALUE };
    typedef TAIL tail;
};

template<typename LHS, unsigned VALUE, typename RHS>
struct data_current {
    typedef LHS lhs;
    enum { value = VALUE };
    typedef RHS rhs;
};

struct tail {
    typedef tail lhs;
    enum { value = 0 };
    typedef tail rhs;
};

template<typename HEAD, typename TAIL>
struct flat_bf {
    typedef HEAD head;
    typedef TAIL tail;
};

struct flat_bf_tail {
    typedef done head;
    typedef flat_bf_tail tail;
};

template<typename CMD, typename REST>
struct nested_bf {
    typedef nested_bf<typename REST::head, typename REST::tail> rest;
    typedef typename rest::up up;
};

template<typename REST>
struct nested_bf<lps, REST> {
    typedef nested_bf<typename REST::head, typename REST::tail> code;
    typedef nested_bf<typename code::up::head, typename code::up::tail> rest;
    typedef typename rest::up up;
};

template<typename REST>
struct nested_bf<lpe, REST> {
    typedef REST up;
    // Not allowed to use rest when matched lpe
};

template<>
struct nested_bf<done, flat_bf_tail> {
    typedef flat_bf_tail up;
};

template<typename T>
struct nest {
    typedef nested_bf<typename T::head, typename T::tail> type;
};

template<typename CODE, typename DATA, typename OUTPUT>
struct executed_bf;

template<typename CREST, typename DATA, typename OUTPUT>
struct executed_bf<nested_bf<inc, CREST>, DATA, OUTPUT> {
    typedef data_current<typename DATA::lhs, (DATA::value+1)&0xFF, typename DATA::rhs> new_data;
    typedef executed_bf<typename nested_bf<inc, CREST>::rest, new_data, OUTPUT> _next;
    typedef typename _next::data data;
    typedef typename _next::output output;
};

template<typename CREST, typename DATA, typename OUTPUT>
struct executed_bf<nested_bf<dec, CREST>, DATA, OUTPUT> {
    typedef data_current<typename DATA::lhs, (DATA::value-1)&0xFF, typename DATA::rhs> new_data;
    typedef executed_bf<typename nested_bf<dec, CREST>::rest, new_data, OUTPUT> _next;
    typedef typename _next::data data;
    typedef typename _next::output output;
};

template<typename CREST, typename DATA, typename OUTPUT>
struct executed_bf<nested_bf<mvl, CREST>, DATA, OUTPUT> {
    typedef data_current<typename DATA::lhs::tail, DATA::lhs::value, data_link<DATA::value, typename DATA::rhs> > new_data;
    typedef executed_bf<typename nested_bf<mvl, CREST>::rest, new_data, OUTPUT> _next;
    typedef typename _next::data data;
    typedef typename _next::output output;
};

template<typename CREST, typename DATA, typename OUTPUT>
struct executed_bf<nested_bf<mvr, CREST>, DATA, OUTPUT> {
    typedef data_current<data_link<DATA::value, typename DATA::lhs>, DATA::rhs::value, typename DATA::rhs::tail> new_data;
    typedef executed_bf<typename nested_bf<mvr, CREST>::rest, new_data, OUTPUT> _next;
    typedef typename _next::data data;
    typedef typename _next::output output;
};

template<typename CREST, typename DATA, typename OUTPUT>
struct executed_bf<nested_bf<out, CREST>, DATA, OUTPUT> {
    typedef data_link<DATA::value, OUTPUT> new_output;
    typedef executed_bf<typename nested_bf<out, CREST>::rest, DATA, new_output> _next;
    typedef typename _next::data data;
    typedef typename _next::output output;
};

template<typename CREST, typename DATA, typename OUTPUT>
struct executed_bf<nested_bf<lps, CREST>, DATA, OUTPUT> {
    typedef executed_bf<typename nested_bf<lps, CREST>::code, DATA, OUTPUT> _body;
    typedef executed_bf<nested_bf<lps, CREST>, typename _body::data, typename _body::output> _next;
    typedef typename _next::data data;
    typedef typename _next::output output;
};

template<typename CREST, typename DATA_LHS, typename DATA_RHS, typename OUTPUT>
struct executed_bf<nested_bf<lps, CREST>, data_current<DATA_LHS, 0, DATA_RHS>, OUTPUT> {
    typedef executed_bf<typename nested_bf<lps, CREST>::rest, data_current<DATA_LHS, 0, DATA_RHS>, OUTPUT> _next;
    typedef typename _next::data data;
    typedef typename _next::output output;
};

template<typename CREST, typename DATA, typename OUTPUT>
struct executed_bf<nested_bf<lpe, CREST>, DATA, OUTPUT> {
    typedef DATA data;
    typedef OUTPUT output;
};

template<typename DATA, typename OUTPUT>
struct executed_bf<nested_bf<done, flat_bf_tail>, DATA, OUTPUT> {
    typedef DATA data;
    typedef OUTPUT output;
};

template<typename T>
struct execute {
    typedef executed_bf<T, data_current<tail, 0, tail>, tail> _executed;
    typedef typename _executed::data data;
    typedef typename _executed::output output;
};


