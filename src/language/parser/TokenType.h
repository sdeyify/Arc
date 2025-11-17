#pragma once
#include "../util/Util.h"

namespace Arc
{
	enum class TokenType
	{
		Numeric,
		Keyword,
		Identifier,
		String,
		Character,
		BinaryOperator,
		UnaryOperator,
		AssignmentOperator,
		StartParenthesis,
		EndParenthesis,
		StartBracket,
		EndBracket,
		StartBrace,
		EndBrace,
		Colon,
		Comma,
		Dot,
		AtSign,
		Hash,
		Semicolon,
		QuestionMark,
		Tilde,
		Pipeline,
		ReturnArrow,
		FunctionArrow,
		Symbol,
		None,
		Eof
	};

	inline std::string stringifyTokenType(const Arc::TokenType& tokenType) {
		switch (tokenType) {
			case Arc::TokenType::Numeric: return "Numeric";
			case Arc::TokenType::Keyword: return "Keyword";
			case Arc::TokenType::Identifier: return "Identifier";
			case Arc::TokenType::String: return "String";
			case Arc::TokenType::Character: return "Character";
			case Arc::TokenType::BinaryOperator: return "BinaryOperator";
			case Arc::TokenType::UnaryOperator: return "UnaryOperator";
			case Arc::TokenType::AssignmentOperator: return "AssignmentOperator";
			case Arc::TokenType::StartParenthesis: return "StartParenthesis";
			case Arc::TokenType::EndParenthesis: return "EndParenthesis";
			case Arc::TokenType::StartBracket: return "StartBracket";
			case Arc::TokenType::EndBracket: return "EndBracket";
			case Arc::TokenType::StartBrace: return "StartBrace";
			case Arc::TokenType::EndBrace: return "EndBrace";
			case Arc::TokenType::Colon: return "Colon";
			case Arc::TokenType::Comma: return "Comma";
			case Arc::TokenType::Dot: return "Dot";
			case Arc::TokenType::AtSign: return "AtSign";
			case Arc::TokenType::Hash: return "Hash";
			case Arc::TokenType::Semicolon: return "Semicolon";
			case Arc::TokenType::QuestionMark: return "QuestionMark";
			case Arc::TokenType::Tilde: return "Tilde";
			case Arc::TokenType::Pipeline: return "Pipeline";
			case Arc::TokenType::ReturnArrow: return "ReturnArrow";
			case Arc::TokenType::FunctionArrow: return "FunctionArrow";
			case Arc::TokenType::Symbol: return "Symbol";
			case Arc::TokenType::None: return "None";
			case Arc::TokenType::Eof: return "Eof";
			default: return "None";
		}
	}
}