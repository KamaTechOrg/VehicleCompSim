#pragma once

#include "IActionHandler.h"
#include "serializableitem.h"
#include "globalstate.h"

class AddItemHandler : public IActionHandler {
public:
    void handle(const QJsonObject& message) override;

private:
    SerializableItem* createModel(int itemType, const QJsonObject& jsonObj);
};
