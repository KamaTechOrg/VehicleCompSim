#pragma once

#include <QObject>
#include <QWebSocket>
#include <QSettings>
#include <QUrl>
#include <QTimer>
#include "observer.h"
#include "items/serializableitem.h"
#include "gui/customscene.h"

class SerializableItem;

class WebSocketClient : public QObject, public Observer
{
    Q_OBJECT
public:
    static WebSocketClient& getInstance(const QUrl &url = QUrl(QStringLiteral("ws://104.131.73.137:28770")), bool debug = false);

    WebSocketClient(const WebSocketClient&) = delete;
    WebSocketClient& operator=(const WebSocketClient&) = delete;

    void setupSslConfiguration();

    void onItemModified(const SerializableItem& item) override;
    void onItemAdded(const SerializableItem& item) override;
    void onItemDeleted(const SerializableItem& item) override;

    const QString& getClientId() const { return m_clientId; }
    void setScene(CustomScene* scene) { m_scene = scene; }

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
    void sendMessage(const QJsonObject &message);

    QString m_clientId;
    QUrl m_url;
    QWebSocket m_webSocket;
    // QSslConfiguration m_sslConfiguration;
    CustomScene* m_scene;
    bool m_debug;
    QTimer *m_reconnectTimer;
};