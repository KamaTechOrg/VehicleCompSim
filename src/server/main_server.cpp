#include "websocketserver.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	WebSocketServer server(1234, true, nullptr);
	
	return app.exec();
}