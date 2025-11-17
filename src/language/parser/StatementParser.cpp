#include "../util/Util.h"
#include "Parser.h"

Arc::Statement Arc::ASTBuilder::parseKeywordStatement(const Arc::Token& token, Arc::TokenStream& tokenStream) {
    if (Util::includes<std::string>(Arc::modifiers, token.value)) {
        this->collectedModifiers.push_back(Arc::ASTNode::Modifier(token.value));
    } else if (token.value == "label") {
        return std::make_shared<Arc::ASTNode::LabelStatement>(tokenStream.expect(Arc::TokenType::Identifier).value);
    } else if (token.value == "goto") {
        return std::make_shared<Arc::ASTNode::GotoStatement>(tokenStream.expect(Arc::TokenType::Identifier).value);
    } else if (token.value == "break") {
        Arc::TokenStream expressionTokens = tokenStream.until(Arc::TokenType::Semicolon);
        return std::make_shared<Arc::ASTNode::BreakStatement>(this->parseExpression(expressionTokens));
    } else if (token.value == "continue") {
        Arc::TokenStream expressionTokens = tokenStream.until(Arc::TokenType::Semicolon);
        return std::make_shared<Arc::ASTNode::ContinueStatement>(this->parseExpression(expressionTokens));
    } else if (token.value == "return") {
        Arc::TokenStream expressionTokens = tokenStream.until(Arc::TokenType::Semicolon);
        return std::make_shared<Arc::ASTNode::ReturnStatement>((this->parseExpression(expressionTokens)));
    } else if (token.value == "namespace") {
        return std::make_shared<Arc::ASTNode::NamespaceDeclaration>(this->parseNamespaceDeclaration(tokenStream));
    } else if (token.value == "when") {
        return std::make_shared<Arc::ASTNode::WhenStatement>(this->parseWhenStatement(tokenStream));
    } else if (token.value == "while") {
        return std::make_shared<Arc::ASTNode::WhileStatement>(this->parseWhileStatement(tokenStream));
    } else if (token.value == "if") {
        return std::make_shared<Arc::ASTNode::IfStatement>(this->parseIfStatement(tokenStream));
    } else if (token.value == "forever") {
        return std::make_shared<Arc::ASTNode::ForeverStatement>(this->parseForeverStatement(tokenStream));
    } else if (token.value == "foreach") {
        return std::make_shared<Arc::ASTNode::ForeachStatement>(this->parseForeachStatement(tokenStream));
    } else if (token.value == "switch") {
        return std::make_shared<Arc::ASTNode::SwitchStatement>(this->parseSwitchStatement(tokenStream));
    } else if (token.value == "extension") {
        return std::make_shared<Arc::ASTNode::ExtensionDeclaration>(this->parseExtensionDeclaration(tokenStream));
    } else if (token.value == "class") {
        return std::make_shared<Arc::ASTNode::ClassDeclaration>(this->parseClassDeclaration(tokenStream));
    } else if (token.value == "struct") {
        return std::make_shared<Arc::ASTNode::StructDeclaration>(this->parseStructDeclaration(tokenStream));
    } else if (token.value == "macro") {
        return std::make_shared<Arc::ASTNode::MacroDeclaration>(this->parseMacroDeclaration(tokenStream));
    } else if (token.value == "interface") {
        return std::make_shared<Arc::ASTNode::InterfaceDeclaration>(this->parseInterfaceDeclaration(tokenStream));
    } else if (token.value == "where") {
        return std::make_shared<Arc::ASTNode::WhereStatement>(this->parseWhereStatement(tokenStream));
    } else if (token.value == "var" || token.value == "const") {
        return std::make_shared<Arc::ASTNode::VariableDeclaration>(this->parseVariableDeclaration(token.value == "const", tokenStream));
    } else if (token.value == "type") {
        return std::make_shared<Arc::ASTNode::TypeDeclaration>(this->parseTypeDeclaration(tokenStream));
    } else if (token.value == "enum") {
        return std::make_shared<Arc::ASTNode::EnumDeclaration>(this->parseEnumDeclaration(tokenStream));
    } else if (token.value == "catch") {
        return std::make_shared<Arc::ASTNode::CatchHandleStatement>(this->parseCatchHandleStatement(tokenStream));
    } else if (token.value == "fn") {
        return std::make_shared<Arc::ASTNode::FunctionDeclaration>(this->parseFunctionDeclaration(tokenStream, false));
    } else if (token.value == "with") {
        return std::make_shared<Arc::ASTNode::WithStatement>(this->parseWithStatement(tokenStream));
    } else if (token.value == "error") {
        return std::make_shared<Arc::ASTNode::ErrorDeclaration>(this->parseErrorDeclaration(tokenStream));
    }

    std::unreachable();
}

