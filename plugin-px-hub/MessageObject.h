//
// Created by fakhri on 12/15/19.
//

#ifndef LXQT_PANEL_MESSAGEOBJECT_H
#define LXQT_PANEL_MESSAGEOBJECT_H
#include <string>
#include <iostream>
using namespace std;
class MessageObject{
private:
    string time, message, icon, sender, link, altLink;
    bool unread;
public:
    const string toString(){
        string _str = "{" + sender + ", " + time + ", " + icon + ", " + link + ", " + altLink
                + ", " + message + ", " + to_string(unread) +"}";
        return _str;
    }

    const string &getLink() const {
        return link;
    }

    void setLink(const string &link) {
        MessageObject::link = link;
    }

    const string &getAltLink() const {
        return altLink;
    }

    void setAltLink(const string &altLink) {
        MessageObject::altLink = altLink;
    }

    const string &getTime() const {
        return time;
    }

    void setTime(const string &time) {
        MessageObject::time = time;
    }

    const string &getMessage() const {
        return message;
    }

    void setMessage(const string &_message) {
        this->message = _message;
    }

    const string getIcon() const {
        return icon;
    }

    void setIcon(const string icon) {
        MessageObject::icon = icon;
    }

    const string getSender() const {
        return sender;
    }

    void setSender(const string sender) {
        MessageObject::sender = sender;
    }

    bool isUnread(){
        return unread;
    }

    void setUnread(bool unread){
        MessageObject::unread = unread;
    }
};

#endif //LXQT_PANEL_MESSAGEOBJECT_H
