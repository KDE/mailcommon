/*
  Copyright (c) 2012-2020 Laurent Montel <montel@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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

namespace MailCommon {
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