Arc::Statement Arc::ASTBuilder::parseExpressionStatement(const Arc::Token& token, Arc::TokenStream& tokenStream) {
    tokenStream.prependToken(token);
    Arc::TokenStream expressionTokens = tokenStream.until(Arc::TokenType::Semicolon);
    Arc::Expression expression = this->parseExpression(expressionTokens);

    Arc::ExpressionKind kind = expression->kind();

    switch (kind) {
        case Arc::ExpressionKind::AssignmentExpression: {
            auto assignmentExpression = *std::dynamic_pointer_cast<Arc::ASTNode::AssignmentExpression>(expression);

            return std::make_shared<Arc::ASTNode::AssignmentStatement>(
                assignmentExpression.left,
                assignmentExpression.right,
                assignmentExpression.op
            );
        }
        
        case Arc::ExpressionKind::CallExpression: {
            auto callExpression = *std::dynamic_pointer_cast<Arc::ASTNode::CallExpression>(expression);

            return std::make_shared<Arc::ASTNode::CallStatement>(
                callExpression.callee,
                callExpression.generics,
                callExpression.arguments
            );
        };

        default: return std::make_shared<Arc::ASTNode::ExpressionStatement>(expression);
    }
}

Arc::ASTNode::NamespaceDeclaration Arc::ASTBuilder::parseNamespaceDeclaration(Arc::TokenStream& tokenStream) {
    Arc::TokenStream nameTokens = tokenStream.until(Arc::TokenType::StartBrace, true);
    Arc::TokenStream bodyTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBrace));

    return Arc::ASTNode::NamespaceDeclaration (
        this->parseExpression(nameTokens),
        this->toAst(bodyTokens)
    );
}

Arc::ASTNode::WhenStatement Arc::ASTBuilder::parseWhenStatement(Arc::TokenStream& tokenStream) {
    Arc::TokenStream conditionTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

    Arc::TokenStream bodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);

    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::WhenStatement (
        this->parseExpression(conditionTokens),
        this->toAst(bodyTokens),
        modifiers
    );
}

Arc::ASTNode::WhileStatement Arc::ASTBuilder::parseWhileStatement(Arc::TokenStream& tokenStream) {
    Arc::TokenStream conditionTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

    Arc::TokenStream bodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);

    Arc::TokenStream elseBodyTokens = (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "else")
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);

    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::WhileStatement (
        this->parseExpression(conditionTokens),
        this->toAst(bodyTokens),
        (!elseBodyTokens.empty()) ? std::make_optional(this->toAst(elseBodyTokens)) : std::nullopt,
        modifiers
    );
}

Arc::ASTNode::IfStatement Arc::ASTBuilder::parseIfStatement(Arc::TokenStream& tokenStream) {
    Arc::TokenStream conditionTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));
    
    Arc::TokenStream bodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);

    Arc::TokenStream elseBodyTokens = (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "else")
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);

    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::IfStatement (
        this->parseExpression(conditionTokens),
        this->toAst(bodyTokens),
        this->toAst(elseBodyTokens),
        modifiers
    );;
}

