//
// Created by fakhri on 3/4/20.
//

#ifndef LXQT_PANEL_RESULTITEM_H
#define LXQT_PANEL_RESULTITEM_H

#include <QString>

class ResultItem {

public:
    QString name;
    QString address;
    QString type;

    QString toString(){
        return  "Name   : "+ name +" ,"+
                "Type    : "+ type +" ,"+
               "Address : "+address;
    }
};


#endif //LXQT_PANEL_RESULTITEM_H
