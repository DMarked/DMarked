/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Copyright (C) 2021 DMarked
**
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit, Now it used by DMarked
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QString>

class DmarkedConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool html MEMBER html NOTIFY configChanged FINAL)
    Q_PROPERTY(bool xhtmlOut MEMBER xhtmlOut NOTIFY configChanged FINAL)
    Q_PROPERTY(bool breaks MEMBER breaks NOTIFY configChanged FINAL)
    Q_PROPERTY(bool linkify MEMBER linkify NOTIFY configChanged FINAL)
    Q_PROPERTY(bool typographer MEMBER typographer NOTIFY configChanged FINAL)
public:
    explicit DmarkedConfig() = default;

Q_SIGNALS:
    void configChanged();

private:
    bool html = true;               // Enable HTML tags in source
    bool xhtmlOut = false;          // Use '/' to close single tags (<br />)
    bool breaks = false;            // Convert '\n' in paragraphs into <br>
    bool linkify = true;            // autoconvert URL-like texts to links
    bool typographer = true;        // Enable smartypants and other sweet transforms
};

class Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged NOTIFY markdownLoadFinished FINAL) // important for QWebChannel
    Q_PROPERTY(QString path MEMBER m_filePath NOTIFY textChanged FINAL)

public:
    explicit Document(QObject *parent = nullptr) : QObject(parent) {}

    void setText(const QString &text);
    void setFilePath(const QString &filePath);
    void setNoGui();

Q_SIGNALS:
    void textChanged(const QString &text);
    void markdownLoadFinished();

public Q_SLOTS:
    void onMdLoadFinished();

private:
    QString m_text = "";
    QString m_filePath = "";
    bool isGui = true;
};

#endif // DOCUMENT_H