Arc::ASTNode::ForeachStatement Arc::ASTBuilder::parseForeachStatement(Arc::TokenStream& tokenStream) {
    Arc::TokenStream iteratorTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));
    
    Arc::TokenStream bodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);

    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::ForeachStatement (
        this->toAst(bodyTokens),
        std::make_shared<Arc::ASTNode::IteratorExpression>(this->parseIteratorExpression(iteratorTokens)),
        modifiers
    );
}

Arc::ASTNode::SwitchStatement Arc::ASTBuilder::parseSwitchStatement(Arc::TokenStream& tokenStream) {
    Arc::TokenStream switchedValueTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

    Arc::TokenStream bodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);
    
    std::vector<Arc::ASTNode::SwitchBlock> switchBody = this->parseSwitchBody(bodyTokens);
    
    std::vector<Arc::ASTNode::ValuedSwitchBlock> valueCases;
    std::optional<Arc::ASTNode::DefaultSwitchBlock> defaultCase;
    for (const auto& entry : switchBody) {
        if (entry->kind() == Arc::ASTNode::SwitchBlockKind::ValuedCase) {
            const auto castCase = *std::dynamic_pointer_cast<Arc::ASTNode::ValuedSwitchBlock>(entry);
            valueCases.push_back(castCase);
        } else {
            const auto castCase = *std::dynamic_pointer_cast<Arc::ASTNode::DefaultSwitchBlock>(entry);
            defaultCase = castCase;
        }
    }

    return Arc::ASTNode::SwitchStatement (
        this->parseExpression(switchedValueTokens),
        valueCases,
        defaultCase,
        this->flushModifiers()
    );
}

Arc::ASTNode::ExtensionDeclaration Arc::ASTBuilder::parseExtensionDeclaration(Arc::TokenStream& tokenStream) {
    const Arc::TokenStream extensionClassTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));
    Arc::TokenStream interfaceSignatureTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "implements") {
        tokenStream.next();
        interfaceSignatureTokens = tokenStream.until(Arc::TokenType::Keyword, "for", true);
    }

    Arc::TokenStream bodyTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBrace));

    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::ExtensionDeclaration (
        extensionClassTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        }),
        interfaceSignatureTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        }),
        this->toAst(bodyTokens),
        modifiers
    );
}

Arc::ASTNode::ClasslikeDeclaration Arc::ASTBuilder::parseClasslikeDeclaration(Arc::TokenStream& tokenStream, const bool expression, const bool parsingEnum) {
    Arc::TokenStream signatureTokens;
    Arc::TokenStream constructorDeclarationTokens;
    Arc::TokenStream implementationTokens;
    Arc::TokenStream extensionTokens;
    Arc::TokenStream datatypeTokens;
    Arc::TokenStream bodyTokens;

    if (!expression) {
        signatureTokens = tokenStream.until({Arc::TokenType::Semicolon, Arc::TokenType::StartBrace, Arc::TokenType::Keyword, Arc::TokenType::StartParenthesis}, true);
        if (tokenStream.at(0).kind == Arc::TokenType::Semicolon) {
            tokenStream.next();
        }
    }

    if (tokenStream.at(0).kind == Arc::TokenType::StartParenthesis) {
        constructorDeclarationTokens = tokenStream.getBody(tokenStream.next());
    }

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value != "implements" && tokenStream.at(0).value != "extends") {
        // throw err
    }

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "implements") {
        tokenStream.next();
        implementationTokens = tokenStream.until({Arc::TokenType::Semicolon, Arc::TokenType::StartBrace, Arc::TokenType::Keyword, Arc::TokenType::ReturnArrow}, true);
    }

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "extends") {
        tokenStream.next();
        extensionTokens = tokenStream.until({Arc::TokenType::Semicolon, Arc::TokenType::StartBrace, Arc::TokenType::Keyword, Arc::TokenType::ReturnArrow}, true);
    }

    if (parsingEnum && tokenStream.at(0).kind == Arc::TokenType::ReturnArrow) {
        tokenStream.next();
        datatypeTokens = tokenStream.until({Arc::TokenType::Semicolon, Arc::TokenType::StartBrace, Arc::TokenType::Keyword, Arc::TokenType::ReturnArrow}, true);
    }

    if (tokenStream.at(0).kind == Arc::TokenType::StartBrace) {
        bodyTokens = tokenStream.getBody(tokenStream.next());
    } else if (tokenStream.at(0).kind != Arc::TokenType::Semicolon) {
        // throw err
    }

    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();
    const std::vector<Arc::ASTNode::Decorator> decorators = this->flushDecorators();

    return Arc::ASTNode::ClasslikeDeclaration (
        (expression) ? std::nullopt : std::make_optional(this->parseExpression(signatureTokens)),
        extensionTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        }),
        implementationTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        }),
        this->toAst(constructorDeclarationTokens),
        bodyTokens.empty(),
        (!bodyTokens.empty()) ? std::make_optional(this->toAst(bodyTokens)) : std::nullopt,
        (!datatypeTokens.empty()) ? std::make_optional(this->parseType(datatypeTokens)) : std::nullopt,
        decorators,
        modifiers
    );
}

