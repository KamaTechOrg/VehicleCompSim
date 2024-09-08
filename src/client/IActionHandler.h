#pragma once

#include <QJsonObject>

class IActionHandler {
public:
    virtual ~IActionHandler() = default;
    virtual void handle(const QJsonObject& message) = 0;
};