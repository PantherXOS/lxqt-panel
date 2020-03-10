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
class ResultItem : public QWidgetAction{
Q_OBJECT
public:
    ResultItem(QString name, QString type, QString address,QFont mfont, QObject *parent) : QWidgetAction(parent) {
        auto title = new QLabel;
        title->setText(name);
        title->setFont(mfont);
        std::replace( type.begin(), type.end(), '/', '-');
        this->name = name;
        this->type = type;
        this->address = address;
        const int icon_size = QFontMetrics(mfont).height()*0.8;
        auto itemIcon = buildIconFromTheme(type,QSize(icon_size,icon_size));
        auto Tlayout = new QHBoxLayout;
        Tlayout->addWidget(itemIcon);
        int left,right,top,buttom;
        itemIcon->getContentsMargins(&left,&top,&right,&buttom);
        //itemIcon->setContentsMargins(left,top,5,buttom);
        Tlayout->addWidget(title);
        Tlayout->setAlignment(Qt::AlignLeft);
//        Tlayout->setMargin(0);
//        Tlayout->setSpacing(0);
//        Tlayout->setContentsMargins(5,0,0,0);

        auto  resultWidget = new QWidget;
        resultWidget->setLayout(Tlayout);
        resultWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
        setDefaultWidget(resultWidget);
        setText(name);
    }

    QLabel * buildIconFromTheme(QString icon, QSize size){
        QIcon qicon = QIcon::fromTheme(icon);
        QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
        auto iconLabel = new QLabel;
        iconLabel->setAttribute(Qt::WA_TranslucentBackground);
        iconLabel->setPixmap(pixmap);
        iconLabel->setFixedSize(size);
        return iconLabel;
    }
    QString toString(){
        return  "Name   : "+ name +" ,"+
                "Type    : "+ type +" ,"+
                "Address : "+address;
    }

private:
    QString name;
    QString address;
    QString type;
};


#endif //LXQT_PANEL_RESULTITEM_H
