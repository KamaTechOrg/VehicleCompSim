// WebSocketConstants.h

#pragma once

#include <QString>

namespace ClientConstants {

// Action names
inline const QString ACTION_IDENTIFY = "identify";
inline const QString ACTION_ADD = "add";
inline const QString ACTION_MODIFY = "modify";
inline const QString ACTION_DELETE = "delete";
inline const QString ACTION_PROJECT = "project";
inline const QString ACTION_GET_POJECT_ITEMS = "getProjectItems";
inline const QString ACTION_UPDATE_CLIENT_ID = "updateClientId";
inline const QString ACTION_RUN = "run";

// Command names
inline const QString COMMAND_START = "start";
inline const QString COMMAND_STOP = "stop";

// JSON keys
inline const QString KEY_ID = "id";
inline const QString KEY_NAME = "name";
inline const QString KEY_ACTION = "action";
inline const QString KEY_COMMAND = "command";
inline const QString KEY_CLIENT_ID = "clientId";
inline const QString KEY_UPDATED_CLIENT_ID = "updatedClientId";
inline const QString KEY_PROJECT_ID = "projectId";
inline const QString KEY_TYPE = "type";

// Other constants
inline const int RECONNECT_INTERVAL = 5000; 

} 