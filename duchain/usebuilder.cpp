/* This file is part of KDevelop
 *
 * Copyright (C) 2011  Miquel Sabaté <mikisabate@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <duchain/usebuilder.h>
#include <rubydefs.h> // TODO: remove this ;)
#include "helpers.h"
#include "editorintegrator.h"
#include <KLocale>


namespace Ruby
{

UseBuilder::UseBuilder(EditorIntegrator *editor) : UseBuilderBase()
{
    m_editor = editor;
}

// TODO
void UseBuilder::visitVariable(RubyAst* node)
{
    NameAst *name = new NameAst(node);
    const RangeInRevision &range = editorFindRange(node, node);
    const QualifiedIdentifier &id = identifierForNode(name);
    const KDevelop::Declaration *decl = declarationForNode(id, range, DUContextPointer(currentContext()));

    if (!decl) {
        KDevelop::Problem *p = new KDevelop::Problem();
        p->setFinalLocation(DocumentRange(m_editor->url(), range.castToSimpleRange()));
        p->setSource(KDevelop::ProblemData::SemanticAnalysis);
        p->setSeverity(KDevelop::ProblemData::Hint);
        p->setDescription(i18n("Undefined variable: %1", name->value));
        {
            DUChainWriteLocker wlock(DUChain::lock());
            ProblemPointer ptr(p);
            topContext()->addProblem(ptr);
        }
    } else if (decl->range() == range) {
        delete name;
        return;
    }
    /// TODO: clean this
    debug() << "UseBuilder: Visiting variable: " << node->tree->name;
//     UseBuilderBase::newUse(node, range, DeclarationPointer(decl));
    delete name;
}

} // End of namespace Ruby

