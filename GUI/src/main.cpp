#include "websocketclient.h"

#include <QApplication>
#include <QUrl>

#include "gui/mainwindow.h"
#include "observerconfig.h"
#include "services/qtlogger.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

    QtLogger::initialize();

    ObserverConfig::setObserver(&WebSocketClient::getInstance());

	MainWindow w;
    w.resize(800, 600);
    w.show();
	return app.exec();
}
