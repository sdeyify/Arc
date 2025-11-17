#pragma once
#include "../util/imports.h"
#include "IRBitsizeKind.h"

namespace Arc
{
    const inline std::vector<std::string> modifiers = {
        "async", "global", "export", "stepped", "operator", "public", "private", "protected",  "abstract", "ref",
        "operator", "static", "strict", "preload", "do", "decorator", "native", "default", "get", "data", "pure",
        "set", "init", "override", "sealed", "unsealed", "circrefdecl", "functorlike", "constfunc", "opaque",
        "transparent"
    };

    inline std::map<std::string, Arc::IRBitsizeKind> nativeValues = {
        {"NativeI1", Arc::IRBitsizeKind::NativeI1},
        {"NativeI8", Arc::IRBitsizeKind::NativeI8},
        {"NativeI16", Arc::IRBitsizeKind::NativeI16},
        {"NativeI32", Arc::IRBitsizeKind::NativeI32},
        {"NativeI64", Arc::IRBitsizeKind::NativeI64},
        {"NativeU8", Arc::IRBitsizeKind::NativeU8},
        {"NativeU16", Arc::IRBitsizeKind::NativeU16},
        {"NativeU32", Arc::IRBitsizeKind::NativeU32},
        {"NativeU64", Arc::IRBitsizeKind::NativeU64},
        {"NativeF16", Arc::IRBitsizeKind::NativeF16},
        {"NativeBF16", Arc::IRBitsizeKind::NativeBF16},
        {"NativeF32", Arc::IRBitsizeKind::NativeF32},
        {"NativeF64", Arc::IRBitsizeKind::NativeF64},
    };

    inline std::map<std::string, bool> customSizedNativeValues = {
        {"NativeISZ", true},
        {"NativeUSZ", false}
    };
}