Arc::ASTNode::ClassDeclaration Arc::ASTBuilder::parseClassDeclaration(Arc::TokenStream& tokenStream) {
    return Arc::ASTNode::ClassDeclaration(this->parseClasslikeDeclaration(tokenStream, false, false));
}

Arc::ASTNode::StructDeclaration Arc::ASTBuilder::parseStructDeclaration(Arc::TokenStream& tokenStream) {
    return Arc::ASTNode::StructDeclaration(this->parseClasslikeDeclaration(tokenStream, false, false));
}

Arc::ASTNode::MacroDeclaration Arc::ASTBuilder::parseMacroDeclaration(Arc::TokenStream& tokenStream) {
    Arc::TokenStream signatureTokens = tokenStream.until({Arc::TokenType::Semicolon, Arc::TokenType::StartBrace}, true);
    if (tokenStream.at(0).kind == Arc::TokenType::Semicolon) {
        tokenStream.next();
    }

    Arc::TokenStream bodyTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::StartBrace) {
        bodyTokens = tokenStream.getBody(tokenStream.next());
    } else if (tokenStream.at(0).kind != Arc::TokenType::Semicolon) {
        // throw err
    }

    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::MacroDeclaration (
        this->parseExpression(signatureTokens),
        bodyTokens.empty(),
        (!bodyTokens.empty()) ? std::make_optional(this->toAst(bodyTokens)) : std::nullopt,
        modifiers
    );
}

Arc::ASTNode::InterfaceDeclaration Arc::ASTBuilder::parseInterfaceDeclaration(Arc::TokenStream& tokenStream) {
    return Arc::ASTNode::InterfaceDeclaration(this->parseClasslikeDeclaration(tokenStream, false, false));
}

Arc::ASTNode::WhereStatement Arc::ASTBuilder::parseWhereStatement(Arc::TokenStream& tokenStream) {
    Arc::TokenStream conditionTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));
    Arc::TokenStream bodyTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBrace));

    return Arc::ASTNode::WhereStatement (
        this->parseExpression(conditionTokens),
        this->toAst(bodyTokens)
    );
}

