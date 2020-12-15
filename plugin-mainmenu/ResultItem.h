//
// Created by fakhri on 3/4/20.
//

#ifndef LXQT_PANEL_RESULTITEM_H
#define LXQT_PANEL_RESULTITEM_H

#include <QDebug>
#include <QString>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QtGui/QDesktopServices>
#include <QUrl>
#include <QMouseEvent>

#include "pxsettings.h"

class ResultItem : public QAction{
Q_OBJECT
public:
    ResultItem(const QString &name, const QString & type, const QString & address, QObject *parent = nullptr)
            : QAction(parent) {
        std::string _type = type.toStdString();
        if(name.size()>MAX_SEARCH_RESULT_FILE_NAME)
            setText(name.left(MAX_SEARCH_RESULT_FILE_NAME-4) + QString::fromStdString("..."));
        else 
            setText(name);
        std::replace( _type.begin(), _type.end(), '/', '-');
        this->name = name;
        this->type = QString::fromStdString(_type);
        this->address = address;
        setIcon(QIcon::fromTheme(QString::fromStdString(_type),QIcon::fromTheme(QString::fromStdString("unknown"))));
        setToolTip(address);
    }

    void open(){
        QDesktopServices::openUrl(QUrl(this->address));
    }

    QString toString(){
        return  QString::fromStdString("Name    : ") + name + QString::fromStdString(" ,")+
                QString::fromStdString("Type    : ") + type + QString::fromStdString(" ,")+
                QString::fromStdString("Address : ") + address;
    }

private:
    QString name;
    QString address;
    QString type;
};


#endif //LXQT_PANEL_RESULTITEM_H
