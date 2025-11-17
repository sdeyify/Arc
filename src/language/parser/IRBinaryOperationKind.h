#pragma once

namespace Arc
{
    enum class IRBinaryOperationKind
    {
        Equal,
        NotEqual,
        Modulus,
        Multiply,
        Add,
        Subtract,
        Divide,
        LessThan,
        LessOrEqual,
        GreaterThan,
        GreaterOrEqual,
        LeftShift,
        RightShift,
        BitwiseAnd,
        LogicalAnd,
        BitwiseOr,
        LogicalOr,
        BitwiseXOR,
        InstanceOf,
        LeftRotate,
        RightRotate,
        SquareRoot,
        Power,
        Maximum,
        Minimum
    };
}