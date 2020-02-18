//
// Created by fakhri on 12/29/19.
//

#ifndef LXQT_PANEL_EVENTOBJECT_H
#define LXQT_PANEL_EVENTOBJECT_H

#include <QObject>
#include <string>
using namespace std;

class EventObject : public QObject {
public:
//    EventObject(QObject *parent= nullptr) : QObject(parent){
//
//    }

    const QString getTopic() const {
        return topic;
    }

    void setTopic(const QString topic) {
        this->topic = topic;
    }

    const QString getSource() const {
        return source;
    }

    void setSource(const QString source) {
        this->source = source;
    }

    const QString getEvent() const {
        return event;
    }

    void setEvent(const QString event) {
        this->event = event;
    }

    uint64_t getTime() const {
        return time;
    }

    void setTime(uint64_t time) {
        this->time = time;
    }


    const map<QString,QString> getParams() const {
        return params;
    }

    void setParams(const map<QString,QString> params) {
        this->params = params;
    }

    QString toString(){
        QString str = "Event: \n\r\ttopic : "+ topic + "\n\r\t" +
                     "time  : " + to_string(time).c_str() + "\n\r\t" +
                     "source: " + source + "\n\r\t" +
                     "event : " + event  + "\n\r\t" +
                     "params: " + "\n\r\t\t";
        for (auto p : params)
            str += "{ " + p.first + " : " + p.second + " }\n\r\t\t";
        return str;
    }

private:
    QString topic,source,event;
    map<QString,QString> params;
    uint64_t time;
};
#endif //LXQT_PANEL_EVENTOBJECT_H
