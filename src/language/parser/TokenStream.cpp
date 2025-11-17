#include "Parser.h"
#include "../util/Util.h"
#include "../messages/Messages.h"

std::vector<Arc::Token> Arc::TokenStream::getTokens() const {
    return this->tokens;
}

bool Arc::TokenStream::empty() const {
    return this->tokens.empty();
}

void Arc::TokenStream::addToken(const Arc::Token& token) {
    this->tokens.push_back(token);
}

void Arc::TokenStream::prependToken(const Arc::Token& token) {
    this->tokens.insert(this->tokens.begin(), token);
}

Arc::TokenStream Arc::TokenStream::concat(const Arc::TokenStream& tokenStream) const {
    Arc::TokenStream concatenatedTokenStream;

    for (const auto& token : this->tokens) {
        concatenatedTokenStream.addToken(token);
    }

    for (const auto& token : tokenStream.getTokens()) {
        concatenatedTokenStream.addToken(token);
    }

    return concatenatedTokenStream;
}

Arc::Token Arc::TokenStream::next() {
    if (this->tokens.empty()) {
        return Token("\0", Arc::TokenType::Eof, 0, 0);
    }
    const Arc::Token nextToken = this->tokens[0];
    this->tokens.erase(this->tokens.begin());
    return nextToken;
}

Arc::Token Arc::TokenStream::at(const int index) const {
    return (index >= 0)
        ? this->tokens[index]
        : this->tokens[this->tokens.size() + index];
}

void Arc::TokenStream::remove(const int index) {
    this->tokens.erase(
        (index >= 0)
        ? this->tokens.begin() + index
        : this->tokens.end() - index + 1
    );
}

std::string Arc::TokenStream::stringify() const {
    std::string stringified;

    for (const auto& token : this->tokens) {
        stringified += token.value;
    }

    return stringified;
}

size_t Arc::TokenStream::size() const {
    return this->tokens.size();
}

Arc::TokenStream Arc::TokenStream::getTopLevelTokens() const {
    Arc::TokenStream tokenStream;
    std::stack<std::string> stack;

    for (const auto& token : this->tokens) {
        if (stack.empty()) {
            if (Arc::TokenStream::scopeBracketPairs.contains(token.value)) {
                stack.push(Arc::TokenStream::scopeBracketPairs[token.value]);
            }

            tokenStream.addToken(token);
        } else {
            if (stack.top() == token.value) {
                stack.pop();

                if (stack.empty()) {
                    tokenStream.addToken(token);
                }
            }
        }
    }

    return tokenStream;
}

Arc::TokenStream Arc::TokenStream::fromTokens(const std::vector<Arc::Token>& tokens) {
    Arc::TokenStream tokenStream;

    for (const auto& token : tokens) {
        tokenStream.addToken(token);
    }

    return tokenStream;
}

bool Arc::TokenStream::includes(const Arc::TokenType& kind) const {
    for (const auto& token : this->tokens) {
        if (token.kind == kind) {
            return true;
        }
    }

    return false;
}

bool Arc::TokenStream::includes(const Arc::TokenType& kind, const std::string& value) const {
    for (const auto& token : this->tokens) {
        if (token.kind == kind && token.value == value) {
            return true;
        }
    }

    return false;
}

Arc::TokenStream Arc::TokenStream::getBody(const Arc::Token& token) {
    Arc::TokenStream tokenStream;
    Arc::Token nextToken = this->next();

    std::map<Arc::TokenType, Arc::TokenType> scopeTokenTypePairs = {
        {Arc::TokenType::StartParenthesis, Arc::TokenType::StartParenthesis},
        {Arc::TokenType::StartBrace, Arc::TokenType::StartBrace},
        {Arc::TokenType::StartBracket, Arc::TokenType::StartBracket},
    };

    while (nextToken != token) {
        nextToken = this->next();
    }

    if (nextToken.kind == Arc::TokenType::BinaryOperator && nextToken.value == "<") {
        tokenStream.concat(this->until(Arc::TokenType::BinaryOperator, ">"));
    } else if (scopeTokenTypePairs.contains(nextToken.kind)) {
        tokenStream.concat(this->until(scopeTokenTypePairs[nextToken.kind]));
    } else {
        Arc::ArcLogger::errln("Did not find StartParenthesis `(` or StartBrace `{` or StartBracket `[` or StartAngle `<` in body");
    }

    return tokenStream;
}

Arc::SplitTokenStream Arc::TokenStream::split(const Arc::TokenType& kind) const {
    std::vector<Arc::TokenStream> splitTokens = {Arc::TokenStream()};
    std::stack<std::string> stack;

    for (const auto& token : this->tokens) {
        if (stack.empty()) {
            if (token.kind == kind) {
                splitTokens.push_back(Arc::TokenStream());
                continue;
            } else if (Arc::TokenStream::scopeBracketPairs.contains(token.value)) {
                stack.push(Arc::TokenStream::scopeBracketPairs[token.value]);
            }
        } else {
            if (token.value == stack.top()) {
                stack.pop();
            }
        }

        splitTokens.back().addToken(token);
    }

    return Arc::SplitTokenStream(splitTokens);
}

