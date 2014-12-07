#include "compoviewwidget.h"
#include "CompoGraphicsContext.h"

CompoViewWidget::CompoViewWidget(QWidget *parent) :
    QWidget(parent), mGC(0)
{
    mGC = new CompoGraphicsContext();
    mGC->init((void*)this->winId());
}


CompoViewWidget::~CompoViewWidget()
{
    delete mGC; mGC = 0;
}

