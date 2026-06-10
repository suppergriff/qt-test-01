#include "AuthValidator.h"

AuthValidator::Result AuthValidator::validate(const QString& username,
                                              const QString& password) {
    if (username == "admin" && password == "123456") {
        return Result::Success;
    }
    return Result::WrongCredentials;
}
