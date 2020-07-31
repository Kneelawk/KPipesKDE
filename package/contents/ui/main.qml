/*
    SPDX-FileCopyrightText: 2020 Kneelawk <kneelawk@gmail.com>
    SPDX-License-Identifier: MIT
*/

import QtQuick 2.1

import QtQuick.Layouts 1.1

import org.kde.plasma.core 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import com.kneelawk.private.kpipeskde 0.1

Item {
    id: root

    KPipesView {
        anchors.fill: parent
    }
}
