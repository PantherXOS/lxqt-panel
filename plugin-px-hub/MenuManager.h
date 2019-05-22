//
// Created by Fakhri on 5/21/19.
//

#ifndef LXQT_PANEL_MENUMANAGER_H
#define LXQT_PANEL_MENUMANAGER_H

#include <vector>
#include "rpc/EventHandler.h"
#include <thread>
#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QFontMetrics>
#include <QTextEdit>
#include <QToolButton>
#include <QList>
#include <XdgIcon>
#include <string>
#include <capnp/ez-rpc.h>
#include <rpc/AccountParser.h>
#include <interfaces/hub.capnp.h>
#include <rpc/ServiceEventHandler.h>
#include <rpc/EventSubscriber.h>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QHBoxLayout>
#include "Account.h"

class MenuManager: public QObject {
public:
    MenuManager(QToolButton *mButton, QMenu *mainMenu);
    void run();
    void stop();
    void refresh();
    vector<EventHandler::EventObject> events;

private:
    QMenu *mainMenu;
    string style = "background: transparent; border: none;";
    std::thread statThread;
    bool isRun;
    QToolButton *mButton;
    QWidgetAction* buildAccountItem(Account account);
    QWidgetAction* createeTitle(string title);

    vector<Account> getAccount();
};


#endif //LXQT_PANEL_MENUMANAGER_H
