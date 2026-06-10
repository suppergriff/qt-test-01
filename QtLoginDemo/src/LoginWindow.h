#pragma once

#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;

class LoginWindow : public QWidget {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);

private slots:
    void onLoginClicked();

private:
    void setupUi();
    void setStatus(const QString& text, const QColor& color);
    void setLoginEnabled(bool enabled);

    QLineEdit* m_usernameEdit;
    QLineEdit* m_passwordEdit;
    QPushButton* m_loginButton;
    QLabel* m_statusLabel;
};
