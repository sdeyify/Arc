#pragma once
#include "imports.h"

namespace Util
{
    template<typename T> inline bool includes(std::vector<T> collection, T value) {
        return std::find(std::begin(collection), std::end(collection), value) != std::end(collection);
    }

    template<typename K, typename V> inline bool includes(std::map<K, V> collection, K value) {
        return collection.find(value) != collection.end();
    }

    template<typename K, typename V> inline bool includes(std::unordered_map<K, V> collection, K value) {
        return collection.find(value) != collection.end();
    }
}