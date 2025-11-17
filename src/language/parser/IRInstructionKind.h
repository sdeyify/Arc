#pragma once

namespace Arc
{
    enum class IRInstructionKind
    {
        ArrayLength,
        BinaryOperation,
        Bitcast,
        BranchIf,
        Breakpoint,
        Bubble,
        Call,
        CallIndirect,
        DebugWrite,
        DeleteObject,
        Exit,
        FreePointer,
        FunctionPointerOf,
        LoadArray,
        Label,
        LoadDirectivePointer,
        LoadField,
        LoadLocal,
        LoadNull,
        LoadPointer,
        LoadValue,
        LoadVoid,
        NewArray,
        NewObject,
        ObjectCast,
        Pointer,
        Pop,
        Return,
        StoreArray,
        StoreField,
        StoreLocal,
        StorePointer,
        Switch,
        UnaryOperation
    };
}