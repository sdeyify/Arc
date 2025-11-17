#include "Parser.h"
#include "../util/imports.h"

Arc::TraversableAST& Arc::TraversableAST::doFn(const std::function<Arc::ASTIterationExitKind(const Arc::ASTIterator&)>& fn) {
    this->commands.push_back(Arc::ASTIterationCommand(fn));

    return *this;
}

Arc::TraversableAST& Arc::TraversableAST::doIf(
    const std::function<bool(const Arc::ASTIterator&)>& condition,
    const std::function<Arc::ASTIterationExitKind(const Arc::ASTIterator&)>& fn
) {
    this->commands.push_back(Arc::ASTIterationCommand(fn, condition));

    return *this;
}

Arc::TraversableAST& Arc::TraversableAST::ifOfKind(
    const Arc::StatementKind& kind,
    const std::function<Arc::ASTIterationExitKind(const Arc::ASTIterator&)>& fn
) {
    this->commands.push_back(Arc::ASTIterationCommand(fn, [kind](const Arc::ASTIterator& iterator) {
        return iterator.stmt->kind() == kind;
    }));

    return *this;
}

void Arc::TraversableAST::traverse() {
    size_t index = 0;
    for (const Arc::Statement& stmt : this->ast.getASTStream()) {
        for (const Arc::ASTIterationCommand& cmd : this->commands) {
            if (cmd.condition(Arc::ASTIterator(stmt, index))) {
                const Arc::ASTIterationExitKind fnExitResult = cmd.fn(Arc::ASTIterator(stmt, index));

                switch (fnExitResult) {
                    case Arc::ASTIterationExitKind::BreakExit:
                        goto traversalEnd;
                    case Arc::ASTIterationExitKind::ContinueExit:
                        goto nextStmt;
                    case Arc::ASTIterationExitKind::NextConditionExit:
                        continue;
                    case Arc::ASTIterationExitKind::VisitChildrenIfBodied:
                        if (stmt->hasBody()) {
                            const auto bodiedStmt = std::dynamic_pointer_cast<Arc::ASTNode::ASTBodiedStatement>(stmt);
                            Arc::TraversableAST traversableBody = Arc::AbstractSyntaxTree(bodiedStmt->body).traverse();
                            
                            for (const Arc::ASTIterationCommand& command : this->commands) {
                                traversableBody.commands.push_back(command);
                            }

                            traversableBody.traverse();
                        }
                        continue;
                }
            }
        }
    nextStmt:
        index++;
    }

traversalEnd:
    return;
}

Arc::TraversableAST Arc::TraversableAST::copyFor(const Arc::AbstractSyntaxTree& otherAst) const {
    Arc::TraversableAST otherTraverser = otherAst.traverse();
    
    for (const Arc::ASTIterationCommand& command : this->commands) {
        otherTraverser.commands.push_back(command);
    }

    return std::move(otherTraverser);
}