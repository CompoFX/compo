TEMPLATE = lib
TARGET = CompoView
QT += declarative
CONFIG += qt plugin

QMAKE_CXXFLAGS += $$(CXXFLAGS) -DJOLLA=1 -D__JOLLA=1
QMAKE_CFLAGS += $$(CFLAGS) -DJOLLA=1 -D__JOLLA=1
QMAKE_LFLAGS += $$(LDFLAGS)

INCLUDEPATH += ../../../renderer \
../../../renderer/jolla \
../../../renderer/egl \
../../../../3rdparty/boost_1_55_0

LIBS += -L../CompoRenderer -lCompoRenderer-x86

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.compofx

# Input
SOURCES += \
compoview_plugin.cpp \
compoview.cpp \
compoviewwidget.cpp

HEADERS += \
compoview_plugin.h \
compoview.h \
compoviewwidget.h

OTHER_FILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
copy_qmldir.target = $$OUT_PWD/qmldir
copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
QMAKE_EXTRA_TARGETS += copy_qmldir
PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
maemo5 | !isEmpty(MEEGO_VERSION_MAJOR) {
installPath = /usr/lib/qt4/imports/$$replace(uri, \\., /)
} else {
installPath = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., /)
}
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir
}

