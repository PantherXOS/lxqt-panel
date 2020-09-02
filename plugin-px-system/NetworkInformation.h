//
// Created by fakhri on 3/30/20.
//

#ifndef LXQT_PANEL_NETWORKINFORMATION_H
#define LXQT_PANEL_NETWORKINFORMATION_H

#include <QObject>
#include <string>
#include <iostream>
#include <QString>
using namespace std;

class NetworkInformation{
public:
    NetworkInformation(){
        qRegisterMetaType<NetworkInformation>("NetworkInformation");
        qRegisterMetaType<QVector<NetworkInformation>>("QVector<NetworkInformation>");
    }

    QString name;
    QString value;
    bool status = false;
    QString profileName;
    bool wifiStatus = false;
    bool vpnStatus = false;
};


#endif //LXQT_PANEL_NETWORKINFORMATION_H
