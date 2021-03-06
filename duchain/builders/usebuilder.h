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

#ifndef RUBY_USE_BUILDER_H
#define RUBY_USE_BUILDER_H

#include <language/duchain/builders/abstractusebuilder.h>

#include <duchain/builders/contextbuilder.h>
#include <duchain/duchainexport.h>

namespace ruby {

using UseBuilderBase =
    KDevelop::AbstractUseBuilder<Ast, NameAst, ContextBuilder>;

/**
 * @class UseBuilder
 *
 * A class which iterates the AST to extract uses of definitions.
 */
class KDEVRUBYDUCHAIN_EXPORT UseBuilder : public UseBuilderBase
{
public:
    explicit UseBuilder(EditorIntegrator *editor);

protected:
    /// Methods re-implemented from AstVisitor.

    bool declaredInContext(const QByteArray &name) const override;

    void visitName(Ast *node) override;
    void visitClassName(Ast *node) override;
    void visitMixin(Ast *node, bool include) override;
    void visitMethodCall(Ast *node) override;
    void visitRequire(Ast *node, bool relative = false) override;

private:
    /// @internal Visit the method call members from the given @p node.
    void visitMethodCallMembers(Ast *node);

private:
    /// Used at the method call visitor to keep track of the last context.
    KDevelop::DUContext *m_lastCtx;

    /**
     * The method call visitor uses this to track the depth of
     * the recursion level.
     */
    int m_depth;

    /**
     * Used in the method call to tell if the previous recursion level
     * consisted of a class/module.
     */
    bool m_classMethod;
};

}

#endif // RUBY_USE_BUILDER_H
