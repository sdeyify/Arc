#include "Parser.h"
#include "../util/Util.h"
#include <stdexcept>

ASTNode::StringInterpolation Arc::ASTBuilder::parseInterpolation(const std::string& src, const ASTNode::InterpolationKind& interpolationParsingKind) {
    if (interpolationParsingKind == Arc::ASTNode::InterpolationKind::SourceInterpolation) {
        return std::make_shared<Arc::ASTNode::SourceInterpolation> (
            Arc::ASTNode::StringLiteral(src)
        );
    } else if (interpolationParsingKind == Arc::ASTNode::InterpolationKind::ExpressionInterpolation) {
        Lexer lexer(src);
        Arc::TokenStream tokenStream = lexer.parse();
        return std::make_shared<Arc::ASTNode::ExpressionInterpolation> (
            this->parseExpression(tokenStream)
        );
    }

    std::unreachable();
}

std::optional<Arc::ASTNode::InterpolatedStringLiteral> Arc::ASTBuilder::parseInterpolatedString(const std::string& string) {
    std::vector<Arc::ASTNode::StringInterpolation> interpolations;
    Arc::ASTNode::InterpolationKind interpolationParsingKind = Arc::ASTNode::InterpolationKind::SourceInterpolation;
    char lastChar = NULL;
    std::string builded;

    for (const char currentChar : string) {
        if (
            (currentChar == '$' && lastChar != '\\') ||
            (currentChar == ' ' || currentChar == '\n' || currentChar == '\t')
        ) {
            interpolations.push_back(this->parseInterpolation(builded, interpolationParsingKind));

            if (currentChar == '$' && lastChar != '\\') {
                interpolationParsingKind = Arc::ASTNode::InterpolationKind::ExpressionInterpolation;
            } else if (currentChar == ' ' || currentChar == '\n' || currentChar == '\t') {
                interpolationParsingKind = Arc::ASTNode::InterpolationKind::SourceInterpolation;
            }
            continue;
        } else if (currentChar == '{' && lastChar == '$') {
            continue;
        }

        builded += currentChar;
        lastChar = currentChar;
    }

    return (interpolations.size() > 1)
        ? std::make_optional(Arc::ASTNode::InterpolatedStringLiteral(interpolations, "\"" + string + "\""))
        : std::nullopt;
}

std::shared_ptr<Arc::ASTNode::NamespaceDeclaration> Arc::ASTBuilder::parseImport(const Arc::Package& importedSource) {
    if (importedSource->kind() == Arc::ImportPackageKind::FileImport) {
        const auto fileImportedSource = *std::dynamic_pointer_cast<Arc::FileImportPackage>(importedSource);
        Arc::Lexer lexer(fileImportedSource.contents);
        Arc::TokenStream tokenStream = lexer.parse();

        return std::make_shared<Arc::ASTNode::NamespaceDeclaration> (
            std::make_shared<Arc::ASTNode::IdentifierLiteral>(importedSource->name),
            Arc::AbstractSyntaxTree(
                this->buildAst(
                    tokenStream,
                    Arc::ASTBuildingContext {
                        .parsingNewPath = std::make_optional(importedSource->path)
                    }
                )
            )
        );
    } else if (importedSource->kind() == Arc::ImportPackageKind::DirectoryImport) {
        const auto directoryImportedSource = *std::dynamic_pointer_cast<Arc::DirectoryImportPackage>(importedSource);
        std::vector<Arc::Statement> importNamespaceBody;

        for (const Arc::Package& child : directoryImportedSource.children) {
            if (child->kind() == Arc::ImportPackageKind::FileImport) {
                const auto fileChildPackage = *std::dynamic_pointer_cast<Arc::FileImportPackage>(child);
                Arc::Lexer lexer(fileChildPackage.contents);
                Arc::TokenStream tokenStream = lexer.parse();
                const std::vector<Arc::Statement> fileAst = this->buildAst(tokenStream, Arc::ASTBuildingContext {
                    .parsingNewPath = std::make_optional(child->path)
                });

                importNamespaceBody.insert(
                    importNamespaceBody.end(),
                    fileAst.begin(),
                    fileAst.end()
                );
            } else {
                importNamespaceBody.push_back(
                    std::make_shared<Arc::ASTNode::NamespaceDeclaration> (
                        std::make_shared<Arc::ASTNode::IdentifierLiteral>(child->name),
                        this->parseImport(child)->body
                    )
                );
            }
        }

        return std::make_shared<Arc::ASTNode::NamespaceDeclaration> (
            std::make_shared<Arc::ASTNode::IdentifierLiteral>(importedSource->name),
            Arc::AbstractSyntaxTree(importNamespaceBody)
        );
    }

    std::unreachable();
}

std::string Arc::ASTBuilder::checkInlineConcatString(Arc::TokenStream tokenStream) {
    std::string builded;

    for (const auto& token : tokenStream.getTokens()) {
        builded += token.value;
        if (token.kind != Arc::TokenType::String) return "\0";
    }

    return builded;
}

