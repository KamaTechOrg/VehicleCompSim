#pragma once

#include "IActionHandler.h"
#include "serializableitem.h"

class DeleteItemHandler : public IActionHandler {
public:
    DeleteItemHandler();
    void handle(const QJsonObject& message) override;

private:
    SerializableItem* createModel(int itemType, const QJsonObject& jsonObj);
};