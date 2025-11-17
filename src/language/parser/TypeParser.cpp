#include "Parser.h"
#include "../util/Util.h"

Arc::Type Arc::ASTBuilder::parseType(Arc::TokenStream& tokenStream) {
    const Arc::TokenStream topLevelTokens = tokenStream.getTopLevelTokens();

    if (topLevelTokens.stringify() == "()") {
        Arc::TokenStream typeTokens = tokenStream.getBody(tokenStream.next());
        return this->parseType(typeTokens);
    } else if (topLevelTokens.includes(Arc::TokenType::BinaryOperator, "|")) {
        return std::make_shared<Arc::ASTNode::UnionType>(this->parseUnionType(tokenStream));
    } else if (topLevelTokens.includes(Arc::TokenType::BinaryOperator, "&")) {
        return std::make_shared<Arc::ASTNode::MergedType>(this->parseMergedType(tokenStream));
    } else if (topLevelTokens.includes(Arc::TokenType::Dot)) {
        return std::make_shared<Arc::ASTNode::MemberType>(this->parseMemberType(tokenStream));
    } else if (topLevelTokens.stringify() == "[]") {
        return std::make_shared<Arc::ASTNode::TupleType>(this->parseTupleType(tokenStream));
    } else if (topLevelTokens.includes(Arc::TokenType::FunctionArrow)) {
        return std::make_shared<Arc::ASTNode::FunctionType>(this->parseFunctionType(tokenStream));
    } else if (topLevelTokens.at(0).kind == Arc::TokenType::Keyword) {
        const Arc::Token token = tokenStream.next();

        if (token.value == "any") {
            return std::make_shared<Arc::ASTNode::AnyType>(this->parseAnyType(tokenStream));
        } else if (token.value == "typeof") {
            return std::make_shared<Arc::ASTNode::TypeofType>(this->parseTypeofType(tokenStream));
        } else if (token.value == "anyobj") {
            return std::make_shared<Arc::ASTNode::ObjectType>(this->parseObjectType(tokenStream));
        } else if (token.value == "unit") {
            return std::make_shared<Arc::ASTNode::UnitType>();
        } else if (token.value == "infer") {
            return std::make_shared<Arc::ASTNode::InferType>();
        }
    } else if (topLevelTokens.includes(Arc::TokenType::QuestionMark)) {
        return std::make_shared<Arc::ASTNode::OptionalType>(this->parseOptionalType(tokenStream));
    } else if (topLevelTokens.includes(Arc::TokenType::Keyword, "throws")) {
        return std::make_shared<Arc::ASTNode::ThrowType>(this->parseThrowType(tokenStream));
    } else if (tokenStream.at(0).kind == Arc::TokenType::AtSign) {
        tokenStream.next();
        const Arc::Token& token = tokenStream.expect(Arc::TokenType::Identifier);

        if (token.value == "NativeFunction") {
            return std::make_shared<Arc::ASTNode::NativeFunctionType>();
        } else if (token.value == "NativeArray") {
            return std::make_shared<Arc::ASTNode::NativeArrayType>(this->parseNativeArrayType(tokenStream));
        } else if (Util::includes(Arc::nativeValues, token.value)) {
            return std::make_shared<Arc::ASTNode::NativeValueType>(Arc::nativeValues[token.value]);
        }
    } else {
        return std::make_shared<Arc::ASTNode::BasicType>(this->parseBasicType(tokenStream));
    }

    return nullptr;
}

Arc::ASTNode::NativeArrayType Arc::ASTBuilder::parseNativeArrayType(Arc::TokenStream& tokenStream) {
    return Arc::ASTNode::NativeArrayType(
        tokenStream
            .getBody(tokenStream.expect(Arc::TokenType::StartParenthesis))
            .split(Arc::TokenType::Comma)
            .transform<Arc::Type>([this](Arc::TokenStream& tokenStream) {
                return this->parseType(tokenStream);
            })
    );
}

