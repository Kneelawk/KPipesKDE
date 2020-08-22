/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef PLASMA_KPIPESKDE_KPIPESRENDERER_H
#define PLASMA_KPIPESKDE_KPIPESRENDERER_H

#include "buffer.h"
#include "instance.h"
#include "vertex.h"

#include <chrono>

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QtQuick/QQuickFramebufferObject>

class KPipesView;

class KPipesRenderer : public QQuickFramebufferObject::Renderer {
public:
    explicit KPipesRenderer(const KPipesView *view);

protected:
    void render() override;

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override;

private:
    // initialization stuff
    bool initialized = false;
    bool initError = false;

    QString loadText(const QString &name);

    void loadObj(const QString &name, Buffer<Vertex> &vertexBuf, Buffer<GLushort> &indexBuf);

    void ensureInit();

    void init();

    void setupBuffers();

    void setupAttribs();

    void initShaders();

    // delta calculations
    std::chrono::milliseconds previousTime{};

    float getDelta();

    // matrices
    float rotation = 0.0;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 frameMatrix;
    QMatrix4x4 vpfMatrix;

    // lights
    QVector3D light1Direction = QVector3D(-1.0f, 3.0f, -5.0f);
    float light1Strength = 0.6f;
    QVector3D light2Direction = QVector3D(1.0f, 2.0f, 3.0f);
    float light2Strength = 0.3f;
    float ambientLight = 0.3f;
    bool lit = false;

    void ensureLighting();

    // vertex stuff
    QOpenGLVertexArrayObject vao;
    Buffer<Instance> instanceBuffer = Buffer<Instance>(QOpenGLBuffer::VertexBuffer);
    Buffer<Vertex> vertexBuffer = Buffer<Vertex>(QOpenGLBuffer::VertexBuffer);
    Buffer<GLushort> indexBuffer = Buffer<GLushort>(QOpenGLBuffer::IndexBuffer);

    // shaders
    QScopedPointer<QOpenGLShaderProgram> shaderProgram;
    int vpfMatrixUniform{};
    int light1DirectionUniform{};
    int light1StrengthUniform{};
    int light2DirectionUniform{};
    int light2StrengthUniform{};
    int ambientLightUniform{};

    // parent view
    const KPipesView *view;
};


#endif //PLASMA_KPIPESKDE_KPIPESRENDERER_H
