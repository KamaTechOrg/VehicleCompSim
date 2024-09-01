#pragma once

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QList>
#include <memory>
#include <unordered_map>


#include "items/serializableitem.h"

class WebSocketServer : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketServer(quint16 port, bool debug = false, QObject *parent = nullptr);
    ~WebSocketServer();
    
    void setupSslConfiguration();
    void broadcastMessage(const QString &message);

signals:
    void newConnection(QWebSocket *socket);
    void messageReceived(QWebSocket *socket, const QString &message);

private slots:
    void onNewConnection();
    void processMessage(const QString &message);
    void socketDisconnected();

private:
    bool m_debug;
    QWebSocketServer *m_pWebSocketServer;
    // QSslConfiguration m_sslConfiguration;
    QMap<QString, QWebSocket*> m_clientsIds;

    std::unordered_map<QString, std::unique_ptr<SerializableItem>> items;

    QString generateClientId();
    SerializableItem* findItemById(const QString &id);
    void addSerializableItem(std::unique_ptr<SerializableItem> item);
};
