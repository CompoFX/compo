#ifndef COMPOVIEW_H
#define COMPOVIEW_H

#include <QDeclarativeItem>

class QGraphicsProxyWidget;

class CompoView : public QDeclarativeItem
{
    Q_OBJECT
    Q_DISABLE_COPY(CompoView)

public:
    CompoView(QDeclarativeItem *parent = 0);
    ~CompoView();
private:
    QGraphicsProxyWidget *mProxy;
};

QML_DECLARE_TYPE(CompoView)

#endif // COMPOVIEW_H