Arc::ASTNode::OptionalType Arc::ASTBuilder::parseOptionalType(Arc::TokenStream& tokenStream) {
    tokenStream.remove(-1);

    return Arc::ASTNode::OptionalType(
        this->parseType(tokenStream)
    );
}

Arc::ASTNode::ThrowType Arc::ASTBuilder::parseThrowType(Arc::TokenStream& tokenStream) {
    Arc::TokenStream typeTokens;
    Arc::TokenStream errorTokens;
    bool foundThrows = false;

    for (const Arc::Token& token : tokenStream.getTokens()) {
        if (foundThrows) {
            errorTokens.addToken(token);
        } else {
            if (token.kind == Arc::TokenType::Keyword && token.value == "throws") {
                foundThrows = true;
                continue;
            } else {
                typeTokens.addToken(token);
            }
        }
    }

    return Arc::ASTNode::ThrowType(
        this->parseType(typeTokens),

        errorTokens
            .getBody(errorTokens.expect(Arc::TokenType::BinaryOperator, "<"))
            .split(Arc::TokenType::Comma)
            .transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
                return this->parseExpression(tokenStream);
            })
    );
}

Arc::ASTNode::MemberType Arc::ASTBuilder::parseMemberType(Arc::TokenStream& tokenStream) {
    Arc::SplitTokenStream splitTokenStream = tokenStream.split(Arc::TokenType::Dot);
    Arc::TokenStream ownerTokens = splitTokenStream.next();
    Arc::TokenStream memberTokens = splitTokenStream.join();
    
    return Arc::ASTNode::MemberType(
        this->parseExpression(ownerTokens),
        this->parseType(memberTokens)
    );
}

Arc::ASTNode::AnyType Arc::ASTBuilder::parseAnyType(Arc::TokenStream& tokenStream) {
    bool hasWhereCondition = false;

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "where") {
        tokenStream.next();
        hasWhereCondition = true;
    }

    return Arc::ASTNode::AnyType(
        (hasWhereCondition && !tokenStream.empty()) ? std::make_optional(this->parseExpression(tokenStream)) : std::nullopt
    );
}

Arc::ASTNode::ObjectType Arc::ASTBuilder::parseObjectType(Arc::TokenStream& tokenStream) {
    bool hasWhereCondition = false;

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "where") {
        tokenStream.next();
        hasWhereCondition = true;
    }

    return Arc::ASTNode::ObjectType(
        (hasWhereCondition && !tokenStream.empty()) ? std::make_optional(this->parseExpression(tokenStream)) : std::nullopt
    );
}

Arc::ASTNode::BasicType Arc::ASTBuilder::parseBasicType(Arc::TokenStream& tokenStream) {
    Arc::TokenStream name = tokenStream.until(Arc::TokenType::BinaryOperator, "<");
    Arc::TokenStream genericTokens;
    Arc::TokenStream parameterTokens;
    Arc::TokenStream throwErrorSignatureTokens;
    Arc::TokenStream whereConditionTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::BinaryOperator && tokenStream.at(0).value == "<") {
        genericTokens = tokenStream.getBody(tokenStream.next());
    }

    if (tokenStream.at(0).kind == Arc::TokenType::StartParenthesis) {
        parameterTokens = tokenStream.getBody(tokenStream.next());
    }

    if (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "where") {
        tokenStream.next();
        whereConditionTokens = tokenStream;
    }

    return Arc::ASTNode::BasicType(
        this->parseExpression(name),
        genericTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Type>([this](Arc::TokenStream& tokenStream) {
            return this->parseType(tokenStream);
        }),
        parameterTokens.split(Arc::TokenType::Comma).transform<Arc::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        }),
        (whereConditionTokens.empty()) ? std::nullopt : std::make_optional(this->parseExpression(whereConditionTokens))
    );
}

