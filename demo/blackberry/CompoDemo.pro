APP_NAME = CompoDemo

CONFIG += qt warn_on cascades10
LIBS += -lbbsystem -lbbcascadespickers

include(config.pri)

device {
    CONFIG(debug, debug|release) {
        LIBS += -L../../../src/views/blackberry/arm/so.le-v7-g
    }

    CONFIG(release, debug|release) {
        LIBS += -L../../../src/views/blackberry/arm/so.le-v7
    }
}

simulator {
    CONFIG(debug, debug|release) {
        LIBS += -L../../../src/views/blackberry/x86/so-g
    }
}

LIBS += -lCompoViewControl