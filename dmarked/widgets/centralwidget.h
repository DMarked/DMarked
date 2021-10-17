#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <DWidget>
#include <DSplitter>
#include <QLayout>

#include <qmarkdowntextedit.h>
#include "preview-widget/previewwidget.h"


DWIDGET_USE_NAMESPACE

class CentralWidget : public DWidget
{
    Q_OBJECT

    friend class MainWindow;
public:
    explicit CentralWidget(DWidget *parent = nullptr);

private:
    QMarkdownTextEdit *m_editor_widget;
    PreviewWidget *m_preview_widget;
    DSplitter *m_splitter;
    QLayout *m_central_layout;
};

#endif // CENTRALWIDGET_H
