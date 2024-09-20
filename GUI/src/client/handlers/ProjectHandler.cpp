#include "ProjectHandler.h"
#include <QDebug>
#include "clientconstants.h"

void ProjectHandler::handle(const QJsonObject& message) {
    QString command = message[ClientConstants::KEY_COMMAND].toString();
    if(command == "add") {
        ProjectModel* project = createProject(message);
        if(project) {
            GlobalState::getInstance().addProject(project);
            qDebug() << "Project added";
        }
    }
}

ProjectModel* ProjectHandler::createProject(const QJsonObject& jsonObj) {
    QString name = jsonObj[ClientConstants::KEY_NAME].toString();
    QString id = jsonObj[ClientConstants::KEY_ID].toString();
    ProjectModel* project = new ProjectModel(name, id, true);
    return project;
}