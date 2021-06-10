//
// Created by fakhri on 06/09/21.
//

#ifndef LXQT_PANEL_CALENDARBOX_H
#define LXQT_PANEL_CALENDARBOX_H


#include <QDebug>
#include <QString>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QtGui/QDesktopServices>
#include <QUrl>
#include <QMouseEvent>
#include <QDateTime>
#include <string>
#include <QtWidgets/QWidgetAction>
#include <QScrollBar>
#include <QFrame>
#include <QFont>
#include <QIcon>
#include <QListWidget>

#include "TimeBoxObject.h"
#include "CalendarItemWidget.h"



class CalendarBox : public QWidget{
Q_OBJECT
public:
    CalendarBox(const QVector<TimeBoxObject> &calendarItemWidgetList, QWidget *parent = nullptr)
            : QWidget(parent) {        
        auto mainLayout = new QHBoxLayout;
        // for(auto calendar : calendarItemWidgetList){
        //     mainLayout->addWidget(calendar);
        // }
        QColor green(Qt::green);
        QColor yellow(Qt::yellow);       
        auto test = new CalendarItemWidget(QString::fromStdString("12:30"),QString::fromStdString(" "),QString::fromStdString(" "),green,130);
        auto mmidletest = new CalendarItemWidget(QString::fromStdString(" "),QString::fromStdString("2h"),QString::fromStdString(" "),yellow,60);
        auto endtest = new CalendarItemWidget(QString::fromStdString(" "),QString::fromStdString(" "),QString::fromStdString("17:30"),green,130);
        mainLayout->addWidget(test);
        mainLayout->addWidget(mmidletest);
        mainLayout->addWidget(endtest);
        mainLayout->setMargin(0);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(6,4,0,2);

        auto calendarIcon = buildIconFromTheme(QString::fromStdString("px-categories"), QSize(MESSAGE_ICON_SIZE,MESSAGE_ICON_SIZE));
        //calendarIcon->setContentsMargins(3,0,0,0);
        auto finalLayout = new QHBoxLayout;
        finalLayout->addWidget(calendarIcon);
        finalLayout->addLayout(mainLayout);
        finalLayout->setAlignment(Qt::AlignLeft);
        finalLayout->setMargin(0);
        finalLayout->setSpacing(0);
        finalLayout->setContentsMargins(4,0,0,0);

        setLayout(finalLayout);
    }

       QLabel *buildIconFromTheme(QString icon, QSize size){
        QIcon qicon = QIcon::fromTheme(icon);
        QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
        auto iconLabel = new QLabel;
        iconLabel->setAttribute(Qt::WA_TranslucentBackground);
        iconLabel->setPixmap(pixmap);
        iconLabel->setFixedSize(size);
        return iconLabel;
    }
};

class CalendarBoxWidgetAction : public QWidgetAction{
Q_OBJECT
public:
    CalendarBoxWidgetAction(const QVector<TimeBoxObject> &calendarItemWidgetList, QObject *parent = nullptr)
            : QWidgetAction(parent) {
        auto widget = new CalendarBox(calendarItemWidgetList);
        setDefaultWidget(widget);
        
    }
};

#endif //LXQT_PANEL_CALENDARBOX_H