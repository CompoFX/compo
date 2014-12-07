#include "compoview_plugin.h"
#include "compoview.h"

#include <qdeclarative.h>

void CompoViewPlugin::registerTypes(const char *uri)
{
    // @uri com.compofx
    qmlRegisterType<CompoView>(uri, 1, 0, "CompoView");
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(CompoView, CompoViewPlugin)
#endif

