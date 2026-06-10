#include <QtTest>
#include "AuthValidator.h"

class TestAuthValidator : public QObject {
    Q_OBJECT

private slots:
    void correctCredentials();
    void wrongPassword();
    void wrongUsername();
    void emptyUsername();
    void emptyPassword();
};

void TestAuthValidator::correctCredentials() {
    QCOMPARE(AuthValidator::validate("admin", "123456"),
             AuthValidator::Result::Success);
}

void TestAuthValidator::wrongPassword() {
    QCOMPARE(AuthValidator::validate("admin", "wrong"),
             AuthValidator::Result::WrongCredentials);
}

void TestAuthValidator::wrongUsername() {
    QCOMPARE(AuthValidator::validate("user", "123456"),
             AuthValidator::Result::WrongCredentials);
}

void TestAuthValidator::emptyUsername() {
    QCOMPARE(AuthValidator::validate("", "123456"),
             AuthValidator::Result::WrongCredentials);
}

void TestAuthValidator::emptyPassword() {
    QCOMPARE(AuthValidator::validate("admin", ""),
             AuthValidator::Result::WrongCredentials);
}

QTEST_MAIN(TestAuthValidator)
#include "test_auth_validator.moc"
