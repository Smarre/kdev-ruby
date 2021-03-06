/* This file is part of KDevelop
 *
 * Copyright (C) 2011-2015 Miquel Sabaté Solà <mikisabate@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <parser/astvisitor.h>
#include <parserdebug.h>

/*
 * Note that each visitor method has a comment that explains how the AST
 * is structured for the specific statement. Only the following attributes
 * will be considered: l, r, cond and ensure. If any of these pointers are not
 * specified, it means that its value is just nullptr.
 */

using namespace ruby;

AstVisitor::~AstVisitor()
{
}

void AstVisitor::visitCode(Ast *node)
{
    qCDebug(PARSER) << "Visiting Code...";
    Node *aux = node->tree;

    for (Node *n = aux; n; n = n->next) {
        visitNode(node);
        node->tree = n->next;
    }
    node->tree = aux;
}

void AstVisitor::visitName(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitNumeric(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitRegexp(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitString(Ast *node)
{
    /*
     * l -> list of variables contained inside the string (by using #{var}).
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitSymbol(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitBody(Ast *node)
{
    /*
     * l -> list of inner statements.
     * r -> optional rescue statement.
     * cond -> optional else statement.
     * ensure -> optional ensure statement.
     */

    Node *n = node->tree;
    if (!n) {
        return;
    }

    node->tree = n->l;
    visitStatements(node);
    node->tree = n->r;
    visitNode(node);
    node->tree = n->cond;
    visitNode(node);
    node->tree = n->ensure;
    visitNode(node);
    node->tree = n;
}

void AstVisitor::visitBinary(Ast *node)
{
    /*
     * l -> left operator.
     * r -> right operator.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitNode(node);
    node->tree = n->r;
    visitNode(node);
    node->tree = n;
}

void AstVisitor::visitBoolean(Ast *node)
{
    /* Same as for the visitBinary method  */

    Node *n = node->tree;
    node->tree = n->l;
    visitNode(node);
    node->tree = n->r;
    visitNode(node);
    node->tree = n;
}

void AstVisitor::visitRange(Ast *node)
{
    /* Same as for the visitBinary method  */

    Node *n = node->tree;
    node->tree = n->l;
    visitNode(node);
    node->tree = n->r;
    visitNode(node);
    node->tree = n;
}

void AstVisitor::visitUnary(Ast *node)
{
    /*
     * l -> the operator.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitNode(node);
    node->tree = n;
}

void AstVisitor::visitArray(Ast *node)
{
    /*
     * l -> list of statements (the items of the array).
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitArrayValue(Ast *node)
{
    /*
     * l -> the node containing the Array object.
     * r -> the index expression.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitNode(node);
    node->tree = n->r;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitHash(Ast *node)
{
    /*
     * l -> list of hash items.
     */

    Node *n = node->tree;
    node->tree = n->l;
    for (; node->tree != nullptr; node->tree = node->tree->next) {
        visitBinary(node);
    }
    node->tree = n;
}

void AstVisitor::visitReturnStatement(Ast *node)
{
    /*
     * l -> the return expression.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitNode(node);
    node->tree = n;
}

void AstVisitor::visitUndefStatement(Ast *node)
{
    /*
     * r -> list of undef items.
     */

    Node *n = node->tree;
    node->tree = n->r;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitAliasStatement(Ast *node)
{
    /* Same as for the visitBinary method. */

    Node *n = node->tree;
    node->tree = n->l;
    visitNode(node);
    node->tree = n->r;
    visitNode(node);
    node->tree = n;
}

void AstVisitor::visitYieldStatement(Ast *node)
{
    /*
     * l -> the yield expression.
     */
    Node *n = node->tree;
    node->tree = n->l;
    visitNode(node);
    node->tree = n;
}

void AstVisitor::visitAssignmentStatement(Ast *node)
{
    /*
     * l -> the left side of the assignment.
     * r -> the right side of the assignment.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitStatements(node);
    node->tree = n->r;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitIfStatement(Ast *node)
{
    /*
     * l -> list of inner statements.
     * r -> the "tail": optional list of elsif's and an optional else.
     * cond -> the condition expression.
     */

    Node *n = node->tree;
    node->tree = n->cond;
    visitNode(node);
    node->tree = n->l;
    visitStatements(node);
    node->tree = n->r;
    visitIfTail(node);
    node->tree = n;
}

void AstVisitor::visitCaseStatement(Ast *node)
{
    /*
     * l -> the case body: list of when statements and an optional else
     * statement at the end.
     * cond -> the condition expression.
     */

    Node *n = node->tree;
    node->tree = n->cond;
    visitNode(node);
    node->tree = n->l;
    visitWhenStatements(node);
    node->tree = n;
}

void AstVisitor::visitBeginStatement(Ast *node)
{
    /*
     * l -> the body of the begin statement. Note that this body is not just
     * a list of "regular" statement, check the visitBody method for more info.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitBody(node);
    node->tree = n;
}

void AstVisitor::visitUpBeginEndStatement(Ast *node)
{
    /*
     * l -> list of inner statements.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitWhileStatement(Ast *node)
{
    /*
     * l -> list of inner statements.
     * cond -> the condition expression.
     */

    Node *n = node->tree;
    node->tree = n->cond;
    visitNode(node);
    node->tree = n->l;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitForStatement(Ast *node)
{
    /*
     * l -> list of inner statements.
     * r -> list of variables declared before the "in" keyword.
     * cond -> the expression after the "in" keyword.
     */

    Node *n = node->tree;
    node->tree = n->r;
    visitStatements(node);
    node->tree = n->cond;
    visitNode(node);
    node->tree = n->l;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitMethodStatement(Ast *node)
{
    /*
     * l -> list of inner statements.
     * r -> list of method arguments.
     */

    Node *n = node->tree;
    if (!n) {
        return;
    }

    node->tree = n->r;
    visitMethodArguments(node);
    node->tree = n->l;
    visitBody(node);
    node->tree = n;
}

void AstVisitor::visitMethodArguments(Ast *node)
{
    /* Just iterate over the "next" pointer. */

    Node *aux = node->tree;
    for (Node *n = aux; n != nullptr; n = n->next) {
        visitParameter(node);
        node->tree = n->next;
    }
    node->tree = aux;
}

void AstVisitor::visitParameter(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitClassStatement(Ast *node)
{
    /*
     * l -> the body of the class statement. Note that this body is not just
     * a list of "regular" statement, check the visitBody method for more info.
     * r -> the name of the class.
     * cond -> the superclass node.
     */

    Node *n = node->tree;
    node->tree = n->cond;
    visitNode(node);
    node->tree = n->l;
    visitBody(node);
    node->tree = n->r;
    visitClassName(node);
    node->tree = n;
}

void AstVisitor::visitSingletonClass(Ast *node)
{
    /*
     * l -> the body of the class statement. Note that this body is not just
     * a list of "regular" statement, check the visitBody method for more info.
     * r -> the expression after the lshift ("<<") operator.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitBody(node);
    node->tree = n->r;
    visitNode(node);
    node->tree = n;
}

void AstVisitor::visitModuleStatement(Ast *node)
{
    /*
     * l -> the body of the class statement. Note that this body is not just
     * a list of "regular" statement, check the visitBody method for more info.
     * r -> the name of the module.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitBody(node);
    node->tree = n->r;
    visitClassName(node);
    node->tree = n;
}

void AstVisitor::visitMethodCall(Ast *node)
{
    /*
     * l -> the caller (note that it's a list, i.e. A::B::foo).
     * r -> the arguments for this method call.
     * cond -> an optional Ruby block.
     */

    /*
     * Note that the l pointer can contain again another method call.
     * This happens for example here:
     *  Class.new { def foo(a, b); end }.new.foo(1, 2)
     * In order to get everything straight, all builders end up
     * re-implementing this method. This is why this method has no
     * implementation by default.
     */
    Q_UNUSED(node);
}

void AstVisitor::visitSuper(Ast *node)
{
    /*
     * r -> the arguments passed to the super call.
     */

    Node *n = node->tree;
    node->tree = n->r;
    for (Node *aux = n->r; aux != nullptr; aux = aux->next) {
        visitNode(node);
        node->tree = aux->next;
    }
    node->tree = n;
}

void AstVisitor::visitLambda(Ast *node)
{
    /*
     * cond -> the block of this lambda expression.
     */

    Node *n = node->tree;
    node->tree = n->cond;
    visitBlock(node);
    node->tree = n;
}

void AstVisitor::visitBlock(Ast *node)
{
    /*
     * l -> list of inner statements.
     * r -> list of block variables.
     */

    Node *aux = node->tree;
    if (!aux) {
        return;
    }
    node->tree = aux->r;
    visitBlockVariables(node);
    node->tree = aux->l;
    visitStatements(node);
    node->tree = aux;
}

void AstVisitor::visitBlockVariables(Ast *node)
{
    /*
     * Just iterate over the next pointer.
     */

    Node *aux = node->tree;
    for (Node *n = node->tree; n != nullptr; n = n->next) {
        visitNode(node);
        node->tree = n->next;
    }
    node->tree = aux;
}

void AstVisitor::visitRequire(Ast *node, bool relative)
{
    Q_UNUSED(node)
    Q_UNUSED(relative)
}

void AstVisitor::visitMixin(Ast *node, bool include)
{
    Q_UNUSED(node);
    Q_UNUSED(include);
}

void AstVisitor::visitDefined(Ast *node)
{
    /*
     * l -> the expression from the "defined" statement.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitNode(node);
    node->tree = n;
}

void AstVisitor::visitTrue(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitFalse(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitNil(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitFile(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitLine(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitEncoding(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitSelf(Ast *node)
{
    Q_UNUSED(node)
}

void AstVisitor::visitAccessSpecifier(const access_t policy)
{
    Q_UNUSED(policy)
}

void AstVisitor::visitClassName(Ast *node)
{
    Q_UNUSED(node);
}

void AstVisitor::visitRescue(Ast *node)
{
    /*
     * l -> rescue arg.
     * r -> list of inner statement.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitNode(node);
    node->tree = n->r;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitRescueArg(Ast *node)
{
    /*
     * l -> Left part of the rescue argument, could be a list.
     * r -> Right part of the rescue argument, only the DeclarationBuilder
     * wants to access this part.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitEnsure(Ast *node)
{
    /*
     * l -> The inner statements.
     */

    Node *n = node->tree;
    node->tree = n->l;
    visitStatements(node);
    node->tree = n;
}

void AstVisitor::visitNode(Ast *node)
{
    QByteArray name;
    Node *n = node->tree;

    /* This is not a valid node */
    if (!n || n->kind == token_invalid) {
        return;
    }
    switch (n->kind) {
        case token_return: visitReturnStatement(node); break;
        case token_yield: visitYieldStatement(node); break;
        case token_alias: visitAliasStatement(node); break;
        case token_undef: visitUndefStatement(node); break;
        case token_if: case token_unless: case token_ternary:
          visitIfStatement(node);
          break;
        case token_begin: visitBeginStatement(node); break;
        case token_up_begin:
        case token_up_end: visitUpBeginEndStatement(node); break;
        case token_case: visitCaseStatement(node); break;
        case token_while: case token_until: visitWhileStatement(node); break;
        case token_for: visitForStatement(node); break;
        case token_class: visitClassStatement(node); break;
        case token_singleton_class: visitSingletonClass(node); break;
        case token_module: visitModuleStatement(node); break;
        case token_function: visitMethodStatement(node); break;
        case token_super: visitSuper(node); break;
        case token_method_call: checkMethodCall(node); break;
        case token_assign:
        case token_op_assign: visitAssignmentStatement(node); break;
        case token_object:
            name = QByteArray(node->tree->name);
            if (name == "public") {
                if (declaredInContext(name)) {
                    visitName(node);
                } else {
                    visitAccessSpecifier(public_a);
                }
            } else if (name == "protected") {
                if (declaredInContext(name)) {
                    visitName(node);
                } else {
                    visitAccessSpecifier(protected_a);
                }
            } else if (name == "private") {
                if (declaredInContext(name)) {
                    visitName(node);
                } else {
                    visitAccessSpecifier(private_a);
                }
            } else {
                visitName(node);
            }
            break;
        case token_hash: visitHash(node); break;
        case token_array: visitArray(node); break;
        case token_array_value: visitArrayValue(node); break;
        case token_defined: visitDefined(node); break;
        case token_unary_plus: case token_unary_minus: case token_neg:
        case token_not:
            visitUnary(node);
            break;
        case token_plus: case token_minus: case token_mul: case token_div:
        case token_mod: case token_lshift: case token_rshift: case token_pow:
            visitBinary(node);
            break;
        case token_dot2: case token_dot3:
            visitRange(node);
            break;
        case token_cmp: case token_eq: case token_eqq: case token_match:
        case token_nmatch: case token_greater: case  token_geq:
        case token_lesser:  case token_leq:
        case token_or: case token_and: case token_kw_and:
        case token_kw_not: case token_kw_or:
            visitBoolean(node);
            break;
        case token_numeric: visitNumeric(node); break;
        case token_string: case token_heredoc: visitString(node); break;
        case token_regexp: visitRegexp(node); break;
        case token_nil: visitNil(node); break;
        case token_true: visitTrue(node); break;
        case token_false: visitFalse(node); break;
        case token_line: visitLine(node); break;
        case token_file: visitFile(node); break;
        case token_encoding: visitEncoding(node); break;
        case token_self: visitSelf(node); break;
        case token_symbol: case token_key: visitSymbol(node); break;
        case token_rescue: visitRescue(node); break;
        case token_rescue_arg: visitRescueArg(node); break;
        case token_ensure: visitEnsure(node); break;
        case token_break: case token__end__: case token_next:
        case token_redo: case token_retry:
            return;
    }
}

void AstVisitor::visitStatements(Ast *list)
{
    Node *aux = list->tree;
    for (Node *n = aux; n; n = n->next) {
        visitNode(list);
        list->tree = n->next;
    }
    list->tree = aux;
}

void AstVisitor::visitIfTail(Ast *tail)
{
    Node *n = tail->tree;
    if (!n) {
        return;
    }

    /* Check if this is an elsif or an else statement */
    if (!n->cond) {
        tail->tree = n->l;
        visitStatements(tail);
    } else {
        visitIfStatement(tail);
    }
    tail->tree = n;
}

void AstVisitor::visitWhenStatements(Ast *list)
{
    Node *n = list->tree;
    if (!n) {
        return;
    }

    /* Check whether this is a when or an else statement */
    if (n->kind == token_when) {
        list->tree = n->cond;
        visitStatements(list);
        list->tree = n->l;
        visitStatements(list);
        list->tree = n->r;
        visitWhenStatements(list);
    } else {
        list->tree = n->l;
        visitStatements(list);
    }
    list->tree = n;
}

void AstVisitor::checkMethodCall(Ast *mc)
{
    /*
     * The method call body resides in the left child. Check if this
     * is either a require, an include/extend or just a normal method call.
     * If the left child is nullptr, this is not a method call but a lambda
     * expression.
     */
    if (mc->tree->l) {
        const QByteArray &name = QByteArray(mc->tree->l->name);
        if (name == "require") {
            (declaredInContext(name)) ? visitMethodCall(mc) : visitRequire(mc);
        } else if (name == "include") {
            (declaredInContext(name)) ? visitMethodCall(mc) : visitMixin(mc, true);
        } else if (name == "extend") {
            (declaredInContext(name)) ? visitMethodCall(mc) : visitMixin(mc, false);
        } else if (name == "require_relative") {
            (declaredInContext(name)) ? visitMethodCall(mc) : visitRequire(mc, true);
        } else if (name == "lambda") {
            (declaredInContext(name)) ? visitMethodCall(mc) : visitLambda(mc);
        } else {
            visitMethodCall(mc);
        }
    } else {
        visitLambda(mc);
    }
}

