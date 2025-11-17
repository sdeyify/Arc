#include "Parser.h"
#include "../util/Util.h"
#include "../importer/Importer.h"
#include "../messages/Messages.h"

Arc::Expression Arc::ASTBuilder::parseExpression(Arc::TokenStream& tokenStream) {
    if (tokenStream.size() == 1) {
        return this->parseLiteral(tokenStream.next());
    } else if (tokenStream.getTokens().size() > 0) {
        const Arc::TokenStream topLevelTokens = tokenStream.getTopLevelTokens();
        const std::string stringifiedTopLevelTokens = topLevelTokens.stringify();
        const std::string stringifiedTokens = tokenStream.stringify();

        if (stringifiedTopLevelTokens == "()") {
            Arc::TokenStream bodyTokens = tokenStream.getBody(tokenStream.next());
            return this->parseExpression(bodyTokens);
        } else if (stringifiedTopLevelTokens == "[]") {
            return std::make_shared<Arc::ASTNode::ArrayLiteral>(stringifiedTokens,
                tokenStream.getBody(tokenStream.next())
                    .split(Arc::TokenType::Comma)
                    .transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
                        return this->parseExpression(tokenStream);
                    })
            );
        } else if (stringifiedTopLevelTokens == ".{}") {
            tokenStream.next();
            return std::make_shared<Arc::ASTNode::ObjectLiteral>(this->parseObjectLiteral(tokenStream));
        } else if (stringifiedTopLevelTokens == ".{}") {
            return std::make_shared<Arc::ASTNode::ObjectLiteral>(this->parseObjectLiteral(tokenStream));
        } else if (stringifiedTopLevelTokens == "!{}") {
            tokenStream.next();
            return std::make_shared<Arc::ASTNode::AnonymousFunctionExpression>(this->parseShorthandLambda(tokenStream));
        } else if (
            topLevelTokens.at(-2).kind == Arc::TokenType::StartParenthesis &&
            topLevelTokens.at(-1).kind == Arc::TokenType::EndParenthesis &&
            tokenStream.at(0).kind == Arc::TokenType::AtSign
        ) {
            return std::make_shared<Arc::ASTNode::CallExpression>(this->parseCallExpression(tokenStream));
        } else if (topLevelTokens.includes(Arc::TokenType::Pipeline)) {
            return std::make_shared<Arc::ASTNode::PipelineExpression>(this->parsePipelineExpression(tokenStream));
        } else if (topLevelTokens.includes(Arc::TokenType::BinaryOperator)) {
            Arc::ASTNode::BinaryExpression expr = this->parseBinaryExpression(tokenStream);

            if (expr.op == "...") {
                return std::make_shared<Arc::ASTNode::RangeLiteral>(stringifiedTokens, expr.left, expr.right);
            } else if (expr.op == ".") {
                if (expr.left->kind() == Arc::ExpressionKind::NumericLiteral && expr.right->kind() == Arc::ExpressionKind::NumericLiteral) {
                    const Arc::ASTNode::NumericLiteral rhs = *std::dynamic_pointer_cast<Arc::ASTNode::NumericLiteral>(expr.right);
                    const Arc::ASTNode::NumericLiteral lhs = *std::dynamic_pointer_cast<Arc::ASTNode::NumericLiteral>(expr.left);
                    return std::make_shared<Arc::ASTNode::NumericLiteral>(lhs.raw + "." + rhs.raw);
                }
                return std::make_shared<Arc::ASTNode::MemberExpression>(expr.left, expr.right);
            } else if (expr.op == "::") {
                return std::make_shared<Arc::ASTNode::StaticMemberExpression>(expr.left, expr.right);
            } else if (Util::includes<std::string>(Arc::assignmentOperators, expr.op)) {
                return std::make_shared<Arc::ASTNode::AssignmentExpression>(expr.left, expr.right, expr.op);
            } else {
                return std::make_shared<Arc::ASTNode::BinaryExpression>(expr);
            }
        } else if (topLevelTokens.at(-1).kind == Arc::TokenType::UnaryOperator) {
            const Arc::Token op = tokenStream.at(-1);
            tokenStream.remove(-1);
            
            return std::make_shared<Arc::ASTNode::UnaryExpression>(
                this->parseExpression(tokenStream),
                true, op.value
            );
        } else if (
            topLevelTokens.at(0).kind == Arc::TokenType::UnaryOperator ||
            (topLevelTokens.at(0).kind == Arc::TokenType::BinaryOperator && topLevelTokens.at(0).value == "...") ||
            (topLevelTokens.at(0).kind == Arc::TokenType::BinaryOperator && topLevelTokens.at(0).value == "-") ||
            (topLevelTokens.at(0).kind == Arc::TokenType::Keyword && topLevelTokens.at(0).value == "throw") ||
            (topLevelTokens.at(0).kind == Arc::TokenType::Keyword && topLevelTokens.at(0).value == "new")
        ) {
            const Arc::Token op = tokenStream.next();
            const Arc::Expression expr = this->parseExpression(tokenStream);

            if (op.value == "...") {
                return std::make_shared<Arc::ASTNode::SpreadExpression>(expr);
            } else if (op.value == "throw") {
                return std::make_shared<Arc::ASTNode::ThrowExpression>(expr);
            } else if (op.value == "new") {
                return std::make_shared<Arc::ASTNode::NewExpression>(expr);
            } else {
                return std::make_shared<Arc::ASTNode::UnaryExpression>(
                    expr,
                    false,
                    op.value
                );
            }
        } else if (
            topLevelTokens.at(-2).kind == Arc::TokenType::StartBracket &&
            topLevelTokens.at(-1).kind == Arc::TokenType::EndBracket
        ) {
            return std::make_shared<Arc::ASTNode::ArrayAccessExpression>(this->parseArrayAccessExpression(tokenStream));
        } else if (tokenStream.at(0).kind == Arc::TokenType::Keyword) {
            if (tokenStream.at(0).value == "import") {
                tokenStream.next();

                return std::make_shared<Arc::ASTNode::AnonymousNamespaceExpression>(this->parseImportExpression(tokenStream));
            }
            return this->parseAnonymousDeclarations(tokenStream);
        } else if (const std::string concatString = this->checkInlineConcatString(tokenStream); concatString != "\0") {
            const std::optional<Arc::ASTNode::InterpolatedStringLiteral> possibleInterpolatedStringLiteral = this->parseInterpolatedString(concatString.substr(1, concatString.length() - 2));
            
            if (possibleInterpolatedStringLiteral == std::nullopt) {
                return std::make_shared<Arc::ASTNode::StringLiteral>(concatString);
            } else {
                return std::make_shared<Arc::ASTNode::InterpolatedStringLiteral>(*possibleInterpolatedStringLiteral);
            }
        } else if (tokenStream.at(0).kind == Arc::TokenType::AtSign) {
            return this->parseSpecialSymbol(tokenStream);
        } else if (tokenStream.at(0).kind == Arc::TokenType::Hash) {
            // Parse Macro
        }
    }

    return nullptr;
}

