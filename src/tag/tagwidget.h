/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef MAILCOMMON_TAGWIDGET_H
#define MAILCOMMON_TAGWIDGET_H

#include "mailcommon_export.h"

#include "tag.h"

#include <QWidget>

class QLineEdit;
class KColorCombo;
class KIconButton;
class KKeySequenceWidget;
class QCheckBox;
class KActionCollection;

namespace MailCommon
{
class TagWidgetPrivate;
class MAILCOMMON_EXPORT TagWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TagWidget(const QList<KActionCollection *> &actionCollections, QWidget *parent = nullptr);
    ~TagWidget();

    void recordTagSettings(MailCommon::Tag::Ptr tag);

    Q_REQUIRED_RESULT QLineEdit *tagNameLineEdit() const;
    Q_REQUIRED_RESULT QCheckBox *textColorCheck() const;
    Q_REQUIRED_RESULT QCheckBox *textFontCheck() const;
    Q_REQUIRED_RESULT QCheckBox *backgroundColorCheck() const;
    Q_REQUIRED_RESULT QCheckBox *inToolBarCheck() const;

    Q_REQUIRED_RESULT KColorCombo *textColorCombo() const;
    Q_REQUIRED_RESULT KColorCombo *backgroundColorCombo() const;

    Q_REQUIRED_RESULT QCheckBox *textBoldCheck() const;
    Q_REQUIRED_RESULT QCheckBox *textItalicCheck() const;

    Q_REQUIRED_RESULT KIconButton *iconButton() const;

    Q_REQUIRED_RESULT KKeySequenceWidget *keySequenceWidget() const;

    void setTagTextColor(const QColor &color);
    void setTagBackgroundColor(const QColor &color);
    void setTagTextFormat(bool bold, bool italic);

Q_SIGNALS:
    void changed();
    void iconNameChanged(const QString &);

private:
    void slotEmitChangeCheck();
    TagWidgetPrivate *const d;
};
}

#endif // MAILCOMMON_TAGWIDGET_H
