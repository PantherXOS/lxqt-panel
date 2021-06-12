//
// Created by fakhri on 06/09/21.
//


#ifndef LXQT_PANEL_CALENDARITEMWIDGET_H
#define LXQT_PANEL_CALENDARITEMWIDGET_H

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
#include <QColor>
#include "Settings.h"
#include "TimeBoxObject.h"

class CalendarItemWidget : public QWidget {
Q_OBJECT
public:
    CalendarItemWidget(const QString &startTime, const QString &middleTime, const QString &endTime, const string &color,int width, QWidget *parent = nullptr) : QWidget(parent) {
        auto coloredPart = new QLabel;
        // QPalette _palette;
        auto message = new QLabel;
        QFont calendarFont = message->font();
        calendarFont.setPointSize(MSG_PREVIEW_FONT_SIZE);
        
        //_palette.setColor(QPalette::Window, color);        
        coloredPart->setAutoFillBackground(true);  
        //coloredPart->setPalette(_palette);  
        coloredPart->setFixedWidth(width);
        coloredPart->setFixedHeight(10);
        coloredPart->setStyleSheet(QString::fromStdString("QLabel{border: 0px ; background-color:"+color+"}"));
        

        auto start = new QLabel;
        start->setText(startTime);
        start->setFont(calendarFont);
        start->setStyleSheet(QString::fromStdString("QLabel{color:rgb(124,127,132)}"));
        start->setAlignment(Qt::AlignLeft);

        auto middle = new QLabel;
        middle->setText(middleTime);
        middle->setFont(calendarFont);
        middle->setAlignment(Qt::AlignCenter);
        middle->setStyleSheet(QString::fromStdString("QLabel{color:rgb(124,127,132)}"));

        auto end = new QLabel;
        end->setText(endTime);
        end->setFont(calendarFont);
        //end->set
        end->setAlignment(Qt::AlignRight);
        end->setStyleSheet(QString::fromStdString("QLabel{color:rgb(124,127,132)}"));

        auto timelayout = new QHBoxLayout;
        timelayout->addWidget(start);
        timelayout->addWidget(middle);
        timelayout->addWidget(end);


        auto llayout = new QVBoxLayout;
        llayout->addWidget(coloredPart);
        llayout->addLayout(timelayout);
        llayout->setMargin(0);
        llayout->setSpacing(0);
        llayout->setContentsMargins(0,0,0,0);
        
        setLayout(llayout);
        setFixedWidth(width);
    }    
 
private:
 
};


#endif //LXQT_PANEL_CALENDARITEMWIDGET_H