Arc::ASTNode::AnonymousNamespaceExpression Arc::ASTBuilder::parseImportExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream circrefTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "circref") {
        tokenStream.next();
        circrefTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::BinaryOperator));
    }

    const Arc::Token pathToken = tokenStream.expect(Arc::TokenType::String);

    if (
        !this->circrefGuards.empty() &&
        this->circrefGuards.back().guardedFor == this->pathStack.back() &&
        pathToken.valueWithoutQuotes() == this->circrefGuards.back().guardedPath
    ) {
        const Arc::AbstractSyntaxTree replacements = this->circrefGuards.back().symbolReplacements;
        this->circrefGuards.pop_back();
        return Arc::ASTNode::AnonymousNamespaceExpression(replacements);
    }

    if (!circrefTokens.empty()) {
        this->circrefGuards.push_back(Arc::ASTNode::CircularReferenceGuard(
            circrefTokens.split(Arc::TokenType::Comma).transform<Arc::Statement>([this](Arc::TokenStream& tokenStream) {
                const Arc::AbstractSyntaxTree circrefSymbols = this->toAst(tokenStream);

                if (circrefSymbols.size() > 1) {
                    // throw err
                } else if (circrefSymbols.size() < 1) {
                    // throw err
                }

                const Arc::Statement circrefSymbol = circrefSymbols.at(0);
                
                if (!circrefSymbol->hasModifiers()) {
                    // throw err
                }

                const Arc::ModifiableStatement modifiableCircrefSymbol = *std::dynamic_pointer_cast<Arc::ModifiableStatement>(circrefSymbol);
                modifiableCircrefSymbol->modifiers.push_back(
                    Arc::ASTNode::Modifier("circrefdecl")
                );

                return modifiableCircrefSymbol;
            }),
            this->pathStack.back(),
            pathToken.valueWithoutQuotes()
        ));
    }

    Arc::ArcImporter importer(this->pathStack.back());
    const Arc::Package importedSource = importer.resolveImport(pathToken.valueWithoutQuotes());

    return Arc::ASTNode::AnonymousNamespaceExpression(this->parseImport(importedSource)->body);
}

