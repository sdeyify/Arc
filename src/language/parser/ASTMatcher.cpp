#include "Parser.h"
#include "../util/imports.h"

std::optional<Arc::Statement> Arc::ASTMatcher::matchFirst(const std::function<bool(const Arc::ASTIterator& iterator)>& fn, const bool checkChildren) const {
    size_t index = 0;

    for (const Arc::Statement& stmt : this->astStream) {
        if (fn(Arc::ASTIterator(stmt, index))) {
            return std::make_optional(stmt);
        }

        if (stmt->hasBody() && checkChildren) {
            const auto bodiedStmt = std::dynamic_pointer_cast<Arc::ASTNode::ASTBodiedStatement>(stmt);
            const Arc::ASTMatcher childBodyMatcher = bodiedStmt->body.matcher();
            return childBodyMatcher.matchFirst(fn, true);
        }

        index++;
    }

    return std::nullopt;
}

std::vector<Arc::Statement> Arc::ASTMatcher::matchAll(const std::function<bool(const Arc::ASTIterator& iterator)>& fn, const bool checkChildren) const {
    std::vector<Arc::Statement> stmts;

    size_t index = 0;

    for (const Arc::Statement& stmt : this->astStream) {
        if (fn(Arc::ASTIterator(stmt, index))) {
            stmts.push_back(stmt);
        }

        if (stmt->hasBody() && checkChildren) {
            const auto bodiedStmt = std::dynamic_pointer_cast<Arc::ASTNode::ASTBodiedStatement>(stmt);
            const Arc::ASTMatcher childBodyMatcher = bodiedStmt->body.matcher();
            const std::vector<Arc::Statement> childBodyMatches = childBodyMatcher.matchAll(fn, true);

            stmts.insert(
                stmts.end(),
                childBodyMatches.begin(),
                childBodyMatches.end()
            );
        }

        index++;
    }

    return stmts;
}