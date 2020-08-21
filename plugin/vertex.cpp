/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/


#include "vertex.h"

bool Vertex::operator==(Vertex other) const {
    return position == other.position && normal == other.normal;
}