Arc::Expression Arc::ASTBuilder::parseAnonymousDeclarations(Arc::TokenStream& tokenStream) {
    while (Util::includes<std::string>(Arc::modifiers, tokenStream.at(0).value)) {
        const std::string name = tokenStream.next().value;
        this->collectedModifiers.push_back(Arc::ASTNode::Modifier(name));
    }

    Arc::Token token = tokenStream.next();
    while (token.kind == Arc::TokenType::Keyword) {
        if (token.value == "namespace") {
            return std::make_shared<Arc::ASTNode::AnonymousNamespaceExpression>(this->parseAnonymousNamespaceExpression(tokenStream));
        } else if (token.value == "foreach") {
            return std::make_shared<Arc::ASTNode::ForeachExpression>(this->parseForeachExpression(tokenStream));
        } else if (token.value == "while") {
            return std::make_shared<Arc::ASTNode::WhileExpression>(this->parseWhileExpression(tokenStream));
        } else if (token.value == "switch") {
            return std::make_shared<Arc::ASTNode::SwitchExpression>(this->parseSwitchExpression(tokenStream));
        } else if (token.value == "if") {
            return std::make_shared<Arc::ASTNode::IfExpression>(this->parseIfExpression(tokenStream));
        } else if (token.value == "class") {
            return std::make_shared<Arc::ASTNode::AnonymousClassExpression>(
                std::make_unique<Arc::ASTNode::ClassDeclaration>(this->parseClasslikeDeclaration(tokenStream, true, false))
            );
        } else if (token.value == "struct") {
            return std::make_shared<Arc::ASTNode::AnonymousStructExpression>(
                std::make_unique<Arc::ASTNode::StructDeclaration>(this->parseClasslikeDeclaration(tokenStream, true, false))
            );
        } else if (token.value == "var" || token.value == "const") {
            return std::make_shared<Arc::ASTNode::AnonymousVariableDeclaration>(
                std::make_unique<Arc::ASTNode::VariableDeclaration>(this->parseVariableDeclaration(token.value == "const", tokenStream))
            );
        } else if (token.value == "type") {
            return std::make_shared<Arc::ASTNode::AnonymousTypeDeclaration>(
                std::make_unique<Arc::ASTNode::TypeDeclaration>(this->parseTypeDeclaration(tokenStream))
            );
        } else if (token.value == "enum") {
            return std::make_shared<Arc::ASTNode::AnonymousEnumExpression>(
                std::make_unique<Arc::ASTNode::EnumDeclaration>(this->parseClasslikeDeclaration(tokenStream, true, true))
            );
        } else if (token.value == "fn") {
            return std::make_shared<Arc::ASTNode::AnonymousFunctionExpression>(
                std::make_unique<Arc::ASTNode::FunctionDeclaration>(this->parseFunctionDeclaration(tokenStream, true))
            );
        } else if (token.value == "try") {
            return std::make_shared<Arc::ASTNode::TryCatchExpression>(this->parseTryExpression(tokenStream));
        } else if (token.value == "forever") {
            return std::make_shared<Arc::ASTNode::ForeverExpression>(this->parseForeverExpression(tokenStream));
        } else if (token.value == "contract") {
            return std::make_shared<Arc::ASTNode::ContractExpression>(this->parseContractExpression(tokenStream));
        } else if (token.value == "functor") {
            return std::make_shared<Arc::ASTNode::FunctorExpression>(this->parseFunctionDeclaration(tokenStream, true));
        } else {
            token = tokenStream.next();
        }
    }

    return nullptr;
}

