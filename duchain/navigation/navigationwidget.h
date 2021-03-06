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


#ifndef NAVIGATIONWIDGET_H
#define NAVIGATIONWIDGET_H


#include <duchain/duchainexport.h>
#include <language/duchain/navigation/abstractnavigationwidget.h>


namespace KDevelop { class IncludeItem; }

namespace ruby {

/**
 * @class NavigationWidget
 *
 * The NavigationWidget for the Ruby plugin.
 */
class KDEVRUBYDUCHAIN_EXPORT NavigationWidget : public KDevelop::AbstractNavigationWidget
{
    Q_OBJECT

public:
    /**
     * Constructor.
     *
     * @param decl A pointer to the involved declaration.
     * @param topContext The involved Top Context.
     * @param htmlPrefix Html-formatted text that should be prepended
     * before any information shown by this widget
     * @param htmlSuffix Html-formatted text that should be appended to any
     * information shown by this widget
     */
    explicit NavigationWidget(KDevelop::DeclarationPointer decl,
                              KDevelop::TopDUContextPointer topContext,
                              const QString &htmlPrefix = QString(),
                              const QString &htmlSuffix = QString(),
                              KDevelop::AbstractNavigationWidget::DisplayHints hints = KDevelop::AbstractNavigationWidget::NoHints);

    /**
     * Constructor.
     *
     * @param item The involved Include Item.
     * @param topContext The involved Top Context.
     * @param htmlPrefix Html-formatted text that should be prepended
     * before any information shown by this widget
     * @param htmlSuffix Html-formatted text that should be appended to any
     * information shown by this widget
     */
    explicit NavigationWidget(const KDevelop::IncludeItem &item,
                              KDevelop::TopDUContextPointer topContext,
                              const QString &htmlPrefix = QString(),
                              const QString &htmlSuffix = QString(),
                              KDevelop::AbstractNavigationWidget::DisplayHints hints = KDevelop::AbstractNavigationWidget::NoHints);

    /**
     * Creates a compact html description given a Declaration.
     *
     * @param decl The given Declaration.
     */
    static QString shortDescription(KDevelop::Declaration *decl);

    /**
     * Creates a compact html description given an Include item.
     *
     * @param decl The given Include item.
     */
    static QString shortDescription(const KDevelop::IncludeItem &item);
};

}


#endif // NAVIGATIONWIDGET_H
