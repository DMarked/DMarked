#include "previewwidget.h"
#include "previewpage.h"

#include <QWebChannel>
#include <QWebEngineSettings>

PreviewWidget::PreviewWidget(QWidget *parent) : QWebEngineView(parent)
{
    PreviewPage *page = new PreviewPage(this);
    page->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
    setPage(page);
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), &m_content);
    page->setWebChannel(channel);
    setUrl(QUrl("qrc:/index.html"));

    setContextMenuPolicy(Qt::NoContextMenu);
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void PreviewWidget::setText(const QString &content) {
    m_content.setText(content);
}
