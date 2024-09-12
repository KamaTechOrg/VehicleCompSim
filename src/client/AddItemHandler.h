#pragma once

#include "IActionHandler.h"
#include "items/serializableitem.h"
#include "gui/customscene.h"

class AddItemHandler : public IActionHandler {
public:
    explicit AddItemHandler(CustomScene* scene);
    void handle(const QJsonObject& message) override;

private:
    QGraphicsItem* createGraphicsItem(int itemType, const QJsonObject& jsonObj);
    CustomScene* m_scene;
};

