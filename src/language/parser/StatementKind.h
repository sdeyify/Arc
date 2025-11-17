#pragma once

namespace Arc
{
    enum class StatementKind
    {
        AssignmentStatement,
        BreakStatement,
        CallStatement,
        CatchHandleStatement,
        ClassDeclaration,
        ClasslikeDeclaration,
        ContinueStatement,
        ContractDeclaration,
        EnumDeclaration,
        EnumMember,
        ErrorDeclaration,
        ExpressionStatement,
        ExtensionDeclaration,
        ForeachStatement,
        ForeverStatement,
        FunctionDeclaration,
        FunctorDeclaration,
        GotoStatement,
        IfStatement,
        InterfaceDeclaration,
        LabelStatement,
        MacroDeclaration,
        ModifierApplyStatement,
        NamespaceDeclaration,
        ReturnStatement,
        StructDeclaration,
        SwitchStatement,
        TypeDeclaration,
        VariableDeclaration,
        WhenStatement,
        WhereStatement,
        WhileStatement,
        WithStatement
    };
}