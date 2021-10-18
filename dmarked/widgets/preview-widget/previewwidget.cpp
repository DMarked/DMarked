#include "previewwidget.h"
#include "../dropdown-menu/mdtheme.h"

#include <QWebEngineSettings>

PreviewWidget::PreviewWidget(QWidget *parent) : QWebEngineView(parent)
{
    m_page = new PreviewPage(this);
    m_page->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
    setPage(m_page);
    m_channel = new QWebChannel(this);
    m_channel->registerObject(QStringLiteral("content"), &m_content);
    m_page->setWebChannel(m_channel);
    setUrl(QUrl("qrc:/index.html"));

    connect(this, &QWebEngineView::loadFinished, [this](bool success) {
        if (success) {
            bool isDark = DGuiApplicationHelper::instance()->applicationPalette().color(QPalette::Background).lightness() < 128;
            setMdTheme(isDark ? MdTheme::dark_current_theme : MdTheme::light_current_theme);
        }
    });

    setContentsMargins(0, 0, 0, 0);
    setContextMenuPolicy(Qt::NoContextMenu);
    setFocusPolicy(Qt::NoFocus);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void PreviewWidget::setText(const QString &content) {
    m_content.setText(content);
}

void PreviewWidget::setMdTheme(const QString &theme) {
    QString method = "setMdTheme(\'" + theme + "\')";
    m_page->runJavaScript(method);
}
