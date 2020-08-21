add_definitions(-DTRANSLATION_DOMAIN=\"plasma_wallpaper_com.kneelawk.kpipeskde\")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# NOTE: You must run a build clean every time a resource included in the resources.s file is updated.
# Otherwise, your build system won't know to rebuild resources.s.
set(kpipeskdeplugin_SRCS
        kpipeskde.qrc
        kpipeskdeplugin.cpp
        kpipesview.cpp
        kpipesrenderer.cpp
        tiny_obj_loader.cc
        vertex.cpp)

add_library(kpipeskdeplugin SHARED ${kpipeskdeplugin_SRCS})

target_include_directories(kpipeskdeplugin PRIVATE ${GLM_INCLUDE_DIRS})
target_link_directories(kpipeskdeplugin PRIVATE ${GLM_LIBRARY_DIRS})
add_definitions(${GLM_DEFINITIONS})

target_link_libraries(kpipeskdeplugin
        KF5::I18n
        Qt5::Gui
        Qt5::Qml
        Qt5::Quick
        ${GLM_LIBRARIES}
        )

install(TARGETS kpipeskdeplugin DESTINATION ${KDE_INSTALL_QMLDIR}/com/kneelawk/private/kpipeskde)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/com/kneelawk/private/kpipeskde)
