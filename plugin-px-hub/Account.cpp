//
// Created by Fakhri on 5/19/19.
//

#include "Account.h"

void Account::setTime(int time) {
    this->time = time;

}

void Account::setUnread(int unread) {
    this->unread = unread;

}

void Account::setType(string type) {
    this->type = type;

}

void Account::setIcon(string icon) {
    this->icon = icon;

}

void Account::setLink(string link) {
    this->link = link;

}

void Account::setAltLink(string altLink) {
    this->altLink = altLink;

}

void Account::setTitle(string title) {
    this->title = title;

}

void Account::setStatus(Status status) {
    this->status = status;

}

int Account::getTime() {
    return this->time;
}

int Account::getUnread() {
    return this->unread;
}

string Account::getType() {
    return this->type;
}

string Account::getIcone() {
    return this->icon;
}

string Account::getLink() {
    return this->link;
}

string Account::getAltLink() {
    return this->altLink;
    }

string Account::getTitle() {
    return title;
}

Status Account::getStatus() {
    return this->status;
}
