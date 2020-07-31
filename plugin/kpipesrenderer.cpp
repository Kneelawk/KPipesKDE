/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/

#include "kpipesrenderer.h"

#include "cube.h"
#include "kpipesview.h"

#include <iostream>
#include <cmath>

#include <QOpenGLFunctions>
#include <QtQuick/QQuickWindow>

#define OpenGLFunctions QOpenGLContext::currentContext()->functions()

extern char kpipes_resource_shader_frag_glsl_start;
extern int kpipes_resource_shader_frag_glsl_size;
extern char kpipes_resource_shader_vert_glsl_start;
extern int kpipes_resource_shader_vert_glsl_size;

KPipesRenderer::KPipesRenderer(const KPipesView *view) : view(view) {
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(0.0f, 4.0f, 4.0f), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, 1.0f, 0.01f, 100.0f);
    frameMatrix.setToIdentity();
    frameMatrix.scale(1.0f, -1.0f, 1.0f);
    vpfMatrix = frameMatrix * projectionMatrix * viewMatrix;
    previousTime = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
}

void KPipesRenderer::ensureInit() {
    if (!initialized) {
        init();
        initShaders();
        initialized = true;
    }
}

void KPipesRenderer::init() {
    QOpenGLVertexArrayObject::Binder binder(&vao);

    vertexBuffer.create();
    vertexBuffer.bind();
    vertexBuffer.allocate(vertices, sizeof(vertices));

    indexBuffer.create();
    indexBuffer.bind();
    indexBuffer.allocate(indices, sizeof(indices));

    setupAttribs();
}

void KPipesRenderer::setupAttribs() {
    auto *f = OpenGLFunctions;

    vertexBuffer.bind();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 3 * sizeof(float), nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * 3 * sizeof(float),
                             reinterpret_cast<void *>(3 * sizeof(float)));
    f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * 3 * sizeof(float),
                             reinterpret_cast<void *>(2 * 3 * sizeof(float)));
    vertexBuffer.release();

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.bufferId());
}

void KPipesRenderer::initShaders() {
    shaderProgram.reset(new QOpenGLShaderProgram);
    shaderProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                                    QString(QLatin1String(&kpipes_resource_shader_vert_glsl_start,
                                                                          kpipes_resource_shader_vert_glsl_size)));
    shaderProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                    QString(QLatin1String(&kpipes_resource_shader_frag_glsl_start,
                                                                          kpipes_resource_shader_frag_glsl_size)));
    shaderProgram->bindAttributeLocation("a_position", 0);
    shaderProgram->bindAttributeLocation("a_normal", 1);
    shaderProgram->bindAttributeLocation("a_color", 2);
    shaderProgram->link();
    vpfMatrixUniform = shaderProgram->uniformLocation("u_vpf_matrix");
    light1DirectionUniform = shaderProgram->uniformLocation("u_light1_direction");
    light1StrengthUniform = shaderProgram->uniformLocation("u_light1_strength");
    light2DirectionUniform = shaderProgram->uniformLocation("u_light2_direction");
    light2StrengthUniform = shaderProgram->uniformLocation("u_light2_strength");
    ambientLightUniform = shaderProgram->uniformLocation("u_ambient_light");
}

void KPipesRenderer::ensureLighting() {
    if (!lit) {
        shaderProgram->setUniformValue(light1DirectionUniform, light1Direction);
        shaderProgram->setUniformValue(light1StrengthUniform, light1Strength);
        shaderProgram->setUniformValue(light2DirectionUniform, light2Direction);
        shaderProgram->setUniformValue(light2StrengthUniform, light2Strength);
        shaderProgram->setUniformValue(ambientLightUniform, ambientLight);
        lit = true;
    }
}

QOpenGLFramebufferObject *KPipesRenderer::createFramebufferObject(const QSize &size) {
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, (float) size.width() / (float) size.height(), 0.01f, 100.0f);
    vpfMatrix = frameMatrix * projectionMatrix * viewMatrix;

    return Renderer::createFramebufferObject(size);
}

float KPipesRenderer::getDelta() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch());
    auto elapsed = (now - previousTime).count();
    previousTime = now;

    return (float) elapsed / 1000.0f;
}

void KPipesRenderer::render() {
    ensureInit();

    // get functions
    auto *f = OpenGLFunctions;

    // clear
    if (initError) {
        f->glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        f->glClear(GL_COLOR_BUFFER_BIT);
    } else {
        if (vao.isCreated()) {
            vao.bind();
        } else {
            setupAttribs();
        }

        shaderProgram->bind();
        ensureLighting();

        float delta = getDelta();
        rotation += delta * 0.2f;
        float x = ::sinf(rotation) * 4.0f;
        float z = ::cosf(rotation) * 4.0f;

        viewMatrix.setToIdentity();
        viewMatrix.lookAt(QVector3D(x, 4.0f, z), QVector3D(0.0f, 0.0f, 0.0f), QVector3D(0.0f, 1.0f, 0.0f));
        vpfMatrix = frameMatrix * projectionMatrix * viewMatrix;

        shaderProgram->setUniformValue(vpfMatrixUniform, vpfMatrix);

        f->glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        f->glClear((GLbitfield) GL_COLOR_BUFFER_BIT | (GLbitfield) GL_DEPTH_BUFFER_BIT);

        f->glEnable(GL_DEPTH_TEST);
        f->glEnable(GL_CULL_FACE);
        f->glDepthMask(GL_TRUE);
        f->glDepthFunc(GL_LESS);
        f->glFrontFace(GL_CW);
        f->glCullFace(GL_BACK);

        // draw
        f->glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

        shaderProgram->release();

        if (vao.isCreated()) {
            vao.release();
        }
    }

    // reset OpenGL state
    view->window()->resetOpenGLState();

    update();
}
