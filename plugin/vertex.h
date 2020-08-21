/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef PLASMA_KPIPESKDE_VERTEX_H
#define PLASMA_KPIPESKDE_VERTEX_H

#include <QVector3D>

class Vertex {
public:
    QVector3D position;
    QVector3D normal;

    bool operator==(Vertex other) const;
};

#endif //PLASMA_KPIPESKDE_VERTEX_H
