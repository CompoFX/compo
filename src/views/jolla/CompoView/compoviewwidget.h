#ifndef COMPOVIEWWIDGET_H
#define COMPOVIEWWIDGET_H

#include <QWidget>

class CompoGraphicsContext;
class CompoViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CompoViewWidget(QWidget *parent = 0);
    virtual ~CompoViewWidget();

signals:

public slots:

private:
    CompoGraphicsContext *mGC;
};

#endif // COMPOVIEWWIDGET_H
