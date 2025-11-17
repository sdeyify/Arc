#pragma once
#include "Token.h"
#include "TokenType.h"
#include "../util/imports.h"
#include "SplitTokenStream.h"

namespace Arc
{
    class TokenStream
    {
    private:
        std::vector<Arc::Token> tokens;

        static inline std::map<std::string, std::string> scopeBracketPairs = {
            {"(", ")"},
            {"{", "}"},
            {"[", "]"},
            {"<", ">"}
        };
    public:
        std::vector<Arc::Token> getTokens() const;
        void addToken(const Arc::Token& token);
        void prependToken(const Arc::Token& token);
        Arc::TokenStream concat(const Arc::TokenStream& tokenStream) const;
        Arc::Token next();
        Arc::Token at(const int index) const;
        void remove(const int index);
        std::string stringify() const;
        Arc::TokenStream getTopLevelTokens() const;
        bool empty() const;
        size_t size() const;

        static Arc::TokenStream fromTokens(const std::vector<Arc::Token>& tokens);

        bool includes(const Arc::TokenType& kind) const;
        bool includes(const Arc::TokenType& kind, const std::string& value) const;
        
        template<typename T>
        std::vector<T> map(std::function<T(Arc::TokenStream)> fn) {
            std::vector<T> mapped;

            for (const auto& token : this->tokens) {
                mapped.push_back(fn(token));
            }

            return mapped;
        }

        Arc::TokenStream getBody(const Arc::Token& token);

        Arc::SplitTokenStream split(const Arc::TokenType& kind) const;
        Arc::SplitTokenStream split(const Arc::TokenType& kind, const std::string& value) const;
        Arc::SplitTokenStream split(const std::vector<Arc::TokenType>& kinds) const;

        Arc::Token expect(const Arc::TokenType& kind);
        Arc::Token expect(const Arc::TokenType& kind, const std::string& value);
        Arc::Token expect(const std::vector<Arc::TokenType>& kinds);

        Arc::TokenStream until(const Arc::TokenType& kind, const bool prependLast = false);
        Arc::TokenStream until(const Arc::TokenType& kind, const std::string& value, const bool prependLast = false);
        Arc::TokenStream until(const std::vector<Arc::TokenType>& kinds, const bool prependLast = false);
    };
}