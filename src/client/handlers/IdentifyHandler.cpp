#include "IdentifyHandler.h"
#include <QSettings>
#include <QDebug>
#include "clientconstants.h"

void IdentifyHandler::handle(const QJsonObject& message) {
    if (!message.contains(ClientConstants::KEY_CLIENT_ID)) {
        qDebug() << "Error: Message does not contain 'clientId' field";
        return;
    }

    QString receivedClientId = message[ClientConstants::KEY_CLIENT_ID].toString();
    WebSocketClient& client = WebSocketClient::getInstance();
    
    if (client.getClientId() == "-1") {
        client.setClientId(receivedClientId);
        QSettings settings("VehicleCompSim", "GUI");
        settings.setValue(ClientConstants::KEY_CLIENT_ID, receivedClientId);
        settings.sync();
    } else {
        QJsonObject updateMessage;
        updateMessage[ClientConstants::KEY_ACTION] = ClientConstants::ACTION_UPDATE_CLIENT_ID;
        updateMessage[ClientConstants::KEY_CLIENT_ID] = receivedClientId;
        updateMessage[ClientConstants::KEY_UPDATED_CLIENT_ID] = client.getClientId();
        client.sendMessage(updateMessage);
    }
}
