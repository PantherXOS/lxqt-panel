//
// Created by fakhri on 3/31/20.
//

#ifndef LXQT_PANEL_NETWORKINSPECTION_H
#define LXQT_PANEL_NETWORKINSPECTION_H

#include <QWidget>
#include <QWidgetAction>
#include <QObject>
#include <string>
#include <stdio.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QVector>
#include <QDebug>

#include <rapidjson/document.h>
#include "NetworkInformation.h"
#include "PxIcon.h"
#include "Settings.h"
#include <thread>

using namespace std;
using namespace rapidjson;

class NetworkInspection  : public QWidgetAction {
Q_OBJECT
public:
    NetworkInspection(QObject *parent = nullptr);
    void run();
    NetworkInformation getVpnStatus();
    NetworkInformation getWifiStatus();

signals:
    void networkInfoIsReady(const QVector <NetworkInformation> &netInfos);

private:
    QVector<NetworkInformation> parseResponse(string data);
    string exec(const char* cmd);
    std::thread statThread;
    NetworkInformation vpnStatus , wifiStatus;
};
#endif //LXQT_PANEL_NETWORKINSPECTION_H
