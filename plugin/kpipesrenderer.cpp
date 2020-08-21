/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/

#include "kpipesrenderer.h"

#include "kpipesview.h"
#include "tiny_obj_loader.h"
#include "vertex.h"

#include <algorithm>
#include <cmath>
#include <iostream>

#include <QOpenGLFunctions>
#include <QtQuick/QQuickWindow>

#define OpenGLFunctions QOpenGLContext::currentContext()->functions()

KPipesRenderer::KPipesRenderer(const KPipesView *view) : view(view) {
    Q_INIT_RESOURCE(kpipeskde);

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

QString KPipesRenderer::loadText(const QString &name) {
    QFile file(name);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        initError = true;
        qWarning() << "Failed to read file:" << name;
        return "Failed to read file!";
    }

    QTextStream stream(&file);

    return stream.readAll();
}

void KPipesRenderer::loadObj(const QString &name, Buffer<Vertex> &vertexBuf, Buffer<GLushort> &indexBuf) {
    QString meshString = loadText(name);
    tinyobj::ObjReaderConfig config;
    config.vertex_color = false;
    tinyobj::ObjReader reader;
    reader.ParseFromString(meshString.toStdString(), "", config);

    if (!reader.Warning().empty()) {
        qWarning() << "Warning loading obj:" << name;
        qWarning() << reader.Warning().c_str();
    }

    if (!reader.Error().empty()) {
        qWarning() << "Error loading obj:" << name;
        qWarning() << reader.Error().c_str();
    }

    if (!reader.Valid()) {
        initError = true;
        return;
    }

    std::vector<tinyobj::shape_t> shapes = reader.GetShapes();
    tinyobj::attrib_t attrib = reader.GetAttrib();

    if (shapes.empty()) {
        qWarning() << "No shapes loaded in obj:" << name;
        initError = true;
        return;
    }

    if (shapes.size() > 1) {
        qWarning() << "More than one shape in the obj file:" << name << "Only loading the first shape.";
    }

    std::vector<Vertex> vertices;
    std::vector<GLushort> indices;

    for (tinyobj::index_t meshIndex : shapes[0].mesh.indices) {
        tinyobj::real_t posX = attrib.vertices[meshIndex.vertex_index * 3];
        tinyobj::real_t posY = attrib.vertices[meshIndex.vertex_index * 3 + 1];
        tinyobj::real_t posZ = attrib.vertices[meshIndex.vertex_index * 3 + 2];
        tinyobj::real_t norX = attrib.normals[meshIndex.normal_index * 3];
        tinyobj::real_t norY = attrib.normals[meshIndex.normal_index * 3 + 1];
        tinyobj::real_t norZ = attrib.normals[meshIndex.normal_index * 3 + 2];

        Vertex vertex = {
                QVector3D(posX, posY, posZ),
                QVector3D(norX, norY, norZ)
        };

        auto it = std::find(vertices.begin(), vertices.end(), vertex);
        if (it == vertices.end()) {
            auto index = (GLushort) vertices.size();
            vertices.push_back(vertex);
            indices.push_back(index);
        } else {
            GLushort index = std::distance(vertices.begin(), it);
            indices.push_back(index);
        }
    }

    vertexBuf.allocate(vertices);
    indexBuf.allocate(indices);
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

    setupBuffers();
    setupAttribs();
}

void KPipesRenderer::setupBuffers() {
    loadObj(":/kpipe-straight.obj", vertexBuffer, indexBuffer);
}

void KPipesRenderer::setupAttribs() {
    auto *f = OpenGLFunctions;

    vertexBuffer.bind();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 2 * sizeof(float), nullptr);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * 2 * sizeof(float),
                             reinterpret_cast<void *>(3 * sizeof(float)));
    vertexBuffer.release();

    indexBuffer.bind();
}

void KPipesRenderer::initShaders() {
    QString vertText = loadText(":/shader.vert.glsl");
    QString fragText = loadText(":/shader.frag.glsl");

    shaderProgram.reset(new QOpenGLShaderProgram);
    shaderProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertText);
    shaderProgram->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragText);
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
        f->glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_SHORT, nullptr);

        shaderProgram->release();

        if (vao.isCreated()) {
            vao.release();
        }
    }

    // reset OpenGL state
    view->window()->resetOpenGLState();

    update();
}