std::vector<Arc::ASTNode::SwitchBlock> Arc::ASTBuilder::parseSwitchBody(Arc::TokenStream& tokens) {
    Arc::Token next = tokens.next();
    std::vector<Arc::ASTNode::SwitchBlock> collected;

    while (next.kind != Arc::TokenType::Eof) {
        if (next.kind == Arc::TokenType::Keyword) {
            if (next.value == "case") {
                Arc::TokenStream caseValueTokens = tokens.getBody(tokens.expect(Arc::TokenType::StartParenthesis));
                
                Arc::TokenStream bodyTokens = (tokens.at(0).kind == Arc::TokenType::StartBrace)
                    ? tokens.getBody(tokens.next())
                    : tokens.until(Arc::TokenType::Semicolon);

                collected.push_back(
                    std::make_shared<Arc::ASTNode::ValuedSwitchBlock>(
                        this->toAst(bodyTokens),
                        std::make_optional(this->parseExpression(caseValueTokens))
                    )
                );
            } else if (next.value == "default") {
                Arc::TokenStream bodyTokens = (tokens.at(0).kind == Arc::TokenType::StartBrace)
                    ? tokens.getBody(tokens.next())
                    : tokens.until(Arc::TokenType::Semicolon);

                collected.push_back(
                    std::make_shared<Arc::ASTNode::DefaultSwitchBlock> (
                        this->toAst(bodyTokens)
                    )
                );
            }
        }
    }

    return collected;
}

Arc::ASTNode::Decorator Arc::ASTBuilder::parseDecorator(Arc::TokenStream& tokens) {
    Arc::TokenStream signatureTokens = (tokens.at(0).kind == Arc::TokenType::StartParenthesis)
        ? tokens.getBody(tokens.next())
        : tokens.until({Arc::TokenType::AtSign, Arc::TokenType::BinaryOperator, Arc::TokenType::StartParenthesis, Arc::TokenType::Keyword});
    
    Arc::TokenStream genericTokens;
    Arc::TokenStream argumentTokens;

    if (tokens.at(0).kind == Arc::TokenType::BinaryOperator && tokens.at(0).value == "<") {
        genericTokens = tokens.getBody(tokens.next());
    }

    if (tokens.at(0).kind == Arc::TokenType::StartParenthesis) {
        argumentTokens = tokens.getBody(tokens.next());
    }

    return Arc::ASTNode::Decorator(
        this->parseExpression(signatureTokens),
        genericTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Type>([this](Arc::TokenStream& tokenStream) {
            return this->parseType(tokenStream);
        }),
        argumentTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        })
    );
}

Arc::ASTNode::Parameter Arc::ASTBuilder::parseParameter(Arc::TokenStream& tokens) {
    std::vector<std::string> modifiers;

    while (tokens.at(0).kind == Arc::TokenType::Keyword) {
        if (tokens.at(0).value == "span" || tokens.at(0).value == "named" || tokens.at(0).value == "const" || tokens.at(0).value == "nocopy") {
            modifiers.push_back(tokens.at(0).value);
        } else {
            Util::quit();
        }

        tokens.next();
    }

    const Arc::Token name = tokens.expect(Arc::TokenType::Identifier);
    Arc::ASTNode::Type type;
    Arc::TokenStream valueTokens;

    if (tokens.at(0).kind == Arc::TokenType::QuestionMark) {
        modifiers.push_back("optional");
        tokens.next();
    }

    if (tokens.at(0).kind == Arc::TokenType::Colon) {
        tokens.next();
        type = this->parseType(tokens);
    }

    if (tokens.at(0).kind == Arc::TokenType::AssignmentOperator && tokens.at(0).value == "=") {
        valueTokens = tokens.until({Arc::TokenType::Comma, Arc::TokenType::EndParenthesis});
    }

    return Arc::ASTNode::Parameter (
        Util::includes<std::string>(modifiers, "span"),
        !Util::includes<std::string>(modifiers, "optional"),
        Util::includes<std::string>(modifiers, "named"),
        Util::includes<std::string>(modifiers, "const"),
        Util::includes<std::string>(modifiers, "passbyref"),
        (valueTokens.empty()) ? std::nullopt : std::make_optional(this->parseExpression(valueTokens)),
        name.value,
        type
    );
}

Arc::ASTNode::Generic Arc::ASTBuilder::parseGeneric(Arc::TokenStream& tokens) {
    std::vector<std::string> modifiers;

    while (tokens.at(0).kind == Arc::TokenType::Keyword) {
        if (tokens.at(0).value == "span" || tokens.at(0).value == "named") {
            modifiers.push_back(tokens.at(0).value);
        } else {
            Util::quit();
        }
        
        tokens.next();
    }

    const Arc::Token name = tokens.expect(Arc::TokenType::Identifier);
    Arc::TokenStream genericTokens;
    Arc::TokenStream argumentTokens;
    Arc::ASTNode::Type defaultValue;

    if (tokens.at(0).kind == Arc::TokenType::BinaryOperator && tokens.at(0).value == "<") {
        genericTokens = tokens.getBody(tokens.next());
    }

    if (tokens.at(0).kind == Arc::TokenType::StartParenthesis) {
        argumentTokens = tokens.getBody(tokens.next());
    }

    if (tokens.at(0).kind == Arc::TokenType::QuestionMark) {
        modifiers.push_back("optional");
        tokens.next();
    }

    if (tokens.at(0).kind == Arc::TokenType::AssignmentOperator && tokens.at(0).value == "=") {
        tokens.next();
        defaultValue = this->parseType(tokens);
    }

    return Arc::ASTNode::Generic (
        Util::includes<std::string>(modifiers, "span"),
        Util::includes<std::string>(modifiers, "optional"),
        Util::includes<std::string>(modifiers, "named"),
        name.value,
        defaultValue,
        genericTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Generic>([this](Arc::TokenStream& tokenStream) {
            return this->parseGeneric(tokenStream);
        }),
        argumentTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Parameter>([this](Arc::TokenStream& tokenStream) {
            return this->parseParameter(tokenStream);
        })
    );
}