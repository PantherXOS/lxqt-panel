//
// Created by Fakhri on 5/19/19.
//

#ifndef LXQT_PANEL_ACCOUNTOBJECT_H
#define LXQT_PANEL_ACCOUNTOBJECT_H
using namespace std;

#include <string>

    enum Status {
        none ,
        online,
        offline,
        error,
    };

class AccountObject {
public:
    string getTime() const {
        return time;
    }

    void setTime(string time) {
        AccountObject::time = time;
    }

    int getUnread() const {
        return unread;
    }

    void setUnread(int unread) {
        AccountObject::unread = unread;
    }

    const string &getType() const {
        return type;
    }

    void setType(const string &type) {
        AccountObject::type = type;
    }

    const string getIcon() const {
        return icon;
    }

    void setIcon(const string icon) {
        AccountObject::icon = icon;
    }

    const string getLink() const {
        return link;
    }

    void setLink(const string link) {
        AccountObject::link = link;
    }

    const string getAltLink() const {
        return altLink;
    }

    void setAltLink(const string altLink) {
        AccountObject::altLink = altLink;
    }

    const string getTitle() const {
        return title;
    }

    void setTitle(const string title) {
        AccountObject::title = title;
    }

    Status getStatus() const {
        return status;
    }

    void setStatus(Status status) {
        AccountObject::status = status;
    }

private:
    int unread;
    string time, type,icon,link,altLink,title;
    Status  status;

};


#endif //LXQT_PANEL_ACCOUNTOBJECT_H
