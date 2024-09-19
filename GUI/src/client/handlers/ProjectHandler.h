#pragma once

#include "IActionHandler.h"
#include "serializableitem.h"
#include "projectmodel.h"
#include "globalstate.h"

class ProjectHandler : public IActionHandler {
public:
    void handle(const QJsonObject& message) override;

private:
    ProjectModel* createProject(const QJsonObject& jsonObj);
private slots:
    void onProjectAdded(ProjectModel* project);
};