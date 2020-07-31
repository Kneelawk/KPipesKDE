/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/

#include "kpipesview.h"

#include "kpipesrenderer.h"

QQuickFramebufferObject::Renderer *KPipesView::createRenderer() const {
    return new KPipesRenderer(this);
}
