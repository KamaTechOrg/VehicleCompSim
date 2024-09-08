#pragma once

#include "IActionHandler.h"
#include "websocketclient.h" 

class IdentifyHandler : public IActionHandler {
public:
    void handle(const QJsonObject& message) override;
};

