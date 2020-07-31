/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef PLASMA_KPIPESKDE_KPIPESVIEW_H
#define PLASMA_KPIPESKDE_KPIPESVIEW_H

#include <QtQuick/QQuickFramebufferObject>

class KPipesView : public QQuickFramebufferObject {
Q_OBJECT

public:
    [[nodiscard]] Renderer *createRenderer() const override;
};


#endif //PLASMA_KPIPESKDE_KPIPESVIEW_H
