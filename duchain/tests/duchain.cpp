/*
 * This file is part of KDevelop
 * Copyright 2012  Miquel Sabaté <mikisabate@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


// Qt + KDevelop
#include <QtTest/QtTest>
#include <language/duchain/duchain.h>
#include <language/duchain/declaration.h>
#include <language/duchain/types/integraltype.h>
#include <language/duchain/types/structuretype.h>
#include <language/duchain/types/unsuretype.h>
#include <language/duchain/types/functiontype.h>
#include <language/duchain/duchainutils.h>

// Ruby
#include <duchain/declarations/methoddeclaration.h>
#include <duchain/tests/duchain.h>


QTEST_MAIN(Ruby::TestDUChain)

using namespace KDevelop;
namespace Ruby
{

TestDUChain::TestDUChain()
{
    /* There's nothing to do here */
}

TopDUContext * TestDUChain::parse(const QByteArray &code, const QString &id)
{
    const QString &name = "duchain_" + id;
    return DUChainTestBase::parse(code, name);
}

void TestDUChain::testUnsureTypes(TypePtr<UnsureType> type, QList<QString> list)
{
    for (uint i = 0; i < type->typesSize(); i++) {
        QualifiedIdentifier qi = type->types()[i].type<StructureType>()->qualifiedIdentifier();
        QCOMPARE(qi, QualifiedIdentifier(list.at(i)));
    }
}

//BEGIN: Builtin classes

void TestDUChain::numeric()
{
    QByteArray code("a = 1; b = 1.2");
    TopDUContext *top = parse(code, "numeric");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    Declaration *dec1 = top->localDeclarations().at(0);
    QVERIFY(dec1->type<StructureType>());
    QCOMPARE(dec1->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));

    Declaration *dec2 = top->localDeclarations().at(1);
    QVERIFY(dec2->type<StructureType>());
    QCOMPARE(dec2->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Float"));
}