Arc::ASTNode::VariableDeclaration Arc::ASTBuilder::parseVariableDeclaration(const bool constant, Arc::TokenStream& tokenStream) {
    Arc::TokenStream signatureTokens = tokenStream.until({Arc::TokenType::Semicolon, Arc::TokenType::AssignmentOperator, Arc::TokenType::Colon}, true);
    
    if (tokenStream.at(0).kind == Arc::TokenType::Semicolon) {
        tokenStream.next();
    }

    Arc::TokenStream datatypeTokens;
    Arc::TokenStream valueTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::Colon) {
        tokenStream.next();
        datatypeTokens = tokenStream.until({Arc::TokenType::AssignmentOperator, Arc::TokenType::Semicolon}, true);
        if (tokenStream.at(0).kind == Arc::TokenType::Semicolon) {
            tokenStream.next();
        }
    }

    if (tokenStream.at(0).kind == Arc::TokenType::AssignmentOperator && tokenStream.at(0).value == "=") {
        tokenStream.next();
        valueTokens = tokenStream.until(Arc::TokenType::Semicolon);
    } else if (tokenStream.at(0).kind != Arc::TokenType::Semicolon) {
        // throw err
    }
    
    const std::vector<Arc::ASTNode::Decorator> decorators = this->flushDecorators();
    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::VariableDeclaration (
        constant,
        this->parseExpression(signatureTokens),
        (!datatypeTokens.empty()) ? std::make_optional(this->parseType(datatypeTokens)) : std::nullopt,
        valueTokens.empty(),
        (!valueTokens.empty()) ? std::make_optional(this->parseExpression(valueTokens)) : std::nullopt,
        this->flushDecorators(),
        this->flushModifiers()
    );
}

Arc::ASTNode::TypeDeclaration Arc::ASTBuilder::parseTypeDeclaration(Arc::TokenStream& tokenStream) {
    Arc::TokenStream signatureTokens = tokenStream.until({Arc::TokenType::Semicolon, Arc::TokenType::AssignmentOperator, Arc::TokenType::BinaryOperator}, true);
    
    if (tokenStream.at(0).kind == Arc::TokenType::Semicolon) {
        tokenStream.next();
    }

    Arc::TokenStream genericTokens;
    Arc::TokenStream parameterTokens;
    Arc::TokenStream valueTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::BinaryOperator && tokenStream.at(0).value == "<") {
        genericTokens = tokenStream.getBody(tokenStream.next());
    }

    if (tokenStream.at(0).kind == Arc::TokenType::StartParenthesis) {
        parameterTokens = tokenStream.getBody(tokenStream.next());
    }

    if (tokenStream.at(0).kind == Arc::TokenType::AssignmentOperator && tokenStream.at(0).value == "=") {
        valueTokens = tokenStream.until(Arc::TokenType::Semicolon);
    } else if (tokenStream.at(0).kind != Arc::TokenType::Semicolon) {
        // throw err
    }

    const std::vector<Arc::ASTNode::Modifier> modifiers;
    const std::vector<Arc::ASTNode::Decorator> decorators;

    return Arc::ASTNode::TypeDeclaration (
        this->parseExpression(signatureTokens),
        genericTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Generic>([this](Arc::TokenStream& tokenStream) {
            return this->parseGeneric(tokenStream);
        }),
        parameterTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Parameter>([this](Arc::TokenStream& tokenStream) {
            return this->parseParameter(tokenStream);
        }),
        valueTokens.empty(),
        (!valueTokens.empty()) ? std::make_optional(this->parseType(valueTokens)) : std::nullopt,
        decorators,
        modifiers
    );
}

Arc::ASTNode::EnumDeclaration Arc::ASTBuilder::parseEnumDeclaration(Arc::TokenStream& tokenStream) {
    return Arc::ASTNode::EnumDeclaration(this->parseClasslikeDeclaration(tokenStream, false, true));
}

