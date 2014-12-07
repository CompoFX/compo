#include "compoview.h"
#include "compoviewwidget.h"\
#include <QDeclarativeItem>
#include <QGraphicsProxyWidget>

CompoView::CompoView(QDeclarativeItem *parent):
    QDeclarativeItem(parent), mProxy(0)
{
    // By default, QDeclarativeItem does not draw anything. If you subclass
    // QDeclarativeItem to create a visual item, you will need to uncomment the
    // following line:

    // setFlag(ItemHasNoContents, false);
    mProxy = new QGraphicsProxyWidget(this);
    CompoViewWidget *w = new CompoViewWidget;
    mProxy->setWidget(w);
}

CompoView::~CompoView()
{
}

