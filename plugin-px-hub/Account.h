//
// Created by root on 5/19/19.
//

#ifndef LXQT_PANEL_ACCOUNT_H
#define LXQT_PANEL_ACCOUNT_H
using namespace std;

#include <string>

    enum Status {
        none ,
        online,
        offline,
        error,
    };



class Account {
public:
    void setTime(int time);
    void setUnread(int unread);
    void setType(string type);
    void setIcon(string icon);
    void setLink(string link);
    void setAltLink(string altLink);
    void setTitle(string title);
    void setStatus(Status status);

    int getTime();
    int getUnread();
    string getType();
    string getIcone();
    string getLink();
    string getAltLink();
    string getTitle();
    Status getStatus();

private:
    int time,unread;
    string type,icon,link,altLink,title;
     Status  status;

};


#endif //LXQT_PANEL_ACCOUNT_H
