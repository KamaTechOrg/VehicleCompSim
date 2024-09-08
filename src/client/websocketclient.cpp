#include "websocketclient.h"
#include <QSslConfiguration>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QDebug>

class CustomScene;

WebSocketClient& WebSocketClient::getInstance(const QUrl &url, bool debug)
{
    static WebSocketClient instance(url, debug);
    return instance;
}

WebSocketClient::WebSocketClient(const QUrl &url, bool debug, QObject *parent)
    : QObject(parent), m_url(url), m_debug(debug)
{
    if (m_debug)
        qDebug() << "WebSocketClient created with URL:" << url.toString();

    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::errorOccurred),
            this, &WebSocketClient::onError);

    m_reconnectTimer = new QTimer(this);
    connect(m_reconnectTimer, &QTimer::timeout, this, &WebSocketClient::attemptReconnection);

    QSettings settings("VehicleCompSim", "GUI");
    m_clientId = settings.value("clientId").toString();
    m_clientId = m_clientId.isEmpty() ? "-1" : m_clientId;

    connectToServer();
}

void WebSocketClient::connectToServer()
{
    if (m_debug)
        qDebug() << "Attempting to connect to server...";

    m_webSocket.open(m_url);
}

void WebSocketClient::onError(QAbstractSocket::SocketError error)
{
    if (m_debug) {
        qDebug() << "WebSocket error:" << error << m_webSocket.errorString();
        qDebug() << "Current URL:" << m_webSocket.requestUrl().toString();
        qDebug() << "Current state:" << m_webSocket.state();        
    }

    emit errorOccurred(m_webSocket.errorString());
}

void WebSocketClient::onConnected() {
    if (m_debug){
        qDebug() << "WebSocket connected successfully";
        qDebug() << "Connected URL:" << m_webSocket.requestUrl().toString();
    }
    // Ensure the signal is connected only once
    disconnect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);

    emit connectionStatusChanged(true);
}

void WebSocketClient::onDisconnected()
{
    if (m_debug)
        qDebug() << "WebSocket disconnected.";

    emit closed();

    // Start reconnection attempts
    m_reconnectTimer->start(10000);  // Try to reconnect every 10 seconds
    emit connectionStatusChanged(false);
}

void WebSocketClient::attemptReconnection()
{
    if (m_debug)
        qDebug() << "Attempting to reconnect...";

    if (!m_webSocket.isValid())
    {
        connectToServer();
    }
    else
    {
        m_reconnectTimer->stop();
    }
}

void WebSocketClient::onTextMessageReceived(const QString &message) {
    if (m_debug)
        qDebug() << "Text message received:" << message;

    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObj = doc.object();

    QString action = jsonObj["action"].toString();
    if (m_actionHandlers.find(action) != m_actionHandlers.end()) {
        m_actionHandlers[action]->handle(jsonObj);
    } else {
        qDebug() << "No handler registered for action:" << action;
    }
}

void WebSocketClient::setScene(CustomScene *scene)
{
    m_scene = scene;
    // Initialize action handlers with the scene
    m_actionHandlers["identify"] = std::make_unique<IdentifyHandler>();
    m_actionHandlers["add"] = std::make_unique<AddItemHandler>(m_scene);
    m_actionHandlers["modify"] = std::make_unique<ModifyItemHandler>(m_scene);
    m_actionHandlers["delete"] = std::make_unique<DeleteItemHandler>(m_scene);
}

void WebSocketClient::sendMessage(const QJsonObject &message)
{
    QJsonDocument doc(message);
    QString messageStr = doc.toJson(QJsonDocument::Compact);

    if (m_debug)
        qDebug() << "Sending message:" << messageStr;

    m_webSocket.sendTextMessage(messageStr);
}

void WebSocketClient::addActionHandler(const QString& action, std::unique_ptr<IActionHandler> handler) {
    m_actionHandlers[action] = std::move(handler);
}

void WebSocketClient::onItemModified(const SerializableItem& item) {
    QJsonObject jsonObj = item.serialize();
    jsonObj["action"] = "modify";

    QJsonDocument doc(jsonObj);
    QString message = doc.toJson(QJsonDocument::Compact);

    if (m_debug)
        qDebug() << "Item modified:" << message;

    m_webSocket.sendTextMessage(message);
}

void WebSocketClient::onItemAdded(const SerializableItem& item) {
    QJsonObject jsonObj = item.serialize();
    jsonObj["action"] = "add";

    QJsonDocument doc(jsonObj);
    QString message = doc.toJson(QJsonDocument::Compact);

    if (m_debug)
        qDebug() << "Item added:" << message;

    m_webSocket.sendTextMessage(message);
}

void WebSocketClient::onItemDeleted(const SerializableItem& item) {
    QJsonObject jsonObj = item.serialize();
    jsonObj["action"] = "delete";

    QJsonDocument doc(jsonObj);
    QString message = doc.toJson(QJsonDocument::Compact);

    if (m_debug)
        qDebug() << "Item deleted:" << message;

    m_webSocket.sendTextMessage(message);
}
