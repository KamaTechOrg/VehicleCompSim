#include "IdentifyHandler.h"
#include <QSettings>
#include <QDebug>

void IdentifyHandler::handle(const QJsonObject& message) {
    if (!message.contains("clientId")) {
        qDebug() << "Error: Message does not contain 'clientId' field";
        return;
    }

    QString receivedClientId = message["clientId"].toString();
    WebSocketClient& client = WebSocketClient::getInstance();
    
    if (client.getClientId() == "-1") {
        client.setClientId(receivedClientId);
        QSettings settings("VehicleCompSim", "GUI");
        settings.setValue("clientId", receivedClientId);
        settings.sync();
    } else {
        QJsonObject updateMessage;
        updateMessage["action"] = "updateClientId";
        updateMessage["clientId"] = receivedClientId;
        updateMessage["updatedClientId"] = client.getClientId();
        client.sendMessage(updateMessage);
    }
}
