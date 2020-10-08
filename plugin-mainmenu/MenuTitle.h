//
// Created by fakhri on 3/4/20.
//

#ifndef LXQT_PANEL_MENUTITLE_H
#define LXQT_PANEL_MENUTITLE_H

#include <QDebug>
#include <QString>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QtGui/QDesktopServices>
#include <QUrl>
#include "pxsettings.h"

class MenuTitle : public QWidgetAction{
Q_OBJECT
public:
    MenuTitle(const QString &title, bool setcolor=false, QObject *parent = nullptr)
            : QWidgetAction(parent) {
        auto qlayout = new QHBoxLayout;
        auto titleLabel = new QLabel;
        titleLabel->setText(title);
        auto _font = titleLabel->font();
        QFont  font = _font;
        font.setPointSize(MENU_TITLE_FONT_SIZE);

        titleLabel->setFont(font);
        titleLabel->setObjectName(QStringLiteral("PxMenuLabel"));
        qlayout->addWidget(titleLabel);
        qlayout->setSpacing(0);
        qlayout->setContentsMargins(0,1,5,1);
        auto  widget = new QWidget;
        widget->setLayout(qlayout);
        if(setcolor) {
            widget->setObjectName(QStringLiteral("PxMenuTitle"));
        }
        setDefaultWidget(widget);
        setText(title);
    }
};


#endif //LXQT_PANEL_MENUTITLE_H
