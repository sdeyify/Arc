#pragma once
#include "../util/imports.h"
#include "Lexer.h"
#include "Token.h"
#include "TokenType.h"
#include "TokenStream.h"
#include "nodes/Expression.h"
#include "nodes/Statement.h"
#include "nodes/ASTSwitchBlock.h"
#include "nodes/Type.h"
#include "nodes/Modifier.h"
#include "nodes/Decorator.h"
#include "nodes/ErrorDeclarationMember.h"
#include "../importer/Importer.h"
#include "ASTBuildingContext.h"
#include "AbstractSyntaxTree.h"

namespace Arc
{
    class ASTBuilder
    {
    private:
        std::vector<ASTNode::Modifier> collectedModifiers;
        std::vector<ASTNode::Decorator> decorators;
        std::vector<std::string> pathStack;
        std::vector<ASTNode::CircularReferenceGuard> circrefGuards;
    public:
        AbstractSyntaxTree parse(TokenStream& tokens, const std::string& path);
    private:
        std::vector<Statement> buildAst(TokenStream& tokens, const Arc::ASTBuildingContext& context = Arc::ASTBuildingContext());
        AbstractSyntaxTree toAst(TokenStream& tokens);

        std::vector<ASTNode::Modifier> flushModifiers();
        std::vector<ASTNode::Decorator> flushDecorators();

        ASTNode::Parameter parseParameter(TokenStream& tokenStream);
        ASTNode::Generic parseGeneric(TokenStream& tokenStream);
        ASTNode::Decorator parseDecorator(TokenStream& tokenStream);
        std::vector<ASTNode::SwitchBlock> parseSwitchBody(TokenStream& tokenStream);
        std::shared_ptr<ASTNode::NamespaceDeclaration> parseImport(const Arc::Package& tokenStream);
        std::string checkInlineConcatString(TokenStream tokenStream);
        ASTNode::StringInterpolation parseInterpolation(const std::string& src, const ASTNode::InterpolationKind&);
        std::optional<ASTNode::InterpolatedStringLiteral> parseInterpolatedString(const std::string& string);

        Type parseType(TokenStream& tokenStream);
        ASTNode::AnyType parseAnyType(TokenStream& tokenStream);
        ASTNode::BasicType parseBasicType(TokenStream& tokenStream);
        ASTNode::FunctionType parseFunctionType(TokenStream& tokenStream);
        ASTNode::MemberType parseMemberType(TokenStream& tokenStream);
        ASTNode::MergedType parseMergedType(TokenStream& tokenStream);
        ASTNode::NativeArrayType parseNativeArrayType(TokenStream& tokenStream);
        ASTNode::ObjectType parseObjectType(TokenStream& tokenStream);
        ASTNode::OptionalType parseOptionalType(TokenStream& tokenStream);
        ASTNode::ThrowType parseThrowType(TokenStream& tokenStream);
        ASTNode::TupleType parseTupleType(TokenStream& tokenStream);
        ASTNode::TypeofType parseTypeofType(TokenStream& tokenStream);
        ASTNode::UnionType parseUnionType(TokenStream& tokenStream);
        
        Literal parseLiteral(const Token& tokenStream);
        Expression parseExpression(TokenStream& tokenStream);
        ASTNode::ObjectLiteral parseObjectLiteral(TokenStream& tokenStream);
        ASTNode::AnonymousFunctionExpression parseShorthandLambda(TokenStream& tokenStream);
        ASTNode::CallExpression parseCallExpression(TokenStream& tokenStream);
        ASTNode::PipelineExpression parsePipelineExpression(TokenStream& tokenStream);
        ASTNode::ArrayAccessExpression parseArrayAccessExpression(TokenStream &tokenStream);
        ASTNode::ContractExpression parseContractExpression(TokenStream& tokenStream);
        Expression parseAnonymousDeclarations(TokenStream& tokenStream);
        ASTNode::AnonymousNamespaceExpression parseAnonymousNamespaceExpression(TokenStream& tokenStream);
        ASTNode::AnonymousNamespaceExpression parseImportExpression(TokenStream& tokenStream);
        ASTNode::ForeachExpression parseForeachExpression(TokenStream& tokenStream);
        ASTNode::WhileExpression parseWhileExpression(TokenStream& tokenStream);
        ASTNode::SwitchExpression parseSwitchExpression(TokenStream& tokenStream);
        ASTNode::IfExpression parseIfExpression(TokenStream& tokenStream);
        ASTNode::TryCatchExpression parseTryExpression(TokenStream& tokenStream);
        ASTNode::ForeverExpression parseForeverExpression(TokenStream& tokenStream);
        Expression parseSpecialSymbol(TokenStream& tokenStream);
        ASTNode::IteratorExpression parseIteratorExpression(TokenStream& tokenStream);
        ASTNode::CatchExpression parseCatchExpression(TokenStream& tokenStream);
        ASTNode::BinaryExpression parseBinaryExpression(TokenStream& tokenStream);
    
        Statement parseKeywordStatement(const Token& token, TokenStream& tokenStream);
        Statement parseExpressionStatement(const Token& token, TokenStream& tokenStream);
        ASTNode::NamespaceDeclaration parseNamespaceDeclaration(TokenStream& tokenStream);
        ASTNode::WhenStatement parseWhenStatement(TokenStream& tokenStream);
        ASTNode::WhileStatement parseWhileStatement(TokenStream& tokenStream);
        ASTNode::IfStatement parseIfStatement(TokenStream& tokenStream);
        ASTNode::ForeverStatement parseForeverStatement(TokenStream& tokenStream);
        ASTNode::ForeachStatement parseForeachStatement(TokenStream& tokenStream);
        ASTNode::SwitchStatement parseSwitchStatement(TokenStream& tokenStream);
        ASTNode::ExtensionDeclaration parseExtensionDeclaration(TokenStream& tokenStream);
        ASTNode::ClasslikeDeclaration parseClasslikeDeclaration(TokenStream& tokenStream, const bool expression, const bool parsingEnum);
        ASTNode::ClassDeclaration parseClassDeclaration(TokenStream& tokenStream);
        ASTNode::StructDeclaration parseStructDeclaration(TokenStream& tokenStream);
        ASTNode::MacroDeclaration parseMacroDeclaration(TokenStream& tokenStream);
        ASTNode::WhereStatement parseWhereStatement(TokenStream& tokenStream);
        ASTNode::InterfaceDeclaration parseInterfaceDeclaration(TokenStream& tokenStream);
        ASTNode::VariableDeclaration parseVariableDeclaration(const bool constant, TokenStream& tokenStream);
        ASTNode::TypeDeclaration parseTypeDeclaration(TokenStream& tokenStream);
        ASTNode::EnumDeclaration parseEnumDeclaration(TokenStream& tokenStream);
        ASTNode::EnumMember parseEnumMember(TokenStream& tokenStream);
        ASTNode::CatchHandleStatement parseCatchHandleStatement(TokenStream& tokenStream);
        ASTNode::FunctionDeclaration parseFunctionDeclaration(TokenStream& tokenStream, const bool expression);
        ASTNode::FunctorDeclaration parseFunctorDeclaration(TokenStream& tokenStream);
        ASTNode::ContractDeclaration parseContractDeclaration(TokenStream& tokenStream);
        ASTNode::WithStatement parseWithStatement(TokenStream& tokenStream);
        ASTNode::ErrorDeclaration parseErrorDeclaration(TokenStream& tokenStream);
        ASTNode::ErrorDeclarationMember parseErrorDeclarationMember(TokenStream& tokenStream);
    };
}