Arc::ASTNode::ContractExpression Arc::ASTBuilder::parseContractExpression(Arc::TokenStream& tokenStream) {
    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();
    const Arc::TokenStream contractBody = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBrace));
    Arc::TokenStream preConditionTokens;
    Arc::TokenStream postConditionTokens;
    Arc::TokenStream runConditionTokens;

    Arc::Token token = tokenStream.next();
    while (token.kind != Arc::TokenType::Eof) {
        if (token.kind == Arc::TokenType::Keyword) {
            if (token.value == "precondition" || token.value == "postcondition" || token.value == "runCondition") {
                tokenStream.expect(Arc::TokenType::AssignmentOperator, "=");
                const Arc::TokenStream conditionTokens = tokenStream.until(Arc::TokenType::Semicolon);

                if (token.value == "precondition") preConditionTokens = conditionTokens;
                else if (token.value == "precondition") preConditionTokens = conditionTokens;
                else if (token.value == "runcondition") runConditionTokens = conditionTokens;
            }
        }

        token = tokenStream.next();
    }

    return Arc::ASTNode::ContractExpression(
        modifiers,
        (preConditionTokens.empty()) ? std::nullopt : std::make_optional(this->parseExpression(preConditionTokens)),
        (postConditionTokens.empty()) ? std::nullopt : std::make_optional(this->parseExpression(postConditionTokens)),
        (runConditionTokens.empty()) ? std::nullopt : std::make_optional(this->parseExpression(runConditionTokens))
    );
}

Arc::ASTNode::AnonymousNamespaceExpression Arc::ASTBuilder::parseAnonymousNamespaceExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream bodyTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBrace));

    return Arc::ASTNode::AnonymousNamespaceExpression(this->toAst(bodyTokens));
}

Arc::ASTNode::ForeachExpression Arc::ASTBuilder::parseForeachExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream iteratorTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

    Arc::TokenStream bodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Eof);
    
    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::ForeachExpression(this->toAst(bodyTokens), this->parseIteratorExpression(iteratorTokens), modifiers);
}

Arc::ASTNode::WhileExpression Arc::ASTBuilder::parseWhileExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream conditionTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));
    
    Arc::TokenStream bodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);
    
    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    Arc::TokenStream elseBodyTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "else") {
        tokenStream.next();
        elseBodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
            ? tokenStream.getBody(tokenStream.next())
            : tokenStream.until(Arc::TokenType::Semicolon);
    }

    return Arc::ASTNode::WhileExpression (
        this->parseExpression(conditionTokens),
        this->toAst(bodyTokens),
        (!elseBodyTokens.empty()) ? std::make_optional(this->toAst(elseBodyTokens)) : std::nullopt,
        modifiers
    );
}

Arc::ASTNode::SwitchExpression Arc::ASTBuilder::parseSwitchExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream switchedValueTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));
    Arc::TokenStream bodyTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBrace));
    const std::vector<Arc::ASTNode::SwitchBlock> switchBody = this->parseSwitchBody(bodyTokens);

    std::vector<Arc::ASTNode::ValuedSwitchBlock> valueCases;
    std::optional<Arc::ASTNode::DefaultSwitchBlock> defaultCase;

    for (const auto& entry : switchBody) {
        if (entry->kind() == Arc::ASTNode::SwitchBlockKind::ValuedCase) {
            const auto switchCase = *std::dynamic_pointer_cast<Arc::ASTNode::ValuedSwitchBlock>(entry);
            valueCases.push_back(switchCase);
        } else {
            const auto switchCase = *std::dynamic_pointer_cast<Arc::ASTNode::DefaultSwitchBlock>(entry);
            defaultCase = switchCase;
        }
    }

    return Arc::ASTNode::SwitchExpression (
        this->parseExpression(switchedValueTokens),
        valueCases,
        defaultCase,
        this->flushModifiers()
    );
}

