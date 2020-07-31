/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/

#include "kpipeskdeplugin.h"

#include "kpipesview.h"

// Qt
#include <QQmlEngine>
#include <QQmlContext>


void KPipesKDEPlugin::registerTypes(const char* uri)
{
    Q_ASSERT(uri == QLatin1String("com.kneelawk.private.kpipeskde"));

    qmlRegisterType<KPipesView>(uri, 0, 1, "KPipesView");
}
