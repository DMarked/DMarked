#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include "document.h"
#include <QWebEngineView>

class PreviewWidget : public QWebEngineView
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);

    void setText(const QString &content);

private:
    Document m_content;
};

#endif // PREVIEWWIDGET_H