Arc::ASTNode::IfExpression Arc::ASTBuilder::parseIfExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream conditionTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

    Arc::TokenStream bodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until(Arc::TokenType::Semicolon);
    
    Arc::TokenStream elseBodyTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "else") {
        tokenStream.next();

        elseBodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
            ? tokenStream.getBody(tokenStream.next())
            : tokenStream.until(Arc::TokenType::Semicolon);
    }

    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::IfExpression (
        this->parseExpression(conditionTokens),
        this->toAst(bodyTokens),
        this->toAst(elseBodyTokens),
        modifiers
    );
}

Arc::ASTNode::TryCatchExpression Arc::ASTBuilder::parseTryExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream tryBodyTokens;
    Arc::TokenStream catchExprTokens;
    Arc::TokenStream catchBodyTokens;

    tryBodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
        ? tokenStream.getBody(tokenStream.next())
        : tokenStream.until({Arc::TokenType::Semicolon, Arc::TokenType::Keyword});

    if (tokenStream.at(0).kind == Arc::TokenType::Semicolon) {
        tokenStream.next();
    }

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "catch") {
        tokenStream.next();

        if (tokenStream.at(0).kind == Arc::TokenType::StartParenthesis) {
            catchExprTokens = tokenStream.getBody(tokenStream.next());
        }

        catchBodyTokens = (tokenStream.at(0).kind == Arc::TokenType::StartBrace)
            ? tokenStream.getBody(tokenStream.next())
            : tokenStream.until(Arc::TokenType::Semicolon);
    }

    return Arc::ASTNode::TryCatchExpression (
        this->parseExpression(tryBodyTokens),
        (!catchExprTokens.empty()) ? std::make_optional(this->parseCatchExpression(catchExprTokens)) : std::nullopt,
        (!catchBodyTokens.empty()) ? std::make_optional(this->toAst(catchBodyTokens)) : std::nullopt
    );
}

Arc::ASTNode::ForeverExpression Arc::ASTBuilder::parseForeverExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream bodyTokens = tokenStream.getBody(tokenStream.next());
    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::ForeverExpression (
        this->toAst(bodyTokens),
        modifiers
    );
}

