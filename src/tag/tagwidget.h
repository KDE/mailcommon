/*
  SPDX-FileCopyrightText: 2012-2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

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
/*!
 * \class MailCommon::TagWidget
 * \inmodule MailCommon
 * \inheaderfile MailCommon/TagWidget
 *
 * \brief The TagWidget class
 */
class MAILCOMMON_EXPORT TagWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     */
    explicit TagWidget(const QList<KActionCollection *> &actionCollections, QWidget *parent = nullptr);
    /*!
     */
    ~TagWidget() override;

    /*!
     */
    void recordTagSettings(MailCommon::Tag::Ptr tag);

    /*!
     */
    [[nodiscard]] QLineEdit *tagNameLineEdit() const;
    /*!
     */
    /*!
     */
    [[nodiscard]] QCheckBox *textColorCheck() const;
    /*!
     */
    [[nodiscard]] QCheckBox *textFontCheck() const;
    /*!
     */
    [[nodiscard]] QCheckBox *backgroundColorCheck() const;
    /*!
     */
    [[nodiscard]] QCheckBox *inToolBarCheck() const;
    /*!
     */

    [[nodiscard]] KColorCombo *textColorCombo() const;
    /*!
     */
    [[nodiscard]] KColorCombo *backgroundColorCombo() const;

    /*!
     */
    [[nodiscard]] QCheckBox *textBoldCheck() const;
    /*!
     */
    [[nodiscard]] QCheckBox *textItalicCheck() const;

    /*!
     */
    [[nodiscard]] KIconButton *iconButton() const;

    /*!
     */
    [[nodiscard]] KKeySequenceWidget *keySequenceWidget() const;

    /*!
     */
    void setTagTextColor(const QColor &color);
    /*!
     */
    void setTagBackgroundColor(const QColor &color);
    /*!
     */
    void setTagTextFormat(bool bold, bool italic);

Q_SIGNALS:
    /*!
     */
    void changed();
    /*!
     */
    void iconNameChanged(const QString &);

private:
    MAILCOMMON_NO_EXPORT void slotEmitChangeCheck();
    std::unique_ptr<TagWidgetPrivate> const d;
};
}
