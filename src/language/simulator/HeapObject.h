#pragma once
#include "RuntimeValue.h"
#include "../util/imports.h"

namespace Arc
{
    struct HeapObject
    {
        HeapObject(
            const int64_t vptr, const std::map<int64_t, RuntimeValue>& vtable, const size_t structId
        ) : vptr(vptr), vtable(vtable), structId(structId) {}

        int64_t vptr;
        std::map<int64_t, RuntimeValue> vtable;
        size_t structId;
    };
}