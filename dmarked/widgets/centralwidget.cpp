#include "centralwidget.h"
#include <QFontDatabase>

CentralWidget::CentralWidget(DWidget *parent): DWidget (parent)
{
      m_editor_widget = new QMarkdownTextEdit;
      m_preview_widget = new PreviewWidget;
      m_splitter = new QSplitter;

      m_splitter->setOrientation(Qt::Horizontal);
      m_splitter->setOpaqueResize(true);
      m_splitter->setHandleWidth(0);
      m_splitter->setChildrenCollapsible(true);

      m_editor_widget->setGeometry(0, 0, 600, 740);
      m_editor_widget->setContentsMargins(0, 0, 0, 0);
      m_editor_widget->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
      m_editor_widget->setFocusPolicy(Qt::StrongFocus);
      m_editor_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      m_editor_widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

      m_preview_widget->setGeometry(0, 0, 600, 740);

      connect(m_editor_widget, &QMarkdownTextEdit::textChanged, [this]() {
          m_preview_widget->setText(m_editor_widget->toPlainText());
      });

      m_splitter->addWidget(m_editor_widget);
      m_splitter->addWidget(m_preview_widget);

      m_central_layout = new QHBoxLayout;
      m_central_layout->addWidget(m_splitter);
      setLayout(m_central_layout);
}

