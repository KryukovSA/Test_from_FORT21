//
// Created by sergeyk on 03.07.25.
//

#include <QApplication>

#include "udp_server.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    UdpServer server;
    server.resize(800, 400);
    server.show();
    return app.exec();
}
