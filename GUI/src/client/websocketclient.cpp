#include "websocketclient.h"
#include <QSslConfiguration>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

class CustomScene;

WebSocketClient& WebSocketClient::getInstance(const QUrl &url, bool debug)
{
    static WebSocketClient instance(url, debug);
    return instance;
}

WebSocketClient::WebSocketClient(const QUrl &url, bool debug, QObject *parent)
    : QObject(parent), m_url(url), m_debug(debug)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);

    m_reconnectTimer = new QTimer(this);
    connect(m_reconnectTimer, &QTimer::timeout, this, &WebSocketClient::attemptReconnection);

    QSettings settings("VehicleCompSim", "GUI");
    m_clientId = settings.value("clientId").toString();
    m_clientId = m_clientId.isEmpty() ? "-1" : m_clientId;

    setupSslConfiguration();

    connectToServer();
}

void WebSocketClient::setupSslConfiguration() {
    m_sslConfiguration = QSslConfiguration::defaultConfiguration();
    m_sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    m_webSocket.setSslConfiguration(m_sslConfiguration);
}
void WebSocketClient::connectToServer()
{
    if (m_debug)
        std::cout << "Attempting to connect to server..." << std::endl;

    m_webSocket.setSslConfiguration(m_sslConfiguration);
    m_webSocket.open(m_url);
}

void WebSocketClient::onConnected() {
    connect(&m_webSocket, &QWebSocket::textMessageReceived,this, &WebSocketClient::onTextMessageReceived);
}

void WebSocketClient::onDisconnected()
{
    if (m_debug)
        std::cout << "WebSocket disconnected." << std::endl;

    emit connectionStatusChanged(false);
    emit closed();

    // Start reconnection attempts
    m_reconnectTimer->start(5000);  // Try to reconnect every 5 seconds
}

void WebSocketClient::attemptReconnection()
{
    if (!m_webSocket.isValid())
    {
        if (m_debug)
            std::cout << "Attempting to reconnect..." << std::endl;

        connectToServer();
    }
    else
    {
        m_reconnectTimer->stop();
    }
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj = doc.object();

    //handle id setting from the server
    QString action = jsonObj["action"].toString();
    if (action == "identify") {
        if (!jsonObj.contains("clientId")) {
            std::cout << "Error: Message does not contain 'clientId' field" << std::endl;
            return;
        }
        if(m_clientId == "-1") {
            m_clientId = jsonObj["clientId"].toString();
            QSettings settings("VehicleCompSim", "GUI");
            settings.setValue("clientId", m_clientId);
            settings.sync();
        } else {
            //send the server the client id
            QJsonObject message;
            message["action"] = "updateClientId";
            message["clientId"] = jsonObj["clientId"].toString();
            message["updatedClientId"] = m_clientId;
            sendMessage(message);
        }
        return;
    }

    // Process the received item
    SerializableItem item;
    item.deserialize(jsonObj);

    int itemType = jsonObj["type"].toInt();

    QGraphicsItem* graphicsItem = nullptr;

    if(itemType == static_cast<int>(ItemType::Connector)) {
        auto temp = new ConnectorItem();
        temp->deserialize(jsonObj);
        graphicsItem = temp;
    } else if(itemType == static_cast<int>(ItemType::Edge)) {
        auto temp = new EdgeItem();
        temp->deserialize(jsonObj);
        graphicsItem = temp;
    } else if(itemType == static_cast<int>(ItemType::Sensor)) {
        auto temp = new SensorItem();
        temp->deserialize(jsonObj);
        graphicsItem = temp;
    }
    // convert item to qgraphicsitem
    if(jsonObj["action"].toString() == "add") {
        m_scene->addItem(graphicsItem);
    } else if(jsonObj["action"].toString() == "modify") {
        m_scene->modifyItem(graphicsItem);
    } else if(jsonObj["action"].toString() == "delete") {
        m_scene->removeItem(graphicsItem);
    }
}

void WebSocketClient::sendMessage(const QJsonObject &message)
{
    QJsonDocument doc(message);
    QString messageStr = doc.toJson(QJsonDocument::Compact);
    m_webSocket.sendTextMessage(messageStr);
}

void WebSocketClient::onItemModified(const SerializableItem& item) {
    QJsonObject jsonObj = item.serialize();

    QJsonDocument doc(jsonObj);
    QString message = doc.toJson(QJsonDocument::Compact);

    m_webSocket.sendTextMessage(message);
}

void WebSocketClient::onItemAdded(const SerializableItem& item) {
    QJsonObject jsonObj = item.serialize();
    jsonObj["action"] = "add";

    QJsonDocument doc(jsonObj);
    QString message = doc.toJson(QJsonDocument::Compact);

    m_webSocket.sendTextMessage(message);
}

void WebSocketClient::onItemDeleted(const SerializableItem& item) {
    QJsonObject jsonObj = item.serialize();
    jsonObj["action"] = "delete";

    QJsonDocument doc(jsonObj);
    QString message = doc.toJson(QJsonDocument::Compact);

    m_webSocket.sendTextMessage(message);
}