Arc::ASTNode::EnumMember Arc::ASTBuilder::parseEnumMember(Arc::TokenStream& tokenStream) {
    Arc::TokenStream signatureTokens = tokenStream.until({Arc::TokenType::Comma, Arc::TokenType::BinaryOperator, Arc::TokenType::StartParenthesis, Arc::TokenType::StartBrace}, true);
    Arc::TokenStream genericTokens;
    Arc::TokenStream parameterTokens;
    Arc::TokenStream bodyTokens;
    Arc::TokenStream defaultValueTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::BinaryOperator && tokenStream.at(0).value == "<") {
        genericTokens = tokenStream.getBody(tokenStream.next());
    }

    if (tokenStream.at(0).kind == Arc::TokenType::StartParenthesis) {
        parameterTokens = tokenStream.getBody(tokenStream.next());
    }

    if (tokenStream.at(0).kind == Arc::TokenType::StartBrace) {
        bodyTokens = tokenStream.getBody(tokenStream.next());
    }

    if (tokenStream.at(0).kind == Arc::TokenType::AssignmentOperator && tokenStream.at(0).value == "=") {
        defaultValueTokens = tokenStream.until({Arc::TokenType::Comma, Arc::TokenType::Semicolon});
    }

    return Arc::ASTNode::EnumMember (
        this->parseExpression(signatureTokens),
        (!genericTokens.empty()) ? std::make_optional(genericTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Generic>([this](Arc::TokenStream& tokenStream) {
            return this->parseGeneric(tokenStream);
        })) : std::nullopt,
        (!parameterTokens.empty()) ? std::make_optional(parameterTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Parameter>([this](Arc::TokenStream& tokenStream) {
            return this->parseParameter(tokenStream);
        })) : std::nullopt,
        this->toAst(bodyTokens),
        this->parseExpression(defaultValueTokens)
    );
}

Arc::ASTNode::FunctionDeclaration Arc::ASTBuilder::parseFunctionDeclaration(Arc::TokenStream& tokenStream, const bool expression) {
    Arc::TokenStream signatureTokens;
    Arc::TokenStream genericTokens;
    Arc::TokenStream selectedSymbolTokens;
    Arc::TokenStream parameterTokens;
    Arc::TokenStream datatypeTokens;
    Arc::TokenStream bodyTokens;
    Arc::TokenStream satisfierTokens;

    if (!expression) {
        signatureTokens = tokenStream.until({Arc::TokenType::BinaryOperator, Arc::TokenType::StartParenthesis}, true);
    }

    if (tokenStream.at(0).kind == Arc::TokenType::BinaryOperator && tokenStream.at(0).value == "<") {
        genericTokens = tokenStream.getBody(tokenStream.next());
    }

    if (tokenStream.at(0).kind == Arc::TokenType::StartBracket) {
        selectedSymbolTokens = tokenStream.getBody(tokenStream.next());
    }

    parameterTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

    if (tokenStream.at(0).kind == Arc::TokenType::ReturnArrow) {
        tokenStream.next();
        datatypeTokens = tokenStream.until({Arc::TokenType::StartBrace, Arc::TokenType::FunctionArrow, Arc::TokenType::Keyword}, true);
    }

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "satisfies") {
        satisfierTokens = tokenStream.until({Arc::TokenType::StartBrace, Arc::TokenType::FunctionArrow}, true);
    }

    if (tokenStream.at(0).kind == Arc::TokenType::StartBrace) {
        bodyTokens = tokenStream.getBody(tokenStream.next());
    } else if (tokenStream.at(0).kind == Arc::TokenType::FunctionArrow) {
        tokenStream.next();
        bodyTokens = tokenStream.until(Arc::TokenType::Semicolon);
    } else if (tokenStream.at(0).kind != Arc::TokenType::Semicolon) {
        // throw err
    }

    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();
    const std::vector<Arc::ASTNode::Decorator> decorators = this->flushDecorators();

    return Arc::ASTNode::FunctionDeclaration (
        (!signatureTokens.empty()) ? this->parseExpression(signatureTokens) : nullptr,
        (!datatypeTokens.empty()) ? this->parseType(datatypeTokens) : Arc::ASTNode::Type(),
        genericTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Generic>([this](Arc::TokenStream& tokenStream) {
            return this->parseGeneric(tokenStream);
        }),
        selectedSymbolTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        }),
        modifiers,
        parameterTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Parameter>([this](Arc::TokenStream& tokenStream) {
            return this->parseParameter(tokenStream);
        }),
        bodyTokens.empty(),
        (!bodyTokens.empty()) ? std::make_optional(this->toAst(bodyTokens)) : std::nullopt,
        decorators,
        satisfierTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        })
    );
}

