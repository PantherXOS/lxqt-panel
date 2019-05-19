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

#include "notes.h"

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
    QMenu *menu = new QMenu;
    for(string s : getAccount()){
        QAction * qact= new QAction(tr(s.c_str()));
        qactVector.push_back(qact);
        menu->addAction(qact);
    }
    ServiceEventHandler hubEventHandler;
    EventSubscriber * eventSubscriber = new EventSubscriber("hub", &hubEventHandler, &mButton);
    eventSubscriber->run();

    mButton.setMenu(menu);
    mButton.setPopupMode(QToolButton::InstantPopup);
    mButton.setAutoRaise(true);
    mButton.setIcon(XdgIcon::fromTheme("date", "date"));
    
    
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

vector<string> Notes::getAccount() {
    vector<string> rsult;
    string addr = string("unix:") + HUB_SERVER_ADDRESS;
    capnp::EzRpcClient rpcClient(addr);
    auto &waitScope = rpcClient.getWaitScope();
    HubReader::Client client = rpcClient.getMain<HubReader>();
    try {
        auto actReq = client.getAccountsStatusRequest();
        auto actRes = actReq.send().wait(waitScope);
        for(auto act : actRes.getAccountsStatus()) {
            string status;
            switch (act.getStatus()){
                case AccountStatus::Status::NONE:
                    status = " - NONE";
                    break;
                case AccountStatus::Status::ERROR:
                    status = " - ERROR";
                    break;
                case AccountStatus::Status::ONLINE:
                    status = " - ONLINE";
                    break;
                case AccountStatus::Status::OFFLINE:
                    status = " - OFFLINE";
                    break;
            }
            rsult.push_back(act.getTitle().cStr() + status);
        }
    } catch (kj::Exception){

    }
    if (rsult.size()==0) rsult.push_back("Empty");
    return rsult;
}
