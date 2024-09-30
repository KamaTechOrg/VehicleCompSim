#include "RunHandler.h"
#include <QDebug>
#include "ClientConstants.h"

RunHandler::RunHandler(const std::function<void(const QString&)> startFunction, const std::function<void()> stopFunction)
  : m_startFunction(startFunction), m_stopFunction(stopFunction)
{
}

void RunHandler::handle(const QJsonObject &message)
{
  // Extract the run command from the message
  QString command = message[ClientConstants::KEY_COMMAND].toString();
  if (command == ClientConstants::COMMAND_START) {
    QString timer = message["timer"].toString();
    m_startFunction(timer);
    qDebug() << "Run started";
  } else if (command == ClientConstants::COMMAND_STOP) {
    m_stopFunction();
    qDebug() << "Run stopped";
  }
}
