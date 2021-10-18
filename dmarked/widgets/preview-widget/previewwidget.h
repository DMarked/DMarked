#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include "document.h"
#include "previewpage.h"

#include <QWebChannel>
#include <QWebEngineView>

class PreviewWidget : public QWebEngineView
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = nullptr);

    void setText(const QString &content);

public slots:
    void setMdTheme(const QString &theme);

private:
    Document m_content;
    PreviewPage *m_page;
    QWebChannel *m_channel;
};

#endif // PREVIEWWIDGET_H
