//
// Created by fakhri on 06/09/21.
//

#ifndef LXQT_PANEL_TIMEBOXOBJECT_H
#define LXQT_PANEL_TIMEBOXOBJECT_H

#include <QString>
#include <QDateTime>

using namespace std;

class TimeBoxObject{
private:
    QDateTime startTime, endTime;
    QString message;

public:
    TimeBoxObject(const QDateTime &start, const QDateTime &end, const QString &msg):
        startTime(start),
        endTime(end),
        message(msg){    }

    const QDateTime &getStartTime() const {
        return startTime;
    }

    const QDateTime &getEndTime() const {
        return endTime;
    }

    void setStartTime(const QDateTime &time) {
        startTime = time;
    }

    void setEndTime(const QDateTime &time) {
        endTime = time;
    }

    const QString &getMessage() const {
        return message;
    }

    void setMessage(const QString &_message) {
        this->message = _message;
    }
};

#endif //LXQT_PANEL_TIMEBOXOBJECT_H
