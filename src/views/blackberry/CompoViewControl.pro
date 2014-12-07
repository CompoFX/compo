TEMPLATE = lib

CONFIG += qt warn_on cascades_library
CONFIG += hardening

INCLUDEPATH += ../../../renderer \
               ../../../renderer/blackberry \
               ../../../renderer/egl \
               ../../../../3rdparty/boost_1_55_0 

DEFINES += "COMPO_SHARED_LIB=1"

LIBS += -lslog2 -lscreen -lEGL -lGLESv2 -lexif


include(config.pri)

device {
    CONFIG(debug, debug|release) {
        # Device-Debug custom configuration
        LIBS += -L../support-static -lCompoRenderer-arm
    }

    CONFIG(release, debug|release) {
        # Device-Release custom configuration
        LIBS += -L../support-static -lCompoRenderer-arm
    }
}

simulator {
    CONFIG(debug, debug|release) {
        # Simulator-Debug custom configuration
        LIBS += -L../support-static -lCompoRenderer-x86
    }
}
