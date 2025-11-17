#pragma once
#include "../util/imports.h"
#include "TokenType.h"

namespace Arc
{
    const inline std::vector<std::string> keywords = {
		"macro", "if", "while", "when", "true", "false", "null", "var", "const", "fn", "async", "global", "export", "stepped", "public", "catch", "try",
		"private", "protected", "native", "default", "strict", "interface", "enum", "class", "foreach", "in", "type", "else", "extends", "implements", "new",
		"finally", "span", "named", "switch", "case", "default", "namespace", "abstract", "where", "return", "break", "continue", "do", "pattern", "anyobj",
		"is", "static", "with", "this", "pipe", "preload", "label", "goto", "extension", "decorator", "error", "it", "throws", "get", "set", "init", "unit",
		"override", "ref", "forever", "pure", "any", "typeof", "circref", "import", "circrefdecl", "sealed", "unsealed", "functor", "functorlike", "satisfies",
		"contract", "precondition", "postcondition", "runcondition", "constfunc", "opaque", "transparent", "super", "struct", "throw", "friendof"
	};

	const inline std::vector<std::string> binaryOperators = {
		"!=", "+", "-", "*", "/", "%", "**", ">>", "<<", ">>>", "<", ">", ">=", "<=", "instanceof", "==", "&", "^", "|", "&&", "||", "::", "??", "?.", "?::",
		".", "..."
	};

	const inline std::vector<std::string> unaryOperators = {
		"!", "++", "--"
	};

	const inline std::vector<std::string> assignmentOperators = {
		"=", "+=", "-=", "*=", "/=", "%=", ">>=", "<<=", "?=",
	};

	const inline std::map<std::string, TokenType> symbolTokenTypes = {
		{"(", TokenType::StartParenthesis},
		{")", TokenType::EndParenthesis},
		{"{", TokenType::StartBrace},
		{"}", TokenType::EndBrace},
		{"[", TokenType::StartBracket},
		{"]", TokenType::EndBracket},
		{":", TokenType::Colon},
		{",", TokenType::Comma},
		{".", TokenType::Dot},
		{"@", TokenType::AtSign},
		{"#", TokenType::Hash},
		{";", TokenType::Semicolon},
		{"?", TokenType::QuestionMark},
		{"|>", TokenType::Pipeline},
		{"->", TokenType::ReturnArrow},
		{"=>", TokenType::FunctionArrow},
		{"~", TokenType::Tilde}
	};
}