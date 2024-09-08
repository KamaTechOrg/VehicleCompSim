#pragma once

#include <unordered_map>
#include <memory>

#include <QObject>
#include <QWebSocket>
#include <QSettings>
#include <QUrl>
#include <QTimer>
#include "observer.h"
#include "items/serializableitem.h"
#include "gui/customscene.h"

#include "IActionHandler.h"
#include "IdentifyHandler.h"
#include "AddItemHandler.h"
#include "ModifyItemHandler.h"
#include "DeleteItemHandler.h"


class SerializableItem;

class WebSocketClient : public QObject, public Observer
{
    Q_OBJECT
public:
    static WebSocketClient& getInstance(const QUrl &url = QUrl(QStringLiteral("ws://localhost:8765")), bool debug = true);

    WebSocketClient(const WebSocketClient&) = delete;
    WebSocketClient& operator=(const WebSocketClient&) = delete;

    void onItemModified(const SerializableItem& item) override;
    void onItemAdded(const SerializableItem& item) override;
    void onItemDeleted(const SerializableItem& item) override;

    const QString& getClientId() const { return m_clientId; }
    void setClientId(const QString& clientId) { m_clientId = clientId; }
    void setScene(CustomScene* scene);

    void sendMessage(const QJsonObject &message);
    void addActionHandler(const QString& action, std::unique_ptr<IActionHandler> handler);
    
Q_SIGNALS:
    void closed();
    void connectionStatusChanged(bool connected);


private Q_SLOTS:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(const QString &message);
    void attemptReconnection();

private:
    explicit WebSocketClient(const QUrl &url, bool debug, QObject *parent = nullptr);
    
    void connectToServer();

    std::unordered_map<QString, std::unique_ptr<IActionHandler>> m_actionHandlers;
    QString m_clientId;
    QUrl m_url;
    QWebSocket m_webSocket;
    CustomScene* m_scene;
    bool m_debug;
    QTimer *m_reconnectTimer;
};