Arc::Expression Arc::ASTBuilder::parseSpecialSymbol(Arc::TokenStream& tokenStream) {
    const std::string raw = tokenStream.stringify();
    tokenStream.next();
    const Arc::Token token = tokenStream.next();

    if (Util::includes<std::string, bool>(Arc::customSizedNativeValues, token.value)) {
        const Arc::Token nativeValueKind = tokenStream.next();
        const Arc::Token nativeValueSizeToken = tokenStream.expect(Arc::TokenType::Numeric);
        const Arc::Token nativeValueToken = tokenStream.expect(Arc::TokenType::Numeric);

        return std::make_shared<Arc::ASTNode::CustomSizedNativeLiteral>(
            Arc::customSizedNativeValues[nativeValueKind.value],
            static_cast<uint8_t>(std::stoul(nativeValueSizeToken.value)),
            nativeValueToken.value,
            raw
        );
    } else if (Arc::nativeValues.contains(token.value)) {
        const Arc::Token nativeValueToken = tokenStream.expect(Arc::TokenType::Numeric);

        return std::make_shared<Arc::ASTNode::NativeLiteral>(
            Arc::nativeValues[nativeValueToken.value],
            nativeValueToken.value,
            raw
        );
    } else if (token.value == "NativeArray") {
        const Arc::TokenStream valueTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

        return std::make_shared<Arc::ASTNode::NativeArrayLiteral>(
            raw,
            valueTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
                return this->parseExpression(tokenStream);
            })
        );
    } else if (token.value == "NativeFunction") {
        Arc::TokenStream parameterTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));
        Arc::TokenStream bodyTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBrace));

        return std::make_shared<Arc::ASTNode::NativeFunction>(
            this->toAst(bodyTokens),
            parameterTokens.split(Arc::TokenType::Colon).transform<Arc::ASTNode::Parameter>([this](Arc::TokenStream& tokenStream) {
                return this->parseParameter(tokenStream);
            })
        );
    } else if (token.value == "NativeCall") {
        const Arc::TokenStream argumentTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

        return std::make_shared<Arc::ASTNode::NativeCall>(
            this->parseExpression(tokenStream),
            argumentTokens.split(Arc::TokenType::Colon).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
                return this->parseExpression(tokenStream);
            })
        );
    } else if (token.value == "NativeWrite") {
        const Arc::ASTNode::StringLiteral message = *std::dynamic_pointer_cast<Arc::ASTNode::StringLiteral>(
            this->parseLiteral(
                tokenStream.expect(Arc::TokenType::String)
            )
        );

        return std::make_shared<Arc::ASTNode::NativeWriteExpression>(message);
    } else if (token.value == "NativeExit") {
        const Arc::ASTNode::NumericLiteral exitCode = *std::dynamic_pointer_cast<Arc::ASTNode::NumericLiteral>(
            this->parseLiteral(
                tokenStream.expect(Arc::TokenType::Numeric)
            )
        );

        return std::make_shared<Arc::ASTNode::NativeExit>(exitCode);
    } else {
        tokenStream.prependToken(token);
        this->decorators.push_back(this->parseDecorator(tokenStream));
        return this->parseExpression(tokenStream);
    }
    
    return nullptr;
}

Arc::ASTNode::ObjectLiteral Arc::ASTBuilder::parseObjectLiteral(Arc::TokenStream& tokenStream) {
    const Arc::TokenStream objectTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBrace));
    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();

    return Arc::ASTNode::ObjectLiteral (
        Arc::AbstractSyntaxTree(
            objectTokens.split(Arc::TokenType::Comma).transform<Arc::Statement>([this](Arc::TokenStream& tokenStream) -> Arc::Statement {
                const Arc::AbstractSyntaxTree parsedEntry = this->toAst(tokenStream);

                if (parsedEntry.size() == 1) {
                    return parsedEntry.at(0);
                } else {
                    Arc::ArcLogger::errln("Can only have one declaration per object literal entry.");
                    return nullptr;
                }
            })
        ),
        modifiers
    );
}

Arc::ASTNode::PipelineExpression Arc::ASTBuilder::parsePipelineExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream streamedTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

    return Arc::ASTNode::PipelineExpression (
        this->parseExpression(streamedTokens),
        tokenStream.split(Arc::TokenType::Pipeline).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        })
    );
}

Arc::ASTNode::ArrayAccessExpression Arc::ASTBuilder::parseArrayAccessExpression(Arc::TokenStream& tokenStream) {
    const Arc::TokenStream topLevelTokens = tokenStream.getTopLevelTokens();
    const Arc::Token accessStart = topLevelTokens.at(-2);
    Arc::TokenStream accesseeTokens;
    Arc::TokenStream keyTokens;

    Arc::Token next = tokenStream.next();
    while (next != accessStart) {
        accesseeTokens.addToken(next);
        next = tokenStream.next();
    }

    tokenStream.next();
    next = tokenStream.next();

    while (next.kind != Arc::TokenType::EndBracket) {
        keyTokens.addToken(next);
        next = tokenStream.next();
    }

    return Arc::ASTNode::ArrayAccessExpression(this->parseExpression(accesseeTokens), keyTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
        return this->parseExpression(tokenStream);
    }));
}

