#ifndef LXQT_PANEL_ACCOUNTITEM_H
#define LXQT_PANEL_ACCOUNTITEM_H

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
#include <XdgIcon>
#include <string>
#include <QtWidgets/QWidgetAction>
#include <QScrollBar>
#include <QFrame>
#include <QFont>
#include <QIcon>
#include "AccountObject.h"
#include "Settings.h"

class AccountItemWidget : public QWidget {
public:
    AccountItemWidget(AccountObject &account, QWidget *parent = nullptr) : QWidget(parent) {
        QString statusIconFile;
        if(account.getStatus() == Status::online)
            statusIconFile=QString::fromStdString(":resources/icon/status_0_green_o");
        else if(account.getStatus() == Status::offline)
            statusIconFile=QString::fromStdString(":resources/icon/status_0_red");
        else if(account.getStatus() == Status::none)
            statusIconFile=QString::fromStdString(":resources/icon/status_0_grey");
        auto statusLabel = buildIconFromFile(statusIconFile,QSize(ACCOUNT_STATUS_ICON_SIZE,ACCOUNT_STATUS_ICON_SIZE));
        auto accountIcon = buildIconFromTheme(QString::fromStdString(account.getIcon()),QSize(ACCOUNT_ICON_SIZE,ACCOUNT_ICON_SIZE));
        accountIcon->setContentsMargins(3,0,0,0);
        auto accountTitle = new QLabel;
        QFont accountTitleFont = accountTitle->font();
        accountTitleFont.setPointSize(ACCOUNT_TITLE_FONT_SIZE);
        accountTitle->setFont(accountTitleFont);

        string acc = account.getTitle();
        if(acc.size()>MAX_ACCOUNT_SIZE)
            acc = acc.substr(0,MAX_ACCOUNT_SIZE)+"...";
        accountTitle->setText(QString::fromStdString(acc));
        accountTitle->setMargin(0);
        accountTitle->setContentsMargins(5,0,0,0);

        auto unreadCount = new QLabel;
        unreadCount->setText(QString::fromStdString(to_string(account.getUnread())));
        if(account.getUnread() == 0)
            unreadCount->setVisible(false);
        accountIcon->setContentsMargins(0,0,0,0);

        auto llayout = new QHBoxLayout;
        llayout->addWidget(accountIcon);
        llayout->addWidget(accountTitle);
        llayout->setAlignment(Qt::AlignLeft);
        llayout->setMargin(0);
        llayout->setSpacing(0);
        llayout->setContentsMargins(0,0,0,0);

        auto rlayout = new QHBoxLayout;
        rlayout->addWidget(unreadCount);
        rlayout->addWidget(statusLabel);
        rlayout->setAlignment(Qt::AlignRight);
        rlayout->setMargin(0);
        rlayout->setSpacing(0);
        rlayout->setContentsMargins(0,0,0,0);

        auto qlayout = new QHBoxLayout;
        qlayout->addLayout(llayout);
        qlayout->addLayout(rlayout);
    //    qlayout->setMargin(0);
        qlayout->setSpacing(0);
    //    qlayout->setContentsMargins(0,2,0,2);

        setObjectName(QString::fromStdString("PxHubItem"));
        setLayout(qlayout);
        setAttribute(Qt::WA_Hover);
        setMouseTracking(true);
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

    QLabel *buildIconFromFile(QString file, QSize size){
        QIcon qicon;
        QImage image(file);
        qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
        QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
        auto iconLabel = new QLabel;
        iconLabel->setAttribute(Qt::WA_TranslucentBackground);
        iconLabel->setPixmap(pixmap);
        iconLabel->setFixedSize(size);
        return iconLabel;
    }
};

class AccountItem : public QWidgetAction{
Q_OBJECT
public:
    AccountItem(AccountObject &account, QObject *parent = nullptr)
            : QWidgetAction(parent) {
        auto widget = new AccountItemWidget(account);
        setDefaultWidget(widget);
    }
};



#endif //LXQT_PANEL_ACCOUNTITEM_H
