#pragma once

#include <QString>

class AuthValidator {
public:
    enum class Result { Success, WrongCredentials };

    static Result validate(const QString& username, const QString& password);
};