Arc::ASTNode::AnonymousFunctionExpression Arc::ASTBuilder::parseShorthandLambda(Arc::TokenStream& tokenStream) {
    const std::vector<Arc::ASTNode::Modifier> modifiers = this->flushModifiers();
    const std::vector<Arc::ASTNode::Decorator> decorators = this->flushDecorators();
    Arc::TokenStream fnBodyTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBrace));
    
    return Arc::ASTNode::AnonymousFunctionExpression (
        std::make_unique<Arc::ASTNode::FunctionDeclaration>(
            nullptr,
            Arc::ASTNode::Type(),
            std::vector<Arc::ASTNode::Generic>(),
            std::vector<Arc::Expression>(),
            modifiers,
            std::vector<Arc::ASTNode::Parameter>(),
            false,
            this->toAst(fnBodyTokens),
            decorators,
            std::vector<Arc::Expression>()
        )
    );
}

Arc::ASTNode::CallExpression Arc::ASTBuilder::parseCallExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream callSignatureTokens = tokenStream.until({Arc::TokenType::StartParenthesis, Arc::TokenType::BinaryOperator});
    Arc::TokenStream genericTokens;

    const bool isInferred = 
        callSignatureTokens.getTokens().size() == 1 &&
        callSignatureTokens.at(0).kind == Arc::TokenType::Keyword &&
        callSignatureTokens.at(0).value == "new";

    if (tokenStream.at(0).kind == Arc::TokenType::BinaryOperator && tokenStream.at(0).value == "<") {
        genericTokens = tokenStream.getBody(tokenStream.next());
    }
    
    const Arc::TokenStream argumentTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

    return Arc::ASTNode::CallExpression(
        argumentTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        }),
        (isInferred) ? nullptr : this->parseExpression(callSignatureTokens),
        genericTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Type>([this](Arc::TokenStream& tokenStream) {
            return this->parseType(tokenStream);
        })
    );
}

Arc::Literal Arc::ASTBuilder::parseLiteral(const Arc::Token& token) {
    switch (token.kind) {
        case Arc::TokenType::Numeric:
            return std::make_shared<Arc::ASTNode::NumericLiteral>(token.value);
        break;
        case Arc::TokenType::String: {
            const std::optional<Arc::ASTNode::InterpolatedStringLiteral> possibleInterpolatedStringLiteral = this->parseInterpolatedString(token.valueWithoutQuotes());
            if (possibleInterpolatedStringLiteral == std::nullopt) {
                return std::make_shared<Arc::ASTNode::StringLiteral>(token.value);
            } else {
                return std::make_shared<Arc::ASTNode::InterpolatedStringLiteral>(*possibleInterpolatedStringLiteral);
            }
        }
        case Arc::TokenType::Character:
            return std::make_shared<Arc::ASTNode::CharLiteral>(token.value);
        case Arc::TokenType::Identifier:
            return std::make_shared<Arc::ASTNode::IdentifierLiteral>(token.value);
        case Arc::TokenType::Keyword:
            if (token.value == "true" || token.value == "false") {
                return std::make_shared<Arc::ASTNode::BooleanLiteral>(token.value);
            } else if (token.value == "null") {
                return std::make_shared<Arc::ASTNode::NullLiteral>();
            } else if (token.value == "this") {
                return std::make_shared<Arc::ASTNode::ThisLiteral>();
            } else if (token.value == "macro") {
                return std::make_shared<Arc::ASTNode::MacroLiteral>();
            } else if (token.value == "pipe") {
                return std::make_shared<Arc::ASTNode::PipeLiteral>();
            } else if (token.value == "it") {
                return std::make_shared<Arc::ASTNode::ItLiteral>();
            } else if (token.value == "async") {
                return std::make_shared<Arc::ASTNode::AsyncLiteral>();
            } else if (token.value == "contract") {
                return std::make_shared<Arc::ASTNode::ContractLiteral>();
            } else if (token.value == "super") {
                return std::make_shared<Arc::ASTNode::SuperLiteral>();
            }
        default: return nullptr;
    }
}

