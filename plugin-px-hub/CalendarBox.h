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

#define GREEN_COLOR "rgb(45,133,55)"
#define YELLOW_COLOR "rgb(248,166,30)"
#define FONT_GRAY  "rgb(124,127,132)"



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
        auto test = new CalendarItemWidget(QString::fromStdString("13:32"),QString::fromStdString(" "),QString::fromStdString(" "),GREEN_COLOR,60);
        auto mmidletest1 = new CalendarItemWidget(QString::fromStdString(" "),QString::fromStdString("+1h"),QString::fromStdString(" "),YELLOW_COLOR,70);
        auto mmidletest2 = new CalendarItemWidget(QString::fromStdString(" "),QString::fromStdString("+2h"),QString::fromStdString(" "),GREEN_COLOR,80);
        auto mmidletest3 = new CalendarItemWidget(QString::fromStdString(" "),QString::fromStdString("+3h"),QString::fromStdString(" "),YELLOW_COLOR,33);
        auto endtest = new CalendarItemWidget(QString::fromStdString(" "),QString::fromStdString(" "),QString::fromStdString("18:00"),GREEN_COLOR,75);
        mainLayout->addWidget(test);
        mainLayout->addWidget(mmidletest1);
        mainLayout->addWidget(mmidletest2);
        mainLayout->addWidget(mmidletest3);
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