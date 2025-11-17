#pragma once
#include "imports.h"

namespace Util
{
    template<typename T, typename U = T>
    T join(std::vector<T> vec, U seperator) {
        T joined;

        for (const T& item : vec) {
            joined += item;
            joined += seperator;
        }

        return joined;
    }

    template<typename T>
    std::vector<T> joinvec(std::vector<T> vec1, std::vector<T> vec2) {
        vec1.insert(vec1.end(), vec2.cbegin(), vec2.cend());

        return vec1;
    }
}