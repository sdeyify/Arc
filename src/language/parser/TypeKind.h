#pragma once

namespace Arc
{
    enum class TypeKind
    {
        BasicType,
        AnyType,
        TypeofType,
        UnionType,
        MergedType,
        ObjectType,
        TupleType,
        FunctionType,
        MemberType,
        ThrowType,
        OptionalType,
        UnitType,
        InferType,
        NativeFunctionType,
        NativeValueType,
        NativeArray
    };
}