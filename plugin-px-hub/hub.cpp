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

#include "hub.h"

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

Notes::Notes(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo),
    mHidden(false)
{
    realign();
    vector <QAction *> qactVector;
    QMenu *mainMenu = new QMenu;

    vector<Account> accounts=getAccount();

    for(Account s : accounts){
        QHBoxLayout *qlayout = new QHBoxLayout;
        QToolButton *statusIcon = new QToolButton;
        QLabel *accountTitle = new QLabel;
        QToolButton *accountIcon = new QToolButton;
     // accountIcon->setStyleSheet("QLabel { border: 0px; }");
        QIcon *qi=new QIcon("/home/fakhri/Downloads/online");
        string title = s.getTitle() + " - (" + to_string(s.getUnread()) + ")";
        if(s.getStatus() == Status::online)
            qi=new QIcon("/home/fakhri/Downloads/online");
        else if(s.getStatus() == Status::offline)
            qi=new QIcon("/home/fakhri/Downloads/offline");
        else if(s.getStatus() == Status::none)
            qi=new QIcon("/home/fakhri/Downloads/none");
        accountIcon->setIcon(*qi);
        accountTitle->setText(title.c_str());
        statusIcon->setIcon(*qi);
        qlayout->addWidget(accountIcon);
        qlayout->addWidget(accountTitle);
        qlayout->addWidget(statusIcon);

        mainMenu->setLayout(qlayout);

    }
    ServiceEventHandler hubEventHandler;
    EventSubscriber * eventSubscriber = new EventSubscriber("hub", &hubEventHandler, &mButton);
    eventSubscriber->run();

    mButton.setMenu(mainMenu);
    mButton.setPopupMode(QToolButton::InstantPopup);
    mButton.setAutoRaise(true);
//    mButton.setIcon(XdgIcon::fromTheme("drive-removable-media", "drive-removable-media"));
    
    
    // load all notes
    QDir dir(dataDir());
    QStringList notes = dir.entryList(QDir::Files);
    
    for(int i = 0; i < notes.size(); ++i) {
        qint64 noteId = notes[i].toLong();
        
        if(!noteId)
            continue;

    }
    
    // set colors & fonts 
    settingsChanged();
}


Notes::~Notes()
{
    for(auto it = mNotes.begin(); it != mNotes.end(); ++it)
        delete it.value();
    
    mNotes.clear();
}

void Notes::realign()
{
  mButton.setFixedHeight(panel()->iconSize());
  mButton.setFixedWidth(panel()->iconSize());
}


QString Notes::dataDir()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QDir::separator() + "lxqt-notes";
    return dir;
}

void Notes::addNewNote()
{

}

void Notes::deleteNote(const qint64 &id)
{
    auto it = mNotes.find(id);
    if(it != mNotes.end()) {
        delete it.value();
        mNotes.erase(it);
    }
    
    // delete note file
    QDir dir(dataDir());
    dir.remove(QString::number(id));
}

void Notes::toggleShowHide()
{

}

void Notes::setIconColor(const QString &icon, const QString &color)
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

void Notes::settingsChanged()
{

}

QDialog* Notes::configureDialog()
{

}

vector<Account> Notes::getAccount() {
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
    return rsult;
}
