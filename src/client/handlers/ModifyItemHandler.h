#pragma once

#include "IActionHandler.h"
#include "serializableitem.h"

class ModifyItemHandler : public IActionHandler {
public:
    ModifyItemHandler();
    void handle(const QJsonObject& message) override;

private:
    SerializableItem* createModel(int itemType, const QJsonObject& jsonObj);
};