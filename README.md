KPipesKDE
=========

A version of my KPipes program but running in OpenGL as a QML object that is then installed as a KDE wallpaper.

## Why Rewrite?
I am rewriting my KPipes program using Qt's OpenGL because it is written to use Vulkan on most platforms. The issue is
that using Vulkan to render to a QML widget in KDE's screen locker will cause it to crash when someone tries to unlock
it.
