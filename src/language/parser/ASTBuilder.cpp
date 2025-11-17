#include <stdexcept>
#include "../util/Util.h"
#include "Parser.h"

Arc::AbstractSyntaxTree Arc::ASTBuilder::parse(Arc::TokenStream& tokenStream, const std::string& path) {
    return this->buildAst(tokenStream, Arc::ASTBuildingContext {
        .parsingNewPath = std::make_optional(path)
    });
}

Arc::AbstractSyntaxTree Arc::ASTBuilder::toAst(Arc::TokenStream& tokenStream) {
    return this->buildAst(tokenStream);
}

std::vector<Arc::Statement> Arc::ASTBuilder::buildAst(Arc::TokenStream& tokenStream, const Arc::ASTBuildingContext& context) {
    if (context.parsingNewPath != std::nullopt) {
        this->pathStack.push_back(*context.parsingNewPath);
    }

    std::vector<Arc::Statement> ast;
    while (!tokenStream.empty() && tokenStream.at(0).kind != Arc::TokenType::Eof) {
        const Arc::Token& token = tokenStream.next();

        if (token.kind == Arc::TokenType::Keyword) {
            ast.push_back(this->parseKeywordStatement(token, tokenStream));
        } else if (token.kind == Arc::TokenType::AtSign) {
            this->decorators.push_back(this->parseDecorator(tokenStream));
        } else if (token.kind == Arc::TokenType::Identifier && context.parsingEnum) {
            ast.push_back(
                std::make_shared<Arc::ASTNode::EnumMember>(this->parseEnumMember(tokenStream))
            );
        } else if (token.kind == Arc::TokenType::StartBrace && this->collectedModifiers.size() > 0) {
            Arc::TokenStream bodyTokens = tokenStream.getBody(token);
            ast.push_back(std::make_shared<Arc::ASTNode::ModifierApplyStatement>(
                this->flushModifiers(),
                this->buildAst(bodyTokens)
            ));
        } else if (token.kind == Arc::TokenType::Semicolon) {
            continue;
        } else if (token.kind == Arc::TokenType::Hash) {
            // handle macros
        } else {
            ast.push_back(this->parseExpressionStatement(token, tokenStream));
        }
    }

    if (context.parsingNewPath != std::nullopt) {
        this->pathStack.pop_back();
    }

    return ast;
}

std::vector<Arc::ASTNode::Modifier> Arc::ASTBuilder::flushModifiers() {
    std::vector<Arc::ASTNode::Modifier> modifiers = this->collectedModifiers;
    this->collectedModifiers.clear();
    return modifiers;
}

std::vector<Arc::ASTNode::Decorator> Arc::ASTBuilder::flushDecorators() {
    std::vector<Arc::ASTNode::Decorator> decorators = this->decorators;
    this->decorators.clear();
    return decorators;
}