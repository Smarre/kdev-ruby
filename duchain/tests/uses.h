/*
 * This file is part of KDevelop
 * Copyright (C) 2012-2015 Miquel Sabaté Solà <mikisabate@gmail.com>
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


#ifndef RUBY_USES_H
#define RUBY_USES_H


#include <duchain/tests/duchaintestbase.h>


namespace ruby {

/**
 * @class TestUseBuilder
 * Test the UseBuilder class from the DUChain.
 */
class TestUseBuilder : public DUChainTestBase
{
    Q_OBJECT

public:
    TestUseBuilder();

protected:
    /// Re-implemented from DUChainTestBase.
    KDevelop::TopDUContext * parse(const QByteArray &code, const QString &id) override;

private:
    /// Compare the uses of the given declaration @p dec with the given @p range.
    void compareUses(KDevelop::Declaration *dec, const KDevelop::RangeInRevision &range);

    /**
     * Let's compare the uses of the given declaration @p dec with the given
     * @p ranges that we expect these uses are enclosed to.
     */
    void compareUses(KDevelop::Declaration *dec, QList<KDevelop::RangeInRevision> ranges);

private slots:
    // Basic stuff
    void stringInterpolation();
    void alias();
    void assignment();
    void checkSubClassing();
    void instanceVariable();
    void classVariable();
    void exceptions();

    // Contexts
    void block();
    void checkMethodArgumentsContext();
    void checkMethodLocalDeclarations();
    void instanceClassMethods();
    void globals1();
    void globals2();
    void globals3();
    void defaultGlobals();
    void classModulesScopes();

    // Method calls
    void builtinUses();
    void chained();
    void fromClassAndAbove();
    void super();
    void moduleMixins1();
    void moduleMixins2();
    void exprIsCalling();
    void stringCalling();

    // Others
    void nestedIdentifier();
};

}

#endif /* RUBY_USES_H */
