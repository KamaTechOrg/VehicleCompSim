#pragma once

#include <memory>
#include "IActionHandler.h"
#include "services/RunService.h"

class RunHandler : public IActionHandler {
public:
    explicit RunHandler(const std::function<void ()> startFunction, const std::function<void ()> stopFunction);
    void handle(const QJsonObject& message) override;

private:
    std::function<void ()> m_startFunction;
    std::function<void ()> m_stopFunction;
};