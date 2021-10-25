#pragma once

#include <QObject>

class FilterActionSetTransportTest : public QObject
{
    Q_OBJECT
public:
    explicit FilterActionSetTransportTest(QObject *parent = nullptr);
    ~FilterActionSetTransportTest() override;

private Q_SLOTS:
    void shouldBeEmpty();
    void shouldHaveDefaultValue();
    void shouldHaveRequiredPart();
    void shouldHaveSieveRequires();
};