Arc::ASTNode::IteratorExpression Arc::ASTBuilder::parseIteratorExpression(Arc::TokenStream& tokenStream) {
    const Arc::SplitTokenStream splitTokenStream = tokenStream.split(Arc::TokenType::Keyword, "in");
    Arc::TokenStream iteratorTokens;
    Arc::TokenStream iterableTokens;

    if (splitTokenStream.size() == 1) {
        iterableTokens = splitTokenStream.at(0);
    } else {
        iteratorTokens = splitTokenStream.at(0);
        iterableTokens = splitTokenStream.at(-1);
    }

    return Arc::ASTNode::IteratorExpression (
        (!iteratorTokens.empty()) ? std::make_optional(this->parseExpression(iteratorTokens)) : std::nullopt,
        this->parseExpression(iterableTokens)
    );
}

Arc::ASTNode::BinaryExpression Arc::ASTBuilder::parseBinaryExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream topLevelOperators;
    std::vector<Arc::TokenStream> genericTokens = {Arc::TokenStream()};
    bool openAngle = false;
    bool anglesPosedAsGenerics = false;

    for (const auto& token : tokenStream.getTopLevelTokens().getTokens()) {
        if (token.kind == Arc::TokenType::BinaryOperator) {
            topLevelOperators.addToken(token);
            
            if (token.value == "<") {
                openAngle = true;
            } else if (token.value == ">" && openAngle) {
                anglesPosedAsGenerics = true;
                genericTokens.back().addToken(token);
                genericTokens.push_back(Arc::TokenStream());
                openAngle = false;
            }

            if (openAngle) {
                genericTokens.back().addToken(token);
            }
        }
    }

    if (anglesPosedAsGenerics) {
        for (const auto& tkns : genericTokens) {
            if (tkns.at(-1).kind == Arc::TokenType::BinaryOperator && tkns.at(-1).value == ">") {
                if (!tkns.includes(Arc::TokenType::BinaryOperator)) {
                    const Arc::Token openAngle = tkns.at(0);
                    const Arc::Token closeAngle = tkns.at(-1);
                    for (int i = 0; i <= topLevelOperators.getTokens().size(); i++) {
                        if (topLevelOperators.at(i) == openAngle || topLevelOperators.at(i) == closeAngle) {
                            topLevelOperators.remove(i);
                            i = -1;
                        }
                    }
                }
            }
        }
    }

    std::vector<Arc::Token> topLevelOperatorTokens = topLevelOperators.getTokens();

    std::sort(topLevelOperatorTokens.begin(), topLevelOperatorTokens.end(), [](Arc::Token a, Arc::Token b) {
        return Arc::binaryOperatorPrecedences.at(a.value) > Arc::binaryOperatorPrecedences.at(b.value);
    });

    topLevelOperators = Arc::TokenStream::fromTokens(topLevelOperatorTokens);

    const Arc::Token highestPrecedenceToken = topLevelOperators.at(0);
    Arc::TokenStream lhs;
    Arc::TokenStream rhs;
    bool operatorFound = false;

    for (auto token : tokenStream.getTokens()) {
        if (token == highestPrecedenceToken) {
            operatorFound = true;
            continue;
        }
        if (operatorFound) {
            rhs.addToken(token);
        } else {
            lhs.addToken(token);
        }
    }

    return Arc::ASTNode::BinaryExpression (
        this->parseExpression(lhs),
        this->parseExpression(rhs),
        highestPrecedenceToken.value
    );
}

Arc::ASTNode::CatchExpression Arc::ASTBuilder::parseCatchExpression(Arc::TokenStream& tokenStream) {
    Arc::TokenStream errorTokens = tokenStream.until(Arc::TokenType::Keyword, "as", true);
    Arc::TokenStream aliasTokens;
    Arc::TokenStream conditionTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "as") {
        tokenStream.next();
        aliasTokens = tokenStream.until(Arc::TokenType::Keyword, "where", true);
    }

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "where") {
        tokenStream.next();
        conditionTokens = tokenStream;
    }

    return Arc::ASTNode::CatchExpression (
        this->parseExpression(errorTokens),
        (!aliasTokens.empty()) ? std::make_optional(this->parseExpression(aliasTokens)) : std::nullopt,
        (!conditionTokens.empty()) ? std::make_optional(this->parseExpression(conditionTokens)) : std::nullopt
    );
}