#pragma once

#include <memory>
#include "IActionHandler.h"

class RunHandler : public IActionHandler {
public:
    explicit RunHandler(const std::function<void (const QString&)> startFunction, const std::function<void ()> stopFunction);
    void handle(const QJsonObject& message) override;

private:
    std::function<void (const QString&)> m_startFunction;
    std::function<void ()> m_stopFunction;
};