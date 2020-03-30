//
// Created by fakhri on 3/30/20.
//

#ifndef LXQT_PANEL_NETWORKINFORMATION_H
#define LXQT_PANEL_NETWORKINFORMATION_H

#include <string>
#include <iostream>
using namespace std;

class NetworkInformation {
public:
    string name;
    string value;
    bool status = false;
    string profileName;
    bool wifiStatus = false;
    bool vpnStatus = false;
};


#endif //LXQT_PANEL_NETWORKINFORMATION_H
