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
    setWindowTitle(QStringLiteral("登录"));
    setFixedSize(360, 200);

    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText(QStringLiteral("请输入用户名"));

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText(QStringLiteral("请输入密码"));
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_loginButton = new QPushButton(QStringLiteral("登录"), this);
    m_loginButton->setAutoDefault(true);
    m_loginButton->setDefault(true);

    m_statusLabel = new QLabel(QStringLiteral("请输入用户名和密码"), this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    setStatus(QStringLiteral("请输入用户名和密码"), QColor("#888888"));

    auto* formLayout = new QFormLayout;
    formLayout->addRow(QStringLiteral("用户名"), m_usernameEdit);
    formLayout->addRow(QStringLiteral("密码"), m_passwordEdit);

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
        ? QStringLiteral("登录")
        : QStringLiteral("验证中..."));
}

void LoginWindow::onLoginClicked()
{
    const QString username = m_usernameEdit->text();
    const QString password = m_passwordEdit->text();

    if (username.isEmpty()) {
        setStatus(QStringLiteral("请输入用户名"), QColor("#FF8C00"));
        return;
    }

    if (password.isEmpty()) {
        setStatus(QStringLiteral("请输入密码"), QColor("#FF8C00"));
        return;
    }

    setLoginEnabled(false);

    const auto result = AuthValidator::validate(username, password);

    if (result == AuthValidator::Result::Success) {
        setStatus(QStringLiteral("登录成功！欢迎，admin"), QColor("#228B22"));
    } else {
        setStatus(QStringLiteral("用户名或密码错误"), QColor("#DC143C"));
    }

    setLoginEnabled(true);
}
