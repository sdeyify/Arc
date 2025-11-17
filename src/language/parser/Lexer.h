#pragma once
#include "../util/Util.h"
#include "nodes/ASTNode.h"
#include "Token.h"
#include "TokenType.h"
#include "TokenStream.h"

namespace Arc
{
	class Lexer
	{
	public:
		Lexer(const std::string& src) : src(src) {}
	private:
		const std::string& src;
	public:
		TokenStream parse() const;
	private:
		TokenStream insertSemicolons(const TokenStream& tokens) const;
		TokenType getTokenType(const std::string& value) const;
	};
}