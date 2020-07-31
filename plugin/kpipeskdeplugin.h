/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef KPIPESKDEPLUGIN_H
#define KPIPESKDEPLUGIN_H

#include <QQmlExtensionPlugin>

class KPipesKDEPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif // KPIPESKDEPLUGIN_H
