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


// Qt + KDE
#include <QtTest/QtTest>

// Ruby
#include <duchain/helpers.h>
#include <duchain/editorintegrator.h>
#include <duchain/builders/declarationbuilder.h>
#include <duchain/tests/benchmarks.h>
#include <parser/parser.h>

QTEST_MAIN(ruby::Benchmarks)

using namespace KDevelop;
namespace ruby
{

Benchmarks::Benchmarks()
{
    // To be able to test without installing to system dirs, we need to have builtins.rb readable from QStandardPaths::GenericLocation.
    QStandardPaths::setTestModeEnabled(true);

    QString genericDataLocation = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    QString docDir = genericDataLocation + "/kdevrubysupport/documentation";
    QString destFile = docDir + "/builtins.rb";

    if (!QDir(docDir).exists()) {
        QDir().mkpath(docDir);
    }

    if (QFile::exists(destFile)) {
        QFile::remove(destFile);
    }

    QString fromPath = QCoreApplication::applicationDirPath() + "/../../../documentation";
    bool ok = QFile::copy(fromPath + "/builtins.rb", destFile);

    Q_ASSERT(ok);
}

const QByteArray Benchmarks::getBuiltinsFile()
{
    const QString &fileName = builtinsFile().str();
    QFile file(fileName);
    bool opened = file.open(QIODevice::ReadOnly);
    Q_ASSERT(opened);
    const QByteArray &txt = file.readAll();
    file.close();
    return txt;
}

void Benchmarks::parser()
{
    const QByteArray &contents = getBuiltinsFile();

    QBENCHMARK {
        Parser parser;
        parser.setContents(contents);
        parser.parse();
    }
}

void Benchmarks::declarationBuilder()
{
    const QByteArray &contents = getBuiltinsFile();
    Parser parser(builtinsFile(), contents);
    Ast * ast = parser.parse();
    EditorIntegrator editor;
    editor.setParseSession(&parser);

    QBENCHMARK {
        DeclarationBuilder builder(&editor);
        ReferencedTopDUContext top = builder.build(builtinsFile(), ast);
    }
}

}

