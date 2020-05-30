//
// Created by fakhri on 5/11/20.
//

#ifndef LXQT_PANEL_CHECKUPDATE_H
#define LXQT_PANEL_CHECKUPDATE_H

#include <qobjectdefs.h>
#include <QObject>
#include <thread>
#include <string>
#include <iostream>
#include <QDebug>
#include <sstream>
#include <iostream>
#include <QVector>
#include <string>
using namespace std;

class CheckUpdate : public QObject {
    Q_OBJECT
    public:
        void run();
        void stop();

    signals:
        void checkUpdateReady(QString pkgs);

    private:
        string exec(const char* cmd);
        volatile bool isRun;
        std::thread statThread;
};


#endif //LXQT_PANEL_CHECKUPDATE_H
