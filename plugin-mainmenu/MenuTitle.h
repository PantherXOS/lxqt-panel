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

class MenuTitle : public QWidgetAction{
Q_OBJECT
public:
    MenuTitle(QString title, QObject *parent = nullptr)
            : QWidgetAction(parent) {
        auto qlayout = new QHBoxLayout;
        auto titleLabel = new QLabel;
        titleLabel->setText(title);
        auto _font = titleLabel->font();
        QFont  font = _font;
        font.setBold(true);
        titleLabel->setFont(font);
        qlayout->addWidget(titleLabel);

        auto  widget = new QWidget;
        widget->setLayout(qlayout);

        setDefaultWidget(widget);
        setText(title);
    }
};


#endif //LXQT_PANEL_MENUTITLE_H
