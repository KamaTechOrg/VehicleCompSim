#pragma once

#include "IActionHandler.h"
#include "items/serializableitem.h"
#include "gui/customscene.h"

class ModifyItemHandler : public IActionHandler {
public:
    explicit ModifyItemHandler(CustomScene* scene);
    void handle(const QJsonObject& message) override;

private:
    QGraphicsItem* createGraphicsItem(int itemType, const QJsonObject& jsonObj);
    CustomScene* m_scene;
};