Arc::ASTNode::MergedType Arc::ASTBuilder::parseMergedType(Arc::TokenStream& tokenStream) {
    const Arc::TokenStream typeTokens = tokenStream.until(Arc::TokenType::Keyword, "where", true);
    bool hasWhereCondition = tokenStream.empty() || (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "where");

    return Arc::ASTNode::MergedType(
        typeTokens.split(Arc::TokenType::BinaryOperator, "|").transform<Arc::Type>([this](Arc::TokenStream& tokenStream) {
            return this->parseType(tokenStream);
        }),
        (hasWhereCondition) ? std::make_optional(this->parseExpression(tokenStream)) : std::nullopt
    );
}

Arc::ASTNode::TypeofType Arc::ASTBuilder::parseTypeofType(Arc::TokenStream& tokenStream) {
    Arc::TokenStream expressionTokens = tokenStream.until(Arc::TokenType::Keyword, "where", true);
    const bool hasWhereCondition = tokenStream.empty() || (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "where");

    return Arc::ASTNode::TypeofType(
        this->parseExpression(expressionTokens),
        (hasWhereCondition) ? std::make_optional(this->parseExpression(tokenStream)) : std::nullopt
    );
}

Arc::ASTNode::UnionType Arc::ASTBuilder::parseUnionType(Arc::TokenStream& tokenStream) {
    const Arc::TokenStream typeTokens = tokenStream.until(Arc::TokenType::Keyword, "where", true);
    bool hasWhereCondition = tokenStream.empty() || (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "where");

    return Arc::ASTNode::UnionType(
        typeTokens.split(Arc::TokenType::BinaryOperator, "&").transform<Arc::Type>([this](Arc::TokenStream& tokenStream) {
            return this->parseType(tokenStream);
        }),
        (hasWhereCondition) ? std::make_optional(this->parseExpression(tokenStream)) : std::nullopt
    );
}

Arc::ASTNode::TupleType Arc::ASTBuilder::parseTupleType(Arc::TokenStream& tokenStream) {
    const Arc::TokenStream typeTokens = tokenStream.until(Arc::TokenType::Keyword, "where", true);
    bool hasWhereCondition = tokenStream.empty() || (tokenStream.at(0).kind == Arc::TokenType::Keyword && tokenStream.at(0).value == "where");

    return Arc::ASTNode::TupleType(
        typeTokens.split(Arc::TokenType::Comma).transform<Arc::Type>([this](Arc::TokenStream& tokenStream) {
            return this->parseType(tokenStream);
        }),
        (hasWhereCondition) ? std::make_optional(this->parseExpression(tokenStream)) : std::nullopt
    );
}

Arc::ASTNode::FunctionType Arc::ASTBuilder::parseFunctionType(Arc::TokenStream& tokenStream) {
    Arc::TokenStream selectedSymbolTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::StartBracket) {
        selectedSymbolTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartBracket));
    }

    const Arc::TokenStream parameterTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::StartParenthesis));

    Arc::TokenStream genericTokens;

    if (tokenStream.at(0).kind == Arc::TokenType::BinaryOperator && tokenStream.at(0).value == "<") {
        genericTokens = tokenStream.getBody(tokenStream.expect(Arc::TokenType::BinaryOperator));
    }

    tokenStream.expect(Arc::TokenType::FunctionArrow);

    return Arc::ASTNode::FunctionType(
        genericTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Generic>([this](Arc::TokenStream& tokenStream) {
            return this->parseGeneric(tokenStream);
        }),
        parameterTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Parameter>([this](Arc::TokenStream& tokenStream) {
            return this->parseParameter(tokenStream);
        }),
        selectedSymbolTokens.split(Arc::TokenType::Comma).transform<Arc::ASTNode::Expression>([this](Arc::TokenStream& tokenStream) {
            return this->parseExpression(tokenStream);
        }),
        this->parseType(tokenStream)
    );
}