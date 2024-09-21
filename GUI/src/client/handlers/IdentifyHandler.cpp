#include "IdentifyHandler.h"
#include <QDebug>
#include "clientconstants.h"
#include "globalstate.h"

void IdentifyHandler::handle(const QJsonObject& message) {
    if (!message.contains(ClientConstants::KEY_CLIENT_ID)) {
        qDebug() << "Error: Message does not contain 'clientId' field";
        return;
    }

    QString receivedClientId = message[ClientConstants::KEY_CLIENT_ID].toString();
    WebSocketClient& client = WebSocketClient::getInstance();
    GlobalState& globalState = GlobalState::getInstance();
    
    if (globalState.myClientId() == "-1") {
        globalState.setMyClientId(receivedClientId);
        QSettings settings("VehicleCompSim", "GUI");
        settings.setValue(ClientConstants::KEY_CLIENT_ID, receivedClientId);
        settings.sync();
    } else {
        QJsonObject updateMessage;
        updateMessage[ClientConstants::KEY_ACTION] = ClientConstants::ACTION_UPDATE_CLIENT_ID;
        updateMessage[ClientConstants::KEY_CLIENT_ID] = receivedClientId;
        updateMessage[ClientConstants::KEY_UPDATED_CLIENT_ID] = globalState.myClientId();
        client.sendMessage(updateMessage);
    }
}
