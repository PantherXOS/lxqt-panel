//
// Created by root on 5/21/19.
//


#include "MenuManager.h"

void MenuManager::run() {
    if(!isRun) {
        isRun=true;
        statThread = std::thread([&]() {
            while (1) {
                if(events.size()!=0){
                    for(EventHandler::EventObject e : events){
                        if(e.event == "hub" && e.topic == "status update"){

                        }
                    }
                }

            }
        });
    }
}

QWidgetAction *MenuManager::buildAccountItem(Account account) {
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

QWidgetAction *MenuManager::createeTitle(string title) {

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

vector<Account> MenuManager::getAccount() {

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

MenuManager::MenuManager(QToolButton *mButton, QMenu *mainMenu) {
    this->mButton = mButton;
    this->mainMenu = mainMenu;
}

void MenuManager::refresh() {
    vector <QAction *> qactVector;
    mainMenu->setFixedWidth(300);
    vector<Account> accounts=getAccount();
    mainMenu->addAction(createeTitle("Your Account"));
    for(Account s : accounts){
        mainMenu->addAction(buildAccountItem(s));
    }
    mainMenu->addSeparator();


    mButton->setMenu(mainMenu);
    mButton->setPopupMode(QToolButton::InstantPopup);
    mButton->setAutoRaise(true);
    mButton->setIcon(QIcon(":resources/icon/user"));
}
