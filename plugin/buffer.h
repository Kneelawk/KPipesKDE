/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef PLASMA_KPIPESKDE_BUFFER_H
#define PLASMA_KPIPESKDE_BUFFER_H

#include <QOpenGLBuffer>

template<typename T>
class Buffer {
private:
    QOpenGLBuffer _buffer;
    int _capacity;
    int _size;

public:
    enum BufferWriteResult {
        WriteOk,
        InsufficientCapacity
    };

    enum BufferRemoveResult {
        RemoveOk,
        InsufficientSize
    };

    explicit Buffer(QOpenGLBuffer::Type type) : _buffer(type), _capacity(0), _size(0) {}

    Buffer(int capacity, QOpenGLBuffer::Type type) : _buffer(type), _capacity(capacity), _size(0) {
        if (!_buffer.isCreated())
            _buffer.create();

        _buffer.bind();
        _buffer.allocate(capacity * sizeof(T));
        _buffer.release();
    }

    Buffer(std::vector<T> data, QOpenGLBuffer::Type type) : _buffer(type), _capacity(data.size()),
                                                            _size(data.size()) {
        if (!_buffer.isCreated())
            _buffer.create();

        _buffer.bind();
        _buffer.allocate(data.data(), _capacity * sizeof(T));
        _buffer.release();
    }

    void allocate(int capacity) {
        if (!_buffer.isCreated())
            _buffer.create();

        _capacity = capacity;
        _size = 0;

        _buffer.bind();
        _buffer.allocate(capacity * sizeof(T));
        _buffer.release();
    }

    void allocate(std::vector<T> data) {
        if (!_buffer.isCreated())
            _buffer.create();

        _capacity = data.size();
        _size = data.size();

        _buffer.bind();
        _buffer.allocate(data.data(), _capacity * sizeof(T));
        _buffer.release();
    }

    bool isValid() {
        return _buffer.isCreated();
    }

    void bind() {
        _buffer.bind();
    }

    void release() {
        _buffer.release();
    }

    int size() {
        return _size;
    }

    int sizeBytes() {
        return _size * sizeof(T);
    }

    void clear() {
        _size = 0;
    }

    BufferWriteResult removeLast() {
        if (_size <= 0) {
            return InsufficientSize;
        } else {
            _size--;
            return RemoveOk;
        }
    }

    BufferWriteResult append(T data) {
        if (_size >= _capacity) {
            return InsufficientCapacity;
        }

        _buffer.bind();
        _buffer.write(sizeBytes(), &data, sizeof(T));
        _buffer.release();

        _size++;

        return WriteOk;
    }
};

#endif //PLASMA_KPIPESKDE_BUFFER_H
