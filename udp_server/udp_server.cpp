//
// Created by sergeyk on 03.07.25.
//

#include "udp_server.h"
#include <QDateTime>

UdpServer::UdpServer(QWidget *parent) : QMainWindow(parent) {
    SetupUi();
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 1234);
    connect(socket, &QUdpSocket::readyRead, this, &UdpServer::ReadData);
}

void UdpServer::SetupUi() {
    plot = new QCustomPlot(this);
    plot->addGraph();
    plot->graph(0)->setPen(QPen(Qt::blue));
    plot->xAxis->setLabel("Time (s)");
    plot->yAxis->setLabel("CPU Usage (%)");
    plot->yAxis->setRange(0, 100);
    setCentralWidget(plot);
}

void UdpServer::ReadData() {
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size());

        float usage = datagram.toFloat();
        double time = QDateTime::currentSecsSinceEpoch();
        plot->graph(0)->addData(time, usage);
        plot->xAxis->setRange(time - 10, time);
        plot->replot();
    }
}
