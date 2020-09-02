//
// Created by Hamzeh Nasajpour on 2/09/20.
//

#ifndef LXQT_PANEL_PX_ICON_H
#define LXQT_PANEL_PX_ICON_H

#include <string>
#include <iostream>
#include <QIcon>
#include <QLabel>

class PxIcon : public QLabel
{
    Q_OBJECT
public:
    PxIcon(QString file, QSize size, QWidget *parent = nullptr) : QLabel(parent){
        QIcon qicon;
        QImage image(file);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
        setAttribute(Qt::WA_TranslucentBackground);
        setPixmap(pixmap);
        setFixedSize(size);
    }
};


#endif //LXQT_PANEL_NETWORKINFORMATION_H
