#include "RunHandler.h"
#include <QDebug>

RunHandler::RunHandler(const std::function<void()> startFunction, const std::function<void()> stopFunction)
  : m_startFunction(startFunction), m_stopFunction(stopFunction)
{
}

void RunHandler::handle(const QJsonObject &message)
{
  // Extract the run command from the message
  QString command = message["command"].toString();
  if (command == "start") {
    m_startFunction();
    qDebug() << "Run started";
  } else if (command == "stop") {
    m_stopFunction();
    qDebug() << "Run stopped";
  }
}
