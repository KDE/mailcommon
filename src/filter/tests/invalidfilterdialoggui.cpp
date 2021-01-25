/*
  SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-only
*/

#include "filter/invalidfilters/invalidfilterdialog.h"

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    auto w = new MailCommon::InvalidFilterDialog();
    QVector<MailCommon::InvalidFilterInfo> infoLst;
    for (int i = 0; i < 10; ++i) {
        MailCommon::InvalidFilterInfo info;
        info.setInformation(QStringLiteral("information %1").arg(i));
        info.setName(QStringLiteral("name %1").arg(i));
        infoLst << info;
    }
    w->setInvalidFilters(infoLst);
    w->resize(800, 600);
    w->show();
    app.exec();
    delete w;
    return 0;
}
