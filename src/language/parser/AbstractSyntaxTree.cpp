#include "Parser.h"

Arc::Statement Arc::AbstractSyntaxTree::at(const int index) const {
    return (index >= 0)
        ? this->astStream[index]
        : this->astStream[this->astStream.size() + index];
}

std::vector<Arc::Statement> Arc::AbstractSyntaxTree::getASTStream() const {
    return this->astStream;
}

bool Arc::AbstractSyntaxTree::empty() const {
    return this->astStream.empty();
}

size_t Arc::AbstractSyntaxTree::size() const {
    return this->astStream.size();
}

Arc::TraversableAST Arc::AbstractSyntaxTree::traverse() const {
    return Arc::TraversableAST(*this);
}

Arc::ASTMatcher Arc::AbstractSyntaxTree::matcher() const {
    return Arc::ASTMatcher(*this);
}