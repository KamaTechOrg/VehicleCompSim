#pragma once

#include <unordered_map>
#include <memory>

#include <QObject>
#include <QWebSocket>
#include <QNetworkReply>
#include <QSettings>
#include <QUrl>
#include <QTimer>
#include "observer.h"
#include "serializableitem.h"
#include "projectmodel.h"

#include "handlers/IActionHandler.h"
#include "handlers/IdentifyHandler.h"
#include "handlers/AddItemHandler.h"
#include "handlers/ModifyItemHandler.h"
#include "handlers/DeleteItemHandler.h"
#include "handlers/ProjectHandler.h"


class SerializableItem;

class WebSocketClient : public QObject, public Observer
{
    Q_OBJECT
public:
    static WebSocketClient& getInstance(const QUrl &url = QUrl(QStringLiteral("ws://165.232.123.144:8080")), bool debug = true);

    WebSocketClient(const WebSocketClient&) = delete;
    WebSocketClient& operator=(const WebSocketClient&) = delete;

    void onItemModified(const SerializableItem& item) override;
    void onItemAdded(const SerializableItem& item) override;
    void onItemDeleted(const SerializableItem& item) override;
    
    const QString& getClientId() const { return m_clientId; }
    void setClientId(const QString& clientId) { m_clientId = clientId; }

    void sendMessage(const QJsonObject &message);
    void addActionHandler(const QString& action, std::unique_ptr<IActionHandler> handler);

Q_SIGNALS:
    void errorOccurred(const QString &errorString);

private slots:
    void onRemoteModeChanged(bool remoteMode);
    void onCurrentProjectChanged(ProjectModel* project);
    void onConnected();
    void onDisconnected();
    void onProjectAdded(ProjectModel* project);
    void onTextMessageReceived(const QString &message);
    void attemptReconnection();
    void onError(QAbstractSocket::SocketError error);

private:
    explicit WebSocketClient(const QUrl &url, bool debug, QObject *parent = nullptr);

    void connectToServer();
    void disconnectFromServer();

    std::unordered_map<QString, std::unique_ptr<IActionHandler>> m_actionHandlers;
    QString m_clientId;
    QUrl m_url;
    QWebSocket m_webSocket;
    bool m_debug;
    QTimer *m_reconnectTimer;
    GlobalState &m_globalState;
};
