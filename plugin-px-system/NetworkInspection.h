//
// Created by fakhri on 3/31/20.
//

#ifndef LXQT_PANEL_NETWORKINSPECTION_H
#define LXQT_PANEL_NETWORKINSPECTION_H


class NetworkInspection {
public:
    void run();
private:
    string exec(const char* cmd);
};


#endif //LXQT_PANEL_NETWORKINSPECTION_H