void TestDUChain::range()
{
    QByteArray code("a = 1..42");
    TopDUContext *top = parse(code, "range");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    Declaration *dec1 = top->localDeclarations().at(0);
    QVERIFY(dec1->type<StructureType>());
    QCOMPARE(dec1->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Range"));
}

void TestDUChain::stringAndRegexp()
{
    QByteArray code("a = 'string'; b = //");
    TopDUContext *top = parse(code, "stringAndRegexp");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    QVERIFY(top->localDeclarations().size() == 2);

    /* a = 'string' */
    Declaration *dec1 = top->localDeclarations().at(0);
    QVERIFY(dec1->type<StructureType>());
    QCOMPARE(dec1->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("String"));

    /* b = // */
    Declaration *dec2 = top->localDeclarations().at(1);
    QVERIFY(dec2->type<StructureType>());
    QCOMPARE(dec2->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Regexp"));
}

void TestDUChain::booleanAndNilAndSelf()
{
    QByteArray code("a = true; b = false; c = nil; d = self");
    TopDUContext *top = parse(code, "booleanAndNilAndSelf");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    QVERIFY(top->localDeclarations().size() == 4);

    /* a = true */
    Declaration *dec1 = top->localDeclarations().at(0);
    QVERIFY(dec1->type<StructureType>());
    QCOMPARE(dec1->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("TrueClass"));

    /* b = false */
    Declaration *dec2 = top->localDeclarations().at(1);
    QVERIFY(dec2->type<StructureType>());
    QCOMPARE(dec2->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("FalseClass"));

    /* c = nil */
    Declaration *dec3 = top->localDeclarations().at(2);
    QVERIFY(dec3->type<StructureType>());
    QCOMPARE(dec3->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("NilClass"));

    /* d = self */
    Declaration *dec4 = top->localDeclarations().at(3);
    QVERIFY(dec4->type<StructureType>());
    QCOMPARE(dec4->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Object"));
}

void TestDUChain::lineFileEncoding()
{
    QByteArray code("a = __LINE__; b = __FILE__; c = __ENCODING__");
    TopDUContext *top = parse(code, "lineFileEncoding");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    QVERIFY(top->localDeclarations().size() == 3);

    /* a = __LINE__ */
    Declaration *dec1 = top->localDeclarations().at(0);
    QVERIFY(dec1->type<StructureType>());
    QCOMPARE(dec1->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));

    /* b = __FILE__ */
    Declaration *dec2 = top->localDeclarations().at(1);
    QVERIFY(dec2->type<StructureType>());
    QCOMPARE(dec2->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("String"));

    /* c = __ENCODING__ */
    Declaration *dec3 = top->localDeclarations().at(2);
    QVERIFY(dec3->type<StructureType>());
    QCOMPARE(dec3->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Encoding"));
}

void TestDUChain::symbol()
{
    QByteArray code("a = :a");
    TopDUContext *top = parse(code, "symbol");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    Declaration *dec = top->localDeclarations().at(0);
    QVERIFY(dec->type<StructureType>());
    QCOMPARE(dec->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Symbol"));
}

void TestDUChain::lambda()
{
    QByteArray code("a = ->(a) { puts a }");
    TopDUContext *top = parse(code, "lambda");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    Declaration *dec = top->localDeclarations().at(0);
    QVERIFY(dec->type<StructureType>());
    QCOMPARE(dec->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Proc"));
}

//END: Builtin classes

//BEGIN: Simple Statements

void TestDUChain::alias()
{
    QByteArray code("def foo; end; alias asd foo; alias asd foo");
    TopDUContext *top = parse(code, "alias");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    /* def foo; end */
    Declaration *dec1 = top->localDeclarations().at(0);
    QVERIFY(dec1->isFunctionDeclaration());
    QCOMPARE(dec1->qualifiedIdentifier(), QualifiedIdentifier("foo"));

    /* alias asd foo */
    Declaration *dec2 = top->localDeclarations().at(1);
    QVERIFY(dec2->isFunctionDeclaration());
    QCOMPARE(dec2->qualifiedIdentifier(), QualifiedIdentifier("asd"));
}

void TestDUChain::aliasGlobal1()
{
    QByteArray code("$a = 0; alias $b $a");
    TopDUContext *top = parse(code, "aliasGlobal1");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    QVERIFY(top->localDeclarations().size() == 2);

    Declaration *dec1 = top->localDeclarations().at(0);
    Declaration *dec2 = top->localDeclarations().at(1);

    QCOMPARE(dec1->qualifiedIdentifier(), QualifiedIdentifier("$a"));
    QCOMPARE(dec1->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));
    QCOMPARE(dec2->qualifiedIdentifier(), QualifiedIdentifier("$b"));
    QCOMPARE(dec2->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));
}

void TestDUChain::aliasGlobal2()
{
    QByteArray code("alias $b $a");
    TopDUContext *top = parse(code, "aliasGlobal2");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    QVERIFY(top->localDeclarations().size() == 2);

    Declaration *dec1 = top->localDeclarations().at(0);
    Declaration *dec2 = top->localDeclarations().at(1);

    QCOMPARE(dec1->qualifiedIdentifier(), QualifiedIdentifier("$b"));
    QCOMPARE(dec1->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("NilClass"));
    QCOMPARE(dec2->qualifiedIdentifier(), QualifiedIdentifier("$a"));
    QCOMPARE(dec2->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("NilClass"));
}

//END: Simple Statements

//BEGIN: Assignments

void TestDUChain::multipleAssignment1()
{
    QByteArray code("a, b = 1, 'a'");
    TopDUContext *top = parse(code, "multipleAssignment1");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    QVERIFY(top->localDeclarations().size() == 2);

    /* a */
    Declaration *dec1 = top->localDeclarations().at(0);
    QVERIFY(dec1->type<StructureType>());
    QCOMPARE(dec1->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));

    /* b */
    Declaration *dec2 = top->localDeclarations().at(1);
    QVERIFY(dec2->type<StructureType>());
    QCOMPARE(dec2->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("String"));
}

void TestDUChain::multipleAssignment2()
{
    QByteArray code("a, b = 1, [1, 2, 3]");

    /* TODO: pending */
    QVERIFY(true);
}

void TestDUChain::multipleAssignmentLeft()
{
    QByteArray code("a, b, c = 1, 2");
    TopDUContext *top = parse(code, "multipleAssignmentLeft");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    QVERIFY(top->localDeclarations().size() == 3);

    /* a */
    Declaration *dec1 = top->localDeclarations().at(0);
    QVERIFY(dec1->type<StructureType>());
    QCOMPARE(dec1->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));

    /* b */
    Declaration *dec2 = top->localDeclarations().at(1);
    QVERIFY(dec2->type<StructureType>());
    QCOMPARE(dec2->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));

    /* c */
    Declaration *dec3 = top->localDeclarations().at(2);
    QVERIFY(dec3->type<StructureType>());
    QCOMPARE(dec3->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("NilClass"));
}

