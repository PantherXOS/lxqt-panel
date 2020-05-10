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


class ResultItem : public QWidgetAction{
Q_OBJECT
public:
    ResultItem(QString name, QString type, QString address, QFont mfont, QObject *parent = nullptr)
            : QWidgetAction(parent) {
        std::string _type = type.toStdString();
        auto title = new QLabel;
        setObjectName(QString::fromStdString("ActionView"));
        title->setText(name);
        title->setFont(mfont);
        std::replace( _type.begin(), _type.end(), '/', '-');
        this->name = name;
        this->type = QString::fromStdString(_type);
        this->address = address;
        const int icon_size = QFontMetrics(mfont).height()*0.8;
        auto itemIcon = buildIconFromTheme(type,QSize(icon_size,icon_size));
        auto Tlayout = new QHBoxLayout;
        Tlayout->addWidget(itemIcon);
        int left,right,top,buttom;
        itemIcon->getContentsMargins(&left,&top,&right,&buttom);
        Tlayout->addWidget(title);
        //if(!searchPart)
           // Tlayout->setContentsMargins(2,2,2,2);
        Tlayout->setAlignment(Qt::AlignLeft);

        resultWidget = new QWidget;
        resultWidget->setMouseTracking(true);
        resultWidget->setLayout(Tlayout);
        resultWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
        resultWidget->setObjectName(QString::fromStdString("PxMenuItem"));
        setDefaultWidget(resultWidget);
        setText(name);
        address = address.remove(0,7);
        setToolTip(address);
    }
    void open(){
        QDesktopServices::openUrl(QUrl(this->address));
    }

    QLabel * buildIconFromTheme(QString icon, QSize size){
        QIcon qicon = QIcon::fromTheme(icon);
        if(qicon.name().isEmpty())
            qicon = QIcon::fromTheme(QString::fromStdString("unknown"));
        QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
        auto iconLabel = new QLabel;
        iconLabel->setAttribute(Qt::WA_TranslucentBackground);
        iconLabel->setPixmap(pixmap);
        iconLabel->setFixedSize(size);
        return iconLabel;
    }

    QString toString(){
        return  QString::fromStdString("Name   : ") + name + QString::fromStdString(" ,")+
                QString::fromStdString("Type    : ") + type + QString::fromStdString(" ,")+
                QString::fromStdString("Address : ") + address;
    }

private:
    QString name;
    QString address;
    QString type;
    QWidget*  resultWidget;


};


#endif //LXQT_PANEL_RESULTITEM_H
