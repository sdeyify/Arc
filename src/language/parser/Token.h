#pragma once
#include "../util/Util.h"
#include "TokenType.h"

namespace Arc
{
    struct Token
    {
        Token(const std::string& value, TokenType kind, size_t line, size_t col) : value(value), kind(kind), line(line), col(col) {}
        Token() : value(""), kind(TokenType::None), line(NULL), col(NULL) {}

        std::string value;
        TokenType kind;
        size_t line;
        size_t col;

        static Token eof() {
            return Token("", TokenType::Eof, NULL, NULL);
        }

        std::string toString() {
            return this->value;
        }

        std::string valueWithoutQuotes() const {
            if (this->kind == TokenType::Character || this->kind == TokenType::String) {
                return this->value.substr(1, this->value.length() - 2);
            } else {
                return this->value;
            }
        }

        bool operator==(Token other) {
            return (
                this->col == other.col &&
                this->line == other.line &&
                this->value == other.value &&
                this->kind == other.kind
            );
        }

        bool operator!=(Token other) {
            return (
                this->col != other.col ||
                this->line != other.line ||
                this->value != other.value ||
                this->kind != other.kind
            );
        }
    };
}