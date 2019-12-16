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
#include <QtGui/QPainter>
#include <QtWidgets/QScrollArea>
#include <rpc/RPCHubClient.h>

hub::hub(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo),
    mHidden(false)
{
    realign();
    refresh();
    ServiceEventHandler hubEventHandler;
    EventSubscriber * eventSubscriber = new EventSubscriber("hub", &hubEventHandler, this);
    eventSubscriber->run();
    run();
}

void hub::realign()
{
  mButton.setFixedHeight(panel()->iconSize());
  mButton.setFixedWidth(panel()->iconSize());
}

void hub::settingsChanged()
{}

QDialog* hub::configureDialog()
{}

void hub::refresh() {
    // get accounts
    RPCHubClient rpcHubClient;
    vector<AccountObject> accounts= rpcHubClient.getAccountList();
    mainMenu->setFixedWidth(MAIN_MENU_SIZE_W);
    mainMenu->addAction(createTitle(tr("YOUR ACCOUNTS")));
    for(const auto &s : accounts){
        mainMenu->addAction(buildAccountItem(s));
    }
    mainMenu->addSeparator();
    mainMenu->addAction(createTitle(tr("PANTHER HUB")));
    // get messages
    vector<MessageObject> messageList = rpcHubClient.getMessageList();
    for(auto &m : messageList){
        cout << m.toString() << endl;
        mainMenu->addAction(buildMessageItem(m));
    }
    //
    mainMenu->setObjectName("LXQtMountPopup");
    mainMenu->addSeparator();
    mButton.setMenu(mainMenu);
    mButton.setPopupMode(QToolButton::InstantPopup);
    mButton.setAutoRaise(true);
    mButton.setIcon(QIcon(":resources/icon/user"));
}

QWidgetAction* hub::buildAccountItem(AccountObject account) {
    auto statusIcon=new QIcon;
    if(account.getStatus() == Status::online)
        statusIcon=new QIcon(":resources/icon/online");
    else if(account.getStatus() == Status::offline)
        statusIcon=new QIcon(":resources/icon/offline");
    else if(account.getStatus() == Status::none)
        statusIcon=new QIcon(":resources/icon/none");

    QIcon accountIcon = XdgIcon::fromTheme("email", "email"); // TODO retrieve from hub
    auto statusButton = new QToolButton;
    statusButton->setStyleSheet(style.c_str());
    statusButton->setIcon(*statusIcon);

    auto accountButton = new QToolButton;
    accountButton->setIcon(accountIcon);
    accountButton->setStyleSheet(style.c_str());

    auto accountTitle = new QLabel;
    accountTitle->setText(account.getTitle().c_str());

    auto unredCount = new QLabel;
    unredCount->setText(to_string(account.getUnread()).c_str());

    auto llayout = new QHBoxLayout;
    llayout->addWidget(accountButton);
    llayout->addWidget(accountTitle);
    llayout->setAlignment(Qt::AlignLeft);

    auto rlayout = new QHBoxLayout;
    rlayout->addWidget(unredCount);
    rlayout->addWidget(statusButton);
    rlayout->setAlignment(Qt::AlignRight);

    auto qlayout = new QHBoxLayout;
    qlayout->addLayout(llayout);
    qlayout->addLayout(rlayout);

    auto  widget = new QWidget;
    widget->setLayout(qlayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}

QWidgetAction *hub::createTitle(QString title) {
    auto subject = new QLabel;
    subject->setText(title);
    subject->setFont(QFont("default",12,QFont::Bold));

    auto slayout = new QHBoxLayout;
    slayout->setAlignment(Qt::AlignLeft);
    slayout->addWidget(subject);
    auto sWidget = new QWidget;
    sWidget->setLayout(slayout);
    auto sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(sWidget);
    return sWidgetAction;
}

void hub::run() {
    if(!isRun) {
        isRun=true;
        statThread = std::thread([&]() {
            while (1) {
                for (EventHandler::EventObject e : events) {
                    if (e.topic == "Status_Change" && e.event == "hub") {
                        refresh();
                    }
                }
            }
        });
    }
}

void hub::putEvent(EventHandler::EventObject eventObject) {
    events.push_back(eventObject);
}

QWidgetAction *hub::buildMessageItem(MessageObject message) {
    auto messageSender = new QLabel;
    messageSender->setText(message.getSender().c_str());

    auto llayout = new QHBoxLayout;
    llayout->addWidget(messageSender);
    llayout->setAlignment(Qt::AlignLeft);

    auto messageTime = new QLabel;
    messageTime->setText(message.getTime().c_str());

    auto rlayout = new QHBoxLayout;
    llayout->addWidget(messageTime);
    llayout->setAlignment(Qt::AlignRight);

    auto qlayout = new QHBoxLayout;
    qlayout->addLayout(llayout);
    qlayout->addLayout(rlayout);

    auto messagePreview = new QLabel;
    messagePreview->setText(message.getMessage().c_str());

    auto Tlayout = new QVBoxLayout;
    Tlayout->addLayout(qlayout);
    llayout->addWidget(messagePreview);
    //llayout->setAlignment(Qt::AlignTop);

    QIcon messageIcon = XdgIcon::fromTheme("email", "email"); // TODO replace suitable icon
    auto messageButton = new QToolButton;
    messageButton->setIcon(messageIcon);
    messageButton->setStyleSheet(style.c_str());

    auto glayout = new QHBoxLayout;
    glayout->addWidget(messageButton);
    glayout->addLayout(Tlayout);

    auto  resultWidget = new QWidget;
    resultWidget->setLayout(glayout);
    auto gWidgetAction = new QWidgetAction(this);
    gWidgetAction->setDefaultWidget(resultWidget);
    return gWidgetAction;
}