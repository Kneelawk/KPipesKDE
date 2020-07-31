//
// Created by jedidiah on 7/31/20.
//

#ifndef PLASMA_KPIPESKDE_CUBE_H
#define PLASMA_KPIPESKDE_CUBE_H

#include "vertex.h"

static const Vertex vertices[] = {
        // back side
        Vertex{
                QVector3D(-1.0, -1.0, -1.0),
                QVector3D(0.0, 0.0, -1.0),
                QVector3D(0.0, 0.0, 0.0),
        },
        Vertex{
                QVector3D(1.0, -1.0, -1.0),
                QVector3D(0.0, 0.0, -1.0),
                QVector3D(1.0, 0.0, 0.0),
        },
        Vertex{
                QVector3D(-1.0, 1.0, -1.0),
                QVector3D(0.0, 0.0, -1.0),
                QVector3D(0.0, 1.0, 0.0),
        },
        Vertex{
                QVector3D(1.0, 1.0, -1.0),
                QVector3D(0.0, 0.0, -1.0),
                QVector3D(1.0, 1.0, 0.0),
        },
        // bottom side
        Vertex{
                QVector3D(1.0, -1.0, -1.0),
                QVector3D(0.0, -1.0, 0.0),
                QVector3D(1.0, 0.0, 0.0),
        },
        Vertex{
                QVector3D(-1.0, -1.0, -1.0),
                QVector3D(0.0, -1.0, 0.0),
                QVector3D(0.0, 0.0, 0.0),
        },
        Vertex{
                QVector3D(1.0, -1.0, 1.0),
                QVector3D(0.0, -1.0, 0.0),
                QVector3D(1.0, 0.0, 1.0),
        },
        Vertex{
                QVector3D(-1.0, -1.0, 1.0),
                QVector3D(0.0, -1.0, 0.0),
                QVector3D(0.0, 0.0, 1.0),
        },
        // left side
        Vertex{
                QVector3D(-1.0, -1.0, 1.0),
                QVector3D(-1.0, 0.0, 0.0),
                QVector3D(0.0, 0.0, 1.0),
        },
        Vertex{
                QVector3D(-1.0, -1.0, -1.0),
                QVector3D(-1.0, 0.0, 0.0),
                QVector3D(0.0, 0.0, 0.0),
        },
        Vertex{
                QVector3D(-1.0, 1.0, 1.0),
                QVector3D(-1.0, 0.0, 0.0),
                QVector3D(0.0, 1.0, 1.0),
        },
        Vertex{
                QVector3D(-1.0, 1.0, -1.0),
                QVector3D(-1.0, 0.0, 0.0),
                QVector3D(0.0, 1.0, 0.0),
        },
        // front side
        Vertex{
                QVector3D(1.0, -1.0, 1.0),
                QVector3D(0.0, 0.0, 1.0),
                QVector3D(1.0, 0.0, 1.0),
        },
        Vertex{
                QVector3D(-1.0, -1.0, 1.0),
                QVector3D(0.0, 0.0, 1.0),
                QVector3D(0.0, 0.0, 1.0),
        },
        Vertex{
                QVector3D(1.0, 1.0, 1.0),
                QVector3D(0.0, 0.0, 1.0),
                QVector3D(1.0, 1.0, 1.0),
        },
        Vertex{
                QVector3D(-1.0, 1.0, 1.0),
                QVector3D(0.0, 0.0, 1.0),
                QVector3D(0.0, 1.0, 1.0),
        },
        // top side
        Vertex{
                QVector3D(1.0, 1.0, 1.0),
                QVector3D(0.0, 1.0, 0.0),
                QVector3D(1.0, 1.0, 1.0),
        },
        Vertex{
                QVector3D(-1.0, 1.0, 1.0),
                QVector3D(0.0, 1.0, 0.0),
                QVector3D(0.0, 1.0, 1.0),
        },
        Vertex{
                QVector3D(1.0, 1.0, -1.0),
                QVector3D(0.0, 1.0, 0.0),
                QVector3D(1.0, 1.0, 0.0),
        },
        Vertex{
                QVector3D(-1.0, 1.0, -1.0),
                QVector3D(0.0, 1.0, 0.0),
                QVector3D(0.0, 1.0, 0.0),
        },
        // right side
        Vertex{
                QVector3D(1.0, -1.0, -1.0),
                QVector3D(1.0, 0.0, 0.0),
                QVector3D(1.0, 0.0, 0.0),
        },
        Vertex{
                QVector3D(1.0, -1.0, 1.0),
                QVector3D(1.0, 0.0, 0.0),
                QVector3D(1.0, 0.0, 1.0),
        },
        Vertex{
                QVector3D(1.0, 1.0, -1.0),
                QVector3D(1.0, 0.0, 0.0),
                QVector3D(1.0, 1.0, 0.0),
        },
        Vertex{
                QVector3D(1.0, 1.0, 1.0),
                QVector3D(1.0, 0.0, 0.0),
                QVector3D(1.0, 1.0, 1.0),
        },
};

static const GLushort indices[] = {
        0, 3, 1, 0, 2, 3, // back side
        4, 7, 5, 4, 6, 7, // bottom side
        8, 11, 9, 8, 10, 11, // left side
        12, 15, 13, 12, 14, 15, // front side
        16, 19, 17, 16, 18, 19, // top side
        20, 23, 21, 20, 22, 23, // right side
};

#endif //PLASMA_KPIPESKDE_CUBE_H
