#include "LoginWindow.h"
#include "AuthValidator.h"

#include <QColor>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScreen>
#include <QVBoxLayout>
#include <QGuiApplication>

LoginWindow::LoginWindow(QWidget* parent)
    : QWidget(parent)
    , m_usernameEdit(nullptr)
    , m_passwordEdit(nullptr)
    , m_loginButton(nullptr)
    , m_statusLabel(nullptr)
{
    setupUi();
}

void LoginWindow::setupUi()
{
    setWindowTitle(QStringLiteral("Login"));
    setFixedSize(360, 200);

    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText(QStringLiteral("Enter username"));

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText(QStringLiteral("Enter password"));
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_loginButton = new QPushButton(QStringLiteral("Login"), this);
    m_loginButton->setAutoDefault(true);
    m_loginButton->setDefault(true);

    m_statusLabel = new QLabel(QStringLiteral("Please enter username and password"), this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    setStatus(QStringLiteral("Please enter username and password"), QColor("#888888"));

    auto* formLayout = new QFormLayout;
    formLayout->addRow(QStringLiteral("Username"), m_usernameEdit);
    formLayout->addRow(QStringLiteral("Password"), m_passwordEdit);

    auto* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addStretch();

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_statusLabel);

    connect(m_loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginClicked);

    // 窗口居中
    if (auto* screen = QGuiApplication::primaryScreen()) {
        QRect screenGeometry = screen->availableGeometry();
        int x = (screenGeometry.width() - width()) / 2;
        int y = (screenGeometry.height() - height()) / 2;
        move(x, y);
    }
}

void LoginWindow::setStatus(const QString& text, const QColor& color)
{
    m_statusLabel->setText(text);
    m_statusLabel->setStyleSheet(
        QStringLiteral("color: %1;").arg(color.name()));
}

void LoginWindow::setLoginEnabled(bool enabled)
{
    m_loginButton->setEnabled(enabled);
    m_loginButton->setText(enabled
        ? QStringLiteral("Login")
        : QStringLiteral("Verifying..."));
}

void LoginWindow::onLoginClicked()
{
    const QString username = m_usernameEdit->text();
    const QString password = m_passwordEdit->text();

    if (username.isEmpty()) {
        setStatus(QStringLiteral("Please enter username"), QColor("#FF8C00"));
        return;
    }

    if (password.isEmpty()) {
        setStatus(QStringLiteral("Please enter password"), QColor("#FF8C00"));
        return;
    }

    setLoginEnabled(false);

    const auto result = AuthValidator::validate(username, password);

    if (result == AuthValidator::Result::Success) {
        setStatus(QStringLiteral("Login successful! Welcome, admin"), QColor("#228B22"));
    } else {
        setStatus(QStringLiteral("Invalid username or password"), QColor("#DC143C"));
    }

    setLoginEnabled(true);
}