void TestDUChain::multipleAssignmentRight1()
{
    QByteArray code("a = 1, 2, 3");
    TopDUContext *top = parse(code, "multipleAssignmentRight1");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    QVERIFY(top->localDeclarations().size() == 1);

    Declaration *dec = top->localDeclarations().at(0);
    QVERIFY(dec->type<StructureType>());
    QCOMPARE(dec->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));
}

void TestDUChain::multipleAssignmentRight2()
{
    QByteArray code("a, = 1, 2, 3");
    TopDUContext *top = parse(code, "multipleAssignmentRight2");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    QVERIFY(top->localDeclarations().size() == 1);

    Declaration *dec = top->localDeclarations().at(0);
    QVERIFY(dec->type<StructureType>());
    QCOMPARE(dec->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));
}

void TestDUChain::multipleAssignmentStar()
{
    QByteArray code("a, *, b = 1, 2, 3, 4, 5, 'a'");

    /* TODO: pending */
    QVERIFY(true);
}

void TestDUChain::multipleAssignmentNamedStar()
{
    QByteArray code("a, *b, c = 1, 2, 3, 4, 5, 'a'");

    /* TODO: pending */
    QVERIFY(true);
}

void TestDUChain::unpackArray()
{
    QByteArray code("a, b, c = [1, 2]");

    /* TODO: pending */
    QVERIFY(true);
}

void TestDUChain::aliasedAssignment()
{
    QByteArray code("a = 1..2; b = 1; c, d = a, b");
    TopDUContext *top = parse(code, "aliasedAssignment");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    QVERIFY(top->localDeclarations().size() == 4);

    Declaration *dec3 = top->localDeclarations().at(2);
    QVERIFY(dec3->type<StructureType>());
    QCOMPARE(dec3->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Range"));

    Declaration *dec4 = top->localDeclarations().at(3);
    QVERIFY(dec4->type<StructureType>());
    QCOMPARE(dec4->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));
}

//END: Assignments

//BEGIN: Variable Length Container

void TestDUChain::assignFromArrayItem1()
{
    QByteArray code("a = [1, 2, 3]; b = a[0]");

    /* TODO: pending */
    QVERIFY(true);
}

void TestDUChain::assignFromArrayItem2()
{
    QByteArray code("a = [1, '2']; b = a[0]");

    /* TODO: pending */
    QVERIFY(true);
}

void TestDUChain::assignFromHashItem()
{
    QByteArray code("a = { :a => 'a', b: 1 }; b = a[:a]");

    /* TODO: pending */
    QVERIFY(true);
}

void TestDUChain::assignToArrayItem()
{
    QByteArray code("a = [1, nil]; a[1] = 2");

    /* TODO: pending */
    QVERIFY(true);
}

//END: Variable Length Container

//BEGIN: Declarations

void TestDUChain::methodDeclaration()
{
    QByteArray code("class Klass; def foo(a, b); end; def asd; end; ");
    code += "def self.selfish; end; def Klass.selfish; end; end";
    TopDUContext *top = parse(code, "methodDeclaration");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());
    QVector<Declaration *> decs = top->childContexts().first()->localDeclarations();

    // Instance methods
    MethodDeclaration *d1 = dynamic_cast<MethodDeclaration *>(decs.first());
    QVERIFY(!d1->isClassMethod());

    MethodDeclaration *d2 = dynamic_cast<MethodDeclaration *>(decs.at(1));
    QVERIFY(!d2->isClassMethod());

    // Class methods
    MethodDeclaration *d3 = dynamic_cast<MethodDeclaration *>(decs.at(2));
    QVERIFY(d3->isClassMethod());

    MethodDeclaration *d4 = dynamic_cast<MethodDeclaration *>(decs.at(3));
    QVERIFY(d4->isClassMethod());
}

//END: Declarations

//BEGIN: Returning Values

void TestDUChain::multipleReturns()
{
    QByteArray code("def foo(a, b); return nil if a.nil?; return 'a'; end");
    TopDUContext *top = parse(code, "multipleReturns");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    Declaration *decl = top->localDeclarations().first();
    FunctionType::Ptr ft = decl->type<FunctionType>();
    UnsureType::Ptr ut = UnsureType::Ptr::dynamicCast(ft->returnType());
    QList<QString> list;
    list << "String" << "NilClass";
    testUnsureTypes(ut, list);
}

void TestDUChain::implicitReturn()
{
    QByteArray code("def foo; 'a'; end");
    TopDUContext *top = parse(code, "implicitReturn");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    Declaration *decl = top->localDeclarations().first();
    FunctionType::Ptr ft = decl->type<FunctionType>();
    StructureType::Ptr st = StructureType::Ptr::dynamicCast(ft->returnType());
    QCOMPARE(st->qualifiedIdentifier(), QualifiedIdentifier("String"));
}

