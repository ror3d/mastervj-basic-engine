#pragma once

#include <tuple>
#include "Selector.h"

namespace sel {
template <typename... T>
class Tuple {
private:
    std::tuple<T&...> _tuple;
public:
    Tuple(T&... args) : _tuple(args...) {}

    void operator=(const sel::Selector &s) {
        _tuple = s.GetTuple<typename std::remove_reference<T>::type...>();
    }

    void operator=(const std::tuple<typename std::remove_reference<T>::type...> &t) {
        _tuple = t;
    }
};

template <typename... T>
Tuple<T&...> tie(T&... args) {
    return Tuple<T&...>(args...);
}
}
