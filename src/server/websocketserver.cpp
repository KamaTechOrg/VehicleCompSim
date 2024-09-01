#include "websocketserver.h"
#include <QSslCertificate>
#include <QSslKey>
#include <QJsonDocument>
#include <QDebug>
#include <iostream>
#include <QFile>

WebSocketServer::WebSocketServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent), m_debug(debug), m_pWebSocketServer(new QWebSocketServer(QStringLiteral("WebSocket Server"),
    QWebSocketServer::NonSecureMode, this)) {
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            std::cout << "the server is listening on port" << port << std::endl;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,this, &WebSocketServer::onNewConnection);
    }
    setupSslConfiguration();
}

void WebSocketServer::setupSslConfiguration() {
    // m_sslConfiguration = QSslConfiguration::defaultConfiguration();
    // m_sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    // m_pWebSocketServer->setSslConfiguration(m_sslConfiguration);
}


WebSocketServer::~WebSocketServer() {
    m_pWebSocketServer->close();
    for (auto client : m_clientsIds) {
        delete client;
    }
}

void WebSocketServer::broadcastMessage(const QString &message)
{
    if (m_debug)
        std::cout << "Broadcasting message:" << message.toStdString() << std::endl;

    for (QWebSocket *client : m_clientsIds) {
        client->sendTextMessage(message);
    }
}

SerializableItem* WebSocketServer::findItemById(const QString &id) {
    auto it = items.find(id);
    if (it != items.end()) {
        return it->second.get();
    }
    return nullptr;
}

void WebSocketServer::addSerializableItem(std::unique_ptr<SerializableItem> item)
{
    items[item->getId()] = std::move(item);
}

QString WebSocketServer::generateClientId() {
    static int idCounter = 0;
    return QString::number(idCounter++);
}

void WebSocketServer::onNewConnection() {
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    if (m_debug)
        std::cout << "New connection established";

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    // Generate and store the client ID
    QString clientId = generateClientId();
    m_clientsIds[clientId] = pSocket;

    //send the client ID to the client
    QJsonObject response;
    response["action"] = "identify";
    response["clientId"] = clientId;
    QJsonDocument responseDoc(response);
    QString responseStr = responseDoc.toJson(QJsonDocument::Compact);
    pSocket->sendTextMessage(responseStr);
}

void WebSocketServer::processMessage(const QString &message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        std::cout << "Message received:" << message.toStdString() << std::endl;

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj = doc.object();

    if (!jsonObj.contains("action")) {
        std::cout << "Error: Message does not contain 'action' field" << std::endl;
        return;
    }

    QString action = jsonObj["action"].toString();

    if (action == "updateClientId") {
        if (!jsonObj.contains("clientId") || !jsonObj.contains("updatedClientId")) {
            std::cout << "Error: Message does not contain 'clientId' field" << std::endl;
            return;
        }
        // Update the client ID
        QString clientId = jsonObj["clientId"].toString();
        QString updatedClientId = jsonObj["updatedClientId"].toString();
        QWebSocket *client = m_clientsIds[clientId];
        m_clientsIds.remove(clientId);
        m_clientsIds[updatedClientId] = client;
        return;
    }

    if (action == "add" || action == "modify" || action == "delete") {
        SerializableItem item;
        item.deserialize(jsonObj);

        // Validate version for modify and delete actions
        if (action != "add") {
            SerializableItem* existingItem = findItemById(item.getId());
            if (existingItem && existingItem->getVersion() > item.getVersion()) {
                // Reject update
                QJsonObject response;
                response["status"] = "error";
                response["message"] = "Version conflict";
                QJsonDocument responseDoc(response);
                QString responseStr = responseDoc.toJson(QJsonDocument::Compact);
                pClient->sendTextMessage(responseStr);
                return;
            }
        }

        // Process the action
        if (action == "add") {
            auto newItem = std::make_unique<SerializableItem>();
            newItem->deserialize(jsonObj);
            addSerializableItem(std::move(newItem));
        } else if (action == "modify") {
            SerializableItem* existingItem = findItemById(item.getId());
            if (existingItem) {
                existingItem->deserialize(jsonObj);
                existingItem->incrementVersion();
            }
        } else if (action == "delete") {
            items.erase(item.getId());
        }

        // Broadcast the update to all clients
        for (QWebSocket *client : m_clientsIds) {
            if (client->isValid() && client != pClient) {
                client->sendTextMessage(message);
            }
        }

        emit messageReceived(pClient, message);
    } else {
        std::cout << "Error: Unknown action '" << action.toStdString() << "'" << std::endl;
    }
}

void WebSocketServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        std::cout << "Socket disconnected";

    if (pClient) {
        QString clientId;
        for (auto it = m_clientsIds.begin(); it != m_clientsIds.end(); ++it) {
            if (it.value() == pClient) {
                clientId = it.key();
                break;
            }
        }
        m_clientsIds.remove(clientId);
        pClient->deleteLater();
    }
}