Arc::SplitTokenStream Arc::TokenStream::split(const Arc::TokenType& kind, const std::string& value) const {
    std::vector<Arc::TokenStream> splitTokens = {Arc::TokenStream()};
    std::stack<std::string> stack;

    for (const auto& token : this->tokens) {
        if (stack.empty()) {
            if (token.kind == kind && token.value == value) {
                splitTokens.push_back(Arc::TokenStream());
                continue;
            } else if (Arc::TokenStream::scopeBracketPairs.contains(token.value)) {
                stack.push(Arc::TokenStream::scopeBracketPairs[token.value]);
            }
        } else {
            if (token.value == stack.top()) {
                stack.pop();
            }
        }

        splitTokens.back().addToken(token);
    }

    return Arc::SplitTokenStream(splitTokens);
}

Arc::SplitTokenStream Arc::TokenStream::split(const std::vector<Arc::TokenType>& kinds) const {
    std::vector<Arc::TokenStream> splitTokens = {Arc::TokenStream()};
    std::stack<std::string> stack;

    for (const auto& token : this->tokens) {
        if (stack.empty()) {
            if (Util::includes<Arc::TokenType>(kinds, token.kind)) {
                splitTokens.push_back(Arc::TokenStream());
                continue;
            } else if (Arc::TokenStream::scopeBracketPairs.contains(token.value)) {
                stack.push(Arc::TokenStream::scopeBracketPairs[token.value]);
            }
        } else {
            if (token.value == stack.top()) {
                stack.pop();
            }
        }

        splitTokens.back().addToken(token);
    }

    return Arc::SplitTokenStream(splitTokens);
}

Arc::Token Arc::TokenStream::expect(const Arc::TokenType& kind) {
    if (this->tokens[0].kind == kind) {
        return this->next();
    } else {
        Arc::ArcLogger::errln(std::format("Expected `{0}` but found `{1}`", Arc::stringifyTokenType(kind), Arc::stringifyTokenType(this->tokens[0].kind)));
        std::unreachable();
    }
}

Arc::Token Arc::TokenStream::expect(const Arc::TokenType& kind, const std::string& value) {
    if (this->tokens[0].kind == kind) {
        return this->next();
    } else {
        Arc::ArcLogger::errln(std::format("Expected `{0} {1}` but found `{2}`", Arc::stringifyTokenType(kind), value, Arc::stringifyTokenType(this->tokens[0].kind)));
        std::unreachable();
    }
}

Arc::Token Arc::TokenStream::expect(const std::vector<Arc::TokenType>& kinds) {
    if (Util::includes<Arc::TokenType>(kinds, this->tokens[0].kind)) {
        return this->next();
    } else {
        std::string stringifiedKinds = "";

        for (const auto& kind : kinds) {
            stringifiedKinds += " or ";
            stringifiedKinds += Arc::stringifyTokenType(kind);
        }

        stringifiedKinds = stringifiedKinds.substr(4);

        Arc::ArcLogger::errln(std::format("Expected {0}, but found {1}", stringifiedKinds, Arc::stringifyTokenType(this->tokens[0].kind)));
        std::unreachable();
    }
}

Arc::TokenStream Arc::TokenStream::until(const Arc::TokenType& kind, const bool prependLast) {
    Arc::TokenStream tokenStream;
    std::stack<std::string> stack;
    Arc::Token nextToken = this->next();

    while (!this->tokens.empty()) {
        if (stack.empty()) {
            if (nextToken.kind == kind) {
                if (prependLast) {
                    this->prependToken(nextToken);
                }
                break;
            } else if (Arc::TokenStream::scopeBracketPairs.contains(nextToken.value)) {
                stack.push(Arc::TokenStream::scopeBracketPairs[nextToken.value]);
            }
        } else {
            if (nextToken.value == stack.top()) {
                stack.pop();
            }
        }

        tokenStream.addToken(nextToken);
    }

    return tokenStream;
}

Arc::TokenStream Arc::TokenStream::until(const Arc::TokenType& kind, const std::string& value, const bool prependLast) {
    Arc::TokenStream tokenStream;
    std::stack<std::string> stack;
    Arc::Token nextToken = this->next();

    while (!this->tokens.empty()) {
        if (stack.empty()) {
            if (nextToken.kind == kind && nextToken.value == value) {
                if (prependLast) {
                    this->prependToken(nextToken);
                }
                break;
            } else if (Arc::TokenStream::scopeBracketPairs.contains(nextToken.value)) {
                stack.push(Arc::TokenStream::scopeBracketPairs[nextToken.value]);
            }
        } else {
            if (nextToken.value == stack.top()) {
                stack.pop();
            }
        }

        tokenStream.addToken(nextToken);
    }

    return tokenStream;
}

Arc::TokenStream Arc::TokenStream::until(const std::vector<Arc::TokenType>& kinds, const bool prependLast) {
    Arc::TokenStream tokenStream;
    std::stack<std::string> stack;
    Arc::Token nextToken = this->next();

    while (!this->tokens.empty()) {
        if (stack.empty()) {
            if (Util::includes<Arc::TokenType>(kinds, nextToken.kind)) {
                if (prependLast) {
                    this->prependToken(nextToken);
                }
                break;
            } else if (Arc::TokenStream::scopeBracketPairs.contains(nextToken.value)) {
                stack.push(Arc::TokenStream::scopeBracketPairs[nextToken.value]);
            }
        } else {
            if (nextToken.value == stack.top()) {
                stack.pop();
            }
        }

        tokenStream.addToken(nextToken);
    }

    return tokenStream;
}