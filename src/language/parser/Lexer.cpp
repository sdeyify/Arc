#include "../util/Util.h"
#include "Parser.h"

Arc::TokenStream Arc::Lexer::parse() const {
	Arc::TokenStream tokenStream;
	std::string builded;
	Arc::LexingCategory lexingCategory = Arc::LexingCategory::Unset;
	size_t line = 1;
	size_t col = 0;
	size_t idx = 0;
	char nextChar = NULL;
    int buffer = 0;

	for (const char letter : this->src) {
		idx++;
		col++;
		builded += letter;

		if (buffer > 0) {
			buffer--;
			continue;
		}

		if (idx < this->src.size()) {
			nextChar = this->src[idx];
		} else {
			nextChar = NULL;
		}

		if (lexingCategory == Arc::LexingCategory::Unset) {
			if (letter == '\'') {
				lexingCategory = Arc::LexingCategory::Character;
			} else if (letter == '"') {
				lexingCategory = Arc::LexingCategory::String;
			} else if (Util::NUMERIC.contains(letter)) {
				lexingCategory = Arc::LexingCategory::Numeric;
			} else if (nextChar != '\0' && letter == '/' && nextChar == '/') {
				lexingCategory = Arc::LexingCategory::InlineComment;
			} else if (nextChar != '\0' && letter == '/' && nextChar == '*') {
				lexingCategory = Arc::LexingCategory::InlineComment;
			} else if (Util::ALPHA.contains(letter)) {
				lexingCategory = Arc::LexingCategory::Alphanumeric;
			} else if (letter == '\n') {
				col = 0;
				line++;
			} else if (Util::WHITESPACE.contains(letter)) {
				builded.pop_back();
			} else {
				lexingCategory = Arc::LexingCategory::Symbol;
			}
		}

		switch (lexingCategory) {
			case Arc::LexingCategory::Character:
				if (nextChar == NULL || letter == '\'') {
					tokenStream.addToken(
						Arc::Token(
							builded,
							Arc::TokenType::Character,
							line,
							col
						)
					);
					builded.clear();
				}
				break;

			case Arc::LexingCategory::String:
				if (nextChar == NULL || letter == '"') {
					tokenStream.addToken(
						Arc::Token(
							builded,
							Arc::TokenType::String,
							line,
							col
						)
					);
					builded.clear();
				}
				break;
			
			case Arc::LexingCategory::Numeric:
				if (nextChar != NULL && Util::ALPHA.contains(nextChar)) {
					lexingCategory = Arc::LexingCategory::Alphanumeric;
				} else if (nextChar == NULL || !Util::NUMERIC.contains(nextChar)) {
					std::cout << letter << std::endl;
					lexingCategory = Arc::LexingCategory::Unset;
					tokenStream.addToken(
						Arc::Token(
							builded,
							Arc::TokenType::Numeric,
							line,
							col
						)
					);
					builded.clear();
				}
				break;
			
			case Arc::LexingCategory::InlineComment:
				if (nextChar == NULL || nextChar == '\n') {
					builded.clear();
					lexingCategory = Arc::LexingCategory::Unset;
				}
				break;

			case Arc::LexingCategory::MultilineComment:
				if (nextChar == NULL || (letter == '*' && nextChar == '/')) {
					builded.clear();
					lexingCategory = Arc::LexingCategory::Unset;
					buffer = 1;
				}
				break;
			
			case Arc::LexingCategory::Alphanumeric:
				if (nextChar == NULL || !Util::ALPHANUM.contains(nextChar)) {
					tokenStream.addToken(
						Arc::Token(
							builded,
							(Util::includes<std::string>(Arc::keywords, builded)) ? Arc::TokenType::Keyword : Arc::TokenType::Identifier,
							line,
							col
						)
					);
					lexingCategory = Arc::LexingCategory::Unset;
					builded.clear();
				}
				break;

			case Arc::LexingCategory::Symbol:
				if (Util::ALPHANUM.contains(nextChar) || Util::WHITESPACE.contains(nextChar) || nextChar == NULL) {
					tokenStream.addToken(
						Arc::Token(
							builded,
							this->getTokenType(builded),
							line,
							col
						)
					);
					lexingCategory = Arc::LexingCategory::Unset;
					builded.clear();
				}
				break;
		}
	}

	tokenStream.addToken(Token::eof());

	return this->insertSemicolons(tokenStream);
};

Arc::TokenType Arc::Lexer::getTokenType(const std::string& value) const {
	if (Util::includes<std::string>(Arc::binaryOperators, value)) {
		return Arc::TokenType::BinaryOperator;
	} else if (Util::includes<std::string>(Arc::unaryOperators, value)) {
		return Arc::TokenType::UnaryOperator;
	} else if (Util::includes<std::string>(Arc::assignmentOperators, value)) {
		return Arc::TokenType::AssignmentOperator;
	} else if (Arc::symbolTokenTypes.contains(value)) {
		return symbolTokenTypes.at(value);
	}
	return Arc::TokenType::Symbol;
}

Arc::TokenStream Arc::Lexer::insertSemicolons(const Arc::TokenStream& tokens) const {
	Arc::TokenStream semicolonInsertedTokens;
	const std::vector<Arc::TokenType> omitBefore = {
		Arc::TokenType::BinaryOperator, Arc::TokenType::AssignmentOperator, Arc::TokenType::StartBracket, Arc::TokenType::StartBrace, Arc::TokenType::StartParenthesis,
		Arc::TokenType::Semicolon, Arc::TokenType::Dot, Arc::TokenType::Pipeline, Arc::TokenType::AtSign, Arc::TokenType::ReturnArrow
	};

	Arc::Token lastToken;
	for (const auto& token : tokens.getTokens()) {
		if (
			lastToken.kind == Arc::TokenType::EndBrace &&
			(!Util::includes<Arc::TokenType>(omitBefore, token.kind) && !(token.kind == Arc::TokenType::Keyword && token.value == "else"))
		) {
			semicolonInsertedTokens.addToken(
				Arc::Token(";", Arc::TokenType::Semicolon, token.line, token.col + 1)
			);
		}

		semicolonInsertedTokens.addToken(token);

		lastToken = token;
	}

	return semicolonInsertedTokens;
}