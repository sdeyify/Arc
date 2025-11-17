#pragma once
#include "../util/Util.h"

namespace Arc
{
    enum class IRBitsizeKind
    {
        NativeI1,
        NativeI8,
        NativeI16,
        NativeI32,
        NativeI64,
        NativeU8,
        NativeU16,
        NativeU32,
        NativeU64,
        NativeF16,
        NativeBF16,
        NativeF32,
        NativeF64,
    };

    inline byte numericBitsize(const Arc::IRBitsizeKind& bitsize) {
        switch (bitsize) {
            case Arc::IRBitsizeKind::NativeI1: return 1;
            case Arc::IRBitsizeKind::NativeI8: return 8;
            case Arc::IRBitsizeKind::NativeI16: return 16;
            case Arc::IRBitsizeKind::NativeI32: return 32;
            case Arc::IRBitsizeKind::NativeI64: return 64;
            case Arc::IRBitsizeKind::NativeU8: return 8;
            case Arc::IRBitsizeKind::NativeU16: return 16;
            case Arc::IRBitsizeKind::NativeU32: return 32;
            case Arc::IRBitsizeKind::NativeU64: return 64;
            case Arc::IRBitsizeKind::NativeF16: return 16;
            case Arc::IRBitsizeKind::NativeBF16: return 16;
            case Arc::IRBitsizeKind::NativeF32: return 32;
            case Arc::IRBitsizeKind::NativeF64: return 64;
        }
    }

    inline std::string stringifyBitsize(const Arc::IRBitsizeKind& bitsize) {
        switch (bitsize) {
            case Arc::IRBitsizeKind::NativeI1: return "Int1";
            case Arc::IRBitsizeKind::NativeI8: return "Int8";
            case Arc::IRBitsizeKind::NativeI16: return "Int16";
            case Arc::IRBitsizeKind::NativeI32: return "Int32";
            case Arc::IRBitsizeKind::NativeI64: return "Int64";
            case Arc::IRBitsizeKind::NativeU8: return "Uint8";
            case Arc::IRBitsizeKind::NativeU16: return "Uint16";
            case Arc::IRBitsizeKind::NativeU32: return "Uint32";
            case Arc::IRBitsizeKind::NativeU64: return "Uint64";
            case Arc::IRBitsizeKind::NativeF16: return "Float16";
            case Arc::IRBitsizeKind::NativeBF16: return "BrainFloat16";
            case Arc::IRBitsizeKind::NativeF32: return "Float32";
            case Arc::IRBitsizeKind::NativeF64: return "Float64";
        }
    }
}