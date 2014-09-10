/* This file is part of KDevelop
 *
 * Copyright 2014 Miquel Sabaté Solà <mikisabate@gmail.com>
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


#ifndef RUBY_DEBUG_DUCHAIN_TESTS_H
#define RUBY_DEBUG_DUCHAIN_TESTS_H


#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(DUCHAIN_TESTS)

#define rDebug() qCDebug(DUCHAIN_TESTS)
#define rWarning() qCWarning(DUCHAIN_TESTS)
#define rError() qCError(DUCHAIN_TESTS)


#endif /* RUBY_DEBUG_DUCHAIN_TESTS_H */
