//
// Created by sergeyk on 03.07.25.
//

#pragma once

#include <QMainWindow>
#include <QUdpSocket>
#include "qcustomplot.h"

class UdpServer : public QMainWindow {
    Q_OBJECT
public:
    explicit UdpServer(QWidget *parent = nullptr);

private slots:
    void ReadData();

private:
    QUdpSocket *socket;
    QCustomPlot *plot;
    void SetupUi();
};
