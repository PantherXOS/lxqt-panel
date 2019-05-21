/* BEGIN_COMMON_COPYRIGHT_HEADER
 * (c)LGPL2+
 *
 * LXQt - a lightweight, Qt based, desktop toolset
 * https://lxqt.org
 *
 * Copyright: 2012 Razor team
 * Authors:
 *   Matteo Fois <giomatfois62@yahoo.it>
 *
 * This program or library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 * END_COMMON_COPYRIGHT_HEADER */

#include "Hub.h"

#include <QMouseEvent>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <QStandardPaths>
#include <QSettings>
#include <QLabel>
#include <QDateTime>
#include <QMessageBox>
#include <QDir>
#include <QFontDialog>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QtGui/QPainter>

Hub::Hub(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo),
    mHidden(false)
{
    realign();
    vector <QAction *> qactVector;
    mainMenu->setFixedWidth(300);
    vector<Account> accounts=getAccount();
    mainMenu->addAction(createeTitle("YOUR ACCOUNTS"));
    for(Account s : accounts){
        mainMenu->addAction(buildAccountItem(s));
    }
    mainMenu->addSeparator();

    ServiceEventHandler hubEventHandler;
    EventSubscriber * eventSubscriber = new EventSubscriber("hub", &hubEventHandler,&mButton);
    eventSubscriber->run();
    mButton.setMenu(mainMenu);
    mButton.setPopupMode(QToolButton::InstantPopup);
    mButton.setAutoRaise(true);
    mButton.setIcon(QIcon(":resources/icon/user"));
}


Hub::~Hub()
{
    for(auto it = mNotes.begin(); it != mNotes.end(); ++it)
        delete it.value();
    
    mNotes.clear();
}

void Hub::realign()
{
  mButton.setFixedHeight(panel()->iconSize());
  mButton.setFixedWidth(panel()->iconSize());
}


QString Hub::dataDir()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QDir::separator() + "lxqt-notes";
    return dir;
}


void Hub::toggleShowHide()
{


}

void Hub::setIconColor(const QString &icon, const QString &color)
{
    // create icons path
    QString iconsDir = dataDir() + QDir::separator() + "icons";
    QDir().mkpath(iconsDir);

    // set icon color
    QFile file(icon);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString svg = in.readAll().arg(color);

    file.close();

    QFileInfo info(icon);
    QString iconFile = iconsDir + QDir::separator() + info.fileName();

    QFile outFile(iconFile);
    if(!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&outFile);
    out << svg;

    outFile.close();
}

void Hub::settingsChanged()
{

}

QDialog* Hub::configureDialog()
{

}

vector<Account> Hub::getAccount() {
    vector<Account> rsult;
    string addr = string("unix:") + HUB_SERVER_ADDRESS;
    capnp::EzRpcClient rpcClient(addr);
    auto &waitScope = rpcClient.getWaitScope();
    HubReader::Client client = rpcClient.getMain<HubReader>();
    try {
        auto actReq = client.getAccountsStatusRequest();
        auto actRes = actReq.send().wait(waitScope);
        for(auto act : actRes.getAccountsStatus()) {
            Account account;
            switch (act.getStatus()){
                case AccountStatus::Status::NONE:
                    account.setStatus(Status::none);
                    break;
                case AccountStatus::Status::ERROR:
                    account.setStatus(Status::error);
                    break;
                case AccountStatus::Status::ONLINE:
                    account.setStatus(Status::online);
                    break;
                case AccountStatus::Status::OFFLINE:
                    account.setStatus(Status::offline);
                    break;
            }
            account.setTime(act.getTime());
            account.setUnread(act.getUnread());
            account.setType(act.getType().cStr());
            account.setIcon(act.getIcon().cStr());
            account.setLink(act.getLink().cStr());
            account.setAltLink(act.getAltLink().cStr());
            account.setTitle(act.getTitle().cStr());

            rsult.push_back(account);
        }
    } catch (kj::Exception){

    }
    if(rsult.size()==0) {
        Account acc;
        acc.setTitle("Fakhri");
        acc.setUnread(10);
        acc.setStatus(Status::online);
        rsult.push_back(acc);

        acc.setTitle("Franz");
        acc.setUnread(2);
        acc.setStatus(Status::offline);
        rsult.push_back(acc);
    }
    return rsult;
}

QWidgetAction* Hub::buildAccountItem(Account account) {
    QHBoxLayout *qlayout = new QHBoxLayout;
    QHBoxLayout *llayout = new QHBoxLayout;
    QHBoxLayout *rlayout = new QHBoxLayout;

    QToolButton *statusIcon = new QToolButton;
    QToolButton *accountIcon = new QToolButton;

    QIcon *qi=new QIcon(":resources/icon/online");
    QIcon *qi2=new QIcon(":resources/icon/mail");
    if(account.getStatus() == Status::online)
        qi=new QIcon(":resources/icon/online");
    else if(account.getStatus() == Status::offline)
        qi=new QIcon(":resources/icon/offline");
    else if(account.getStatus() == Status::none)
        qi=new QIcon(":resources/icon/none");
    accountIcon->setIcon(*qi2);
    accountIcon->setStyleSheet(style.c_str());
    statusIcon->setStyleSheet(style.c_str());


    QToolButton *accountTitle = new QToolButton;
    QToolButton *unredCount = new QToolButton;
    accountTitle->setText(account.getTitle().c_str());
    unredCount->setText(to_string(account.getUnread()).c_str());
    accountTitle->setStyleSheet(style.c_str());
    unredCount->setStyleSheet(style.c_str());
    statusIcon->setIcon(*qi);
    llayout->addWidget(accountIcon);
    llayout->addWidget(accountTitle);
    rlayout->addWidget(unredCount);
    rlayout->addWidget(statusIcon);
    llayout->setAlignment(Qt::AlignLeft);
    rlayout->setAlignment(Qt::AlignRight);
    qlayout->addLayout(llayout);
    qlayout->addLayout(rlayout);
    QWidget* widget = new QWidget;
    widget->setLayout(qlayout);
    QWidgetAction *qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}

QWidgetAction *Hub::createeTitle(string title) {
    QHBoxLayout *slayout = new QHBoxLayout;
    QToolButton *subject = new QToolButton;
    subject->setText(title.c_str());
    subject->setStyleSheet(style.c_str());
    slayout->setAlignment(Qt::AlignLeft);
    slayout->addWidget(subject);
    QAction *qact=new QAction;
    QWidget* sWidget = new QWidget;
    sWidget->setLayout(slayout);
    QWidgetAction *sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(sWidget);
    return sWidgetAction;
}