void TestDUChain::mixedExplicitAndImplicitReturn()
{
    QByteArray code("def foo(a); return nil if a.nil?; 'a'; end");
    TopDUContext *top = parse(code, "mixedExplicitAndImplicitReturn");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    Declaration *decl = top->localDeclarations().first();
    FunctionType::Ptr ft = decl->type<FunctionType>();
    UnsureType::Ptr ut = UnsureType::Ptr::dynamicCast(ft->returnType());
    QList<QString> list;
    list << "String" << "NilClass";
    testUnsureTypes(ut, list);
}

//END: Returning Values

//BEGIN: Method Calls

void TestDUChain::callingtoNew()
{
    QByteArray code("class Klass; end; obj = Klass.new");
    TopDUContext *top = parse(code, "callingToNew");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    Declaration *obj = top->localDeclarations().at(1);
    QCOMPARE(obj->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Klass"));
}

void TestDUChain::setMethodArgumentTypes1()
{
    QByteArray code("def foo(a, b); end; foo 1, 2");
    TopDUContext *top = parse(code, "setMethodArgumentTypes1");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    MethodDeclaration *md = dynamic_cast<MethodDeclaration *>(top->localDeclarations().first());
    QVERIFY(md);
    QVector<Declaration *> args = DUChainUtils::getArgumentContext(md)->localDeclarations();
    QVERIFY(args.size() == 2);
    foreach (const Declaration *d, args)
        QCOMPARE(d->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));
}

void TestDUChain::setMethodArgumentTypes2()
{
    QByteArray code("def foo(a, b); end; c = 1.2; foo c, 2");
    TopDUContext *top = parse(code, "setMethodArgumentTypes2");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    MethodDeclaration *md = dynamic_cast<MethodDeclaration *>(top->localDeclarations().first());
    QVERIFY(md);
    QVector<Declaration *> args = DUChainUtils::getArgumentContext(md)->localDeclarations();
    QVERIFY(args.size() == 2);
    QCOMPARE(args.first()->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Float"));
    QCOMPARE(args.last()->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));
}

void TestDUChain::setUnsureArgument()
{
    QByteArray code("def foo(a, b); end; foo 1, 2; foo 'asd', 2");
    TopDUContext *top = parse(code, "setUnsureArgument");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    MethodDeclaration *md = dynamic_cast<MethodDeclaration *>(top->localDeclarations().first());
    QVERIFY(md);
    QVector<Declaration *> args = DUChainUtils::getArgumentContext(md)->localDeclarations();
    QVERIFY(args.size() == 2);
    UnsureType::Ptr unsure = UnsureType::Ptr::dynamicCast(args.first()->indexedType().abstractType());
    QList<QString> list;
    list << "Fixnum" << "String";
    testUnsureTypes(unsure, list);
    QCOMPARE(args.last()->type<StructureType>()->qualifiedIdentifier(), QualifiedIdentifier("Fixnum"));
}

//END: Method Calls

//BEGIN: Include & Extend

void TestDUChain::include1()
{
    /*
     * The class Klass includes the module AA::BB that has the instance
     * method foo.
     */
    QByteArray code("module AA; module BB; def foo; end; def self.selfish; end;");
    code += "end; end; class Klass; include AA::BB; end";
    TopDUContext *top = parse(code, "include1");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    Declaration *obj = top->findLocalDeclarations(Identifier("Klass")).first();
    QVERIFY(obj->internalContext()->localDeclarations(obj->topContext()).size() == 1);
}

void TestDUChain::include2()
{
    /*
     * The module AA::CC includes the module AA::BB, that has the instance
     * method foo. Finally, the class Klass includes the module AA::CC.
     */
    QByteArray code("module AA; module BB; def foo; end; end; module CC; ");
    code += "include ::AA::BB; end; end; class Klass; include AA::CC; end";
    TopDUContext *top = parse(code, "include2");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    /* TODO: pending */
    QVERIFY(true);
}

void TestDUChain::extend1()
{
    /* Same as include1 but with extend and a class method */
    QByteArray code("module AA; module BB; def foo; end; def self.selfish; end;");
    code += "end; end; class Klass; extend AA::BB; end";
    TopDUContext *top = parse(code, "extend1");
    DUChainReleaser releaser(top);
    DUChainWriteLocker lock(DUChain::lock());

    /* TODO: pending */
    QVERIFY(true);
}

//END: Include & Extend

} // End of namespace Ruby


#include "duchain.moc"
