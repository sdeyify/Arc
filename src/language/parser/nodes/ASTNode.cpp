#include "statements/ASTBodiedStatement.h"
#include "statements/ASTNamedStatement.h"
#include "statements/ASTModifiableStatement.h"
#include "expressions/ASTExpression.h"
#include "expressions/ASTBodiedExpression.h"
#include "expressions/literals/ASTLiteral.h"
#include "../AbstractSyntaxTree.h"
#include "Modifier.h"
#include "../../util/imports.h"

ASTNode::ASTBodiedStatement::ASTBodiedStatement() : body(Arc::AbstractSyntaxTree()) {}
ASTNode::ASTModifiableStatement::ASTModifiableStatement() : modifiers(std::vector<Modifier>()) {}
ASTNode::ASTNamedStatement::ASTNamedStatement() : signature(std::shared_ptr<ASTExpression>()) {}
ASTNode::ASTBodiedExpression::ASTBodiedExpression() : body(Arc::AbstractSyntaxTree()) {}
ASTNode::ASTLiteral::ASTLiteral() : raw("") {}