Arc::ASTNode::CatchHandleStatement Arc::ASTBuilder::parseCatchHandleStatement(Arc::TokenStream& tokenStream) {
    Arc::TokenStream bodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);

    return Arc::ASTNode::CatchHandleStatement (
        this->toAst(bodyTokens)
    );
}

Arc::ASTNode::WithStatement Arc::ASTBuilder::parseWithStatement(Arc::TokenStream& tokenStream) {
    Arc::TokenStream exprTokens = (tokenStream.at(0).kind == Arc::TokenType::StartParenthesis)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);
    
    Arc::TokenStream bodyTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::StartBrace) {
        bodyTokens = tokenStream.getBody(tokenStream.next());
    }

    return Arc::ASTNode::WithStatement (
        this->parseExpression(exprTokens),
        (!bodyTokens.empty()) ? std::make_optional(this->toAst(bodyTokens)) : std::nullopt
    );
}

Arc::ASTNode::ErrorDeclarationMember Arc::ASTBuilder::parseErrorDeclarationMember(Arc::TokenStream& tokenStream) {
    Arc::TokenStream signatureTokens = tokenStream.until(Arc::TokenType::StartParenthesis);
    Arc::TokenStream messageTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::StartParenthesis) {
        messageTokens = tokenStream.getBody(tokenStream.next());
    }

    return Arc::ASTNode::ErrorDeclarationMember (
        this->parseExpression(signatureTokens),
        (!messageTokens.empty()) ? std::make_optional(this->parseExpression(messageTokens)) : std::nullopt
    );
}

Arc::ASTNode::ErrorDeclaration Arc::ASTBuilder::parseErrorDeclaration(Arc::TokenStream& tokenStream) {
    Arc::TokenStream errorMemberTokens;
    std::vector<Arc::ASTNode::ErrorDeclarationMember> errorMembers;
    Arc::TokenStream inheritanceTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::StartBrace) {
        errorMemberTokens = tokenStream.getBody(tokenStream.next());

        errorMembers = errorMemberTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::ErrorDeclarationMember>([this](Arc::TokenStream& tokenStream) {
            return this->parseErrorDeclarationMember(tokenStream);
        });
    } else {
        errorMemberTokens = tokenStream.until({Arc::TokenType::Semicolon, Arc::TokenType::Keyword}, true);
        if (tokenStream.at(0).kind == Arc::TokenType::Semicolon) {
            tokenStream.next();
        }

        errorMembers.push_back(this->parseErrorDeclarationMember(errorMemberTokens));
    }

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "extends") {
        inheritanceTokens = tokenStream.until(Arc::TokenType::Semicolon);
    }

    return Arc::ASTNode::ErrorDeclaration (
        errorMembers,
        inheritanceTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        })
    );
}

Arc::ASTNode::ForeverStatement Arc::ASTBuilder::parseForeverStatement(Arc::TokenStream& tokenStream) {
    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();
    Arc::TokenStream bodyTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBrace));

    return Arc::ASTNode::ForeverStatement (
        this->toAst(bodyTokens),
        modifiers
    );
}

Arc::ASTNode::FunctorDeclaration Arc::ASTBuilder::parseFunctorDeclaration(Arc::TokenStream& tokenStream) {
    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();
    Arc::TokenStream signatureTokens = tokenStream.until(Arc::TokenType::AssignmentOperator, "=");

    return Arc::ASTNode::FunctorDeclaration(
        std::make_shared<Arc::ASTNode::FunctorExpression>(this->parseFunctionDeclaration(tokenStream, true)),
        this->parseExpression(signatureTokens),
        modifiers
    );
}

Arc::ASTNode::ContractDeclaration Arc::ASTBuilder::parseContractDeclaration(Arc::TokenStream& tokenStream) {
    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();
    Arc::TokenStream signatureTokens = tokenStream.until(Arc::TokenType::AssignmentOperator, "=");

    return Arc::ASTNode::ContractDeclaration(
        this->parseExpression(signatureTokens),
        modifiers,
        this->parseContractExpression(tokenStream)
    );
}