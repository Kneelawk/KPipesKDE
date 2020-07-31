cmake_minimum_required(VERSION 3.0)

project(plasma-kpipeskde C CXX ASM)

find_package(ECM 1.4.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(KF5 REQUIRED COMPONENTS
        Plasma
        I18n
        )

find_package(Qt5 CONFIG REQUIRED COMPONENTS
        Quick
        Qml
        Gui
        Core
        )

find_package(glm REQUIRED)

# wallpaper applet
plasma_install_package(package com.kneelawk.kpipeskde wallpapers wallpaper)

# qml extension plugin
add_subdirectory(plugin)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
