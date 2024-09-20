#include "websocketclient.h"
#include <QSslConfiguration>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <QDebug>

#include "clientconstants.h"
#include "globalstate.h"

class CustomScene;

WebSocketClient& WebSocketClient::getInstance(const QUrl &url, bool debug)
{
    static WebSocketClient instance(url, debug);
    return instance;
}

WebSocketClient::WebSocketClient(const QUrl &url, bool debug, QObject *parent)
    : QObject(parent), m_url(url), m_debug(debug), m_globalState(GlobalState::getInstance())
{
    if (m_debug)
        qDebug() << "WebSocketClient created with URL:" << url.toString();

    connect(&m_webSocket, &QWebSocket::connected, this, &WebSocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &WebSocketClient::onDisconnected);
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &WebSocketClient::onTextMessageReceived);
    connect(&m_webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::errorOccurred), this, &WebSocketClient::onError);

    connect(&m_globalState, &GlobalState::isRemoteModeChanged, this, &WebSocketClient::onRemoteModeChanged);
    connect(&m_globalState, &GlobalState::currentProjectPublished, this, &WebSocketClient::onProjectAdded);
    connect(&m_globalState, &GlobalState::currentProjectChanged, this, &WebSocketClient::onCurrentProjectChanged);

    m_reconnectTimer = new QTimer(this);
    connect(m_reconnectTimer, &QTimer::timeout, this, &WebSocketClient::attemptReconnection);

    QSettings settings("VehicleCompSim", "GUI");
    m_clientId = settings.value("clientId").toString();
    m_clientId = m_clientId.isEmpty() ? "-1" : m_clientId;

    // Register action handlers
    m_actionHandlers[ClientConstants::ACTION_IDENTIFY] = std::make_unique<IdentifyHandler>();
    m_actionHandlers[ClientConstants::ACTION_ADD] = std::make_unique<AddItemHandler>();
    m_actionHandlers[ClientConstants::ACTION_MODIFY] = std::make_unique<ModifyItemHandler>();
    m_actionHandlers[ClientConstants::ACTION_DELETE] = std::make_unique<DeleteItemHandler>();
    m_actionHandlers[ClientConstants::ACTION_PROJECT] = std::make_unique<ProjectHandler>();
}

void WebSocketClient::connectToServer()
{
    m_globalState.setIsConnecting(true);
    if (m_webSocket.isValid())
    {
        if (m_debug)
            qDebug() << "WebSocket is already connected.";
        m_globalState.setIsConnecting(false);
        return;
    }
    if (m_debug)
        qDebug() << "Attempting to connect to server...";

    m_webSocket.open(m_url);
}

void WebSocketClient::disconnectFromServer()
{
    if (m_debug)
        qDebug() << "Disconnecting from server...";

    m_webSocket.close();
    m_globalState.setIsOnline(false);
}

void WebSocketClient::onRemoteModeChanged(bool remoteMode)
{
    if (remoteMode){
        connectToServer();
    } else {
        disconnectFromServer();
    }
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

    m_globalState.setIsConnecting(false);
    m_globalState.setIsOnline(true);
}

void WebSocketClient::onDisconnected()
{
    if (m_debug)
        qDebug() << "WebSocket disconnected.";

    m_globalState.setIsOnline(false);

    if(m_globalState.isRemoteMode()) {
        m_reconnectTimer->start(5000);
    }
}

void WebSocketClient::attemptReconnection()
{
    if (m_debug)
        qDebug() << "Attempting to reconnect...";

    m_globalState.setIsConnecting(true);

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

    QString action = jsonObj[ClientConstants::KEY_ACTION].toString();
    if (m_actionHandlers.find(action) != m_actionHandlers.end()) {
        m_actionHandlers[action]->handle(jsonObj);
    } else {
        qDebug() << "No handler registered for action:" << action;
    }
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
    jsonObj[ClientConstants::KEY_ACTION] = ClientConstants::ACTION_MODIFY;
    jsonObj[ClientConstants::KEY_PROJECT_ID] = m_globalState.currentProject()->id();

    QJsonDocument doc(jsonObj);
    QString message = doc.toJson(QJsonDocument::Compact);

    if (m_debug)
        qDebug() << "Item modified:" << message;

    m_webSocket.sendTextMessage(message);
}

void WebSocketClient::onItemAdded(const SerializableItem& item) {
    QJsonObject jsonObj = item.serialize();
    jsonObj[ClientConstants::KEY_ACTION] = ClientConstants::ACTION_ADD;
    jsonObj[ClientConstants::KEY_PROJECT_ID] = m_globalState.currentProject()->id();

    QJsonDocument doc(jsonObj);
    QString message = doc.toJson(QJsonDocument::Compact);

    if (m_debug)
        qDebug() << "Item added:" << message;

    m_webSocket.sendTextMessage(message);
}

void WebSocketClient::onItemDeleted(const SerializableItem& item) {
    QJsonObject jsonObj = item.serialize();
    jsonObj[ClientConstants::KEY_ACTION] = ClientConstants::ACTION_DELETE;
    jsonObj[ClientConstants::KEY_PROJECT_ID] = m_globalState.currentProject()->id();

    QJsonDocument doc(jsonObj);
    QString message = doc.toJson(QJsonDocument::Compact);

    if (m_debug)
        qDebug() << "Item deleted:" << message;

    m_webSocket.sendTextMessage(message);
}

void WebSocketClient::onProjectAdded(ProjectModel* project) {
    QJsonObject jsonObj;
    jsonObj[ClientConstants::KEY_ACTION] = ClientConstants::ACTION_PROJECT;
    jsonObj[ClientConstants::KEY_COMMAND] = ClientConstants::ACTION_ADD;
    jsonObj[ClientConstants::KEY_ID] = project->id();
    jsonObj[ClientConstants::KEY_NAME] = project->name();

    QJsonDocument doc(jsonObj);
    QString message = doc.toJson(QJsonDocument::Compact);

    if (m_debug)
        qDebug() << "Project added:" << message;

    m_webSocket.sendTextMessage(message);
}

void WebSocketClient::onCurrentProjectChanged(ProjectModel* project) {
    if (project && m_globalState.isRemoteMode()) {
        QJsonObject jsonObj;
        jsonObj[ClientConstants::KEY_ACTION] = ClientConstants::ACTION_GET_POJECT_ITEMS;
        jsonObj[ClientConstants::KEY_PROJECT_ID] = project->id();

        QJsonDocument doc(jsonObj);
        QString message = doc.toJson(QJsonDocument::Compact);

        if (m_debug)
            qDebug() << "Current project changed:" << message;

        m_webSocket.sendTextMessage(message);
    }
}