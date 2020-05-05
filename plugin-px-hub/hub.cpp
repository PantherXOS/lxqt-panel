#include <QtWidgets/QPushButton>
#include "hub.h"

hub::hub(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo),
    mHidden(false)
{
    realign();
    refresh();
    EventSubscriber * eventSubscriber = new EventSubscriber("hub");
    connect(eventSubscriber,SIGNAL(hubEvents(EventObject *)),this,SLOT(hubEventsHandler(EventObject *)));
    eventSubscriber->run();
}

void hub::realign()
{
  mButton.setFixedHeight(panel()->iconSize());
  mButton.setFixedWidth(panel()->iconSize());
}

void hub::refresh() {
    mainMenu->clear();
    //sleep(1);
    // get accounts
    RPCHubClient rpcHubClient;
    vector<AccountObject> accounts= rpcHubClient.getAccountList();
    //Test
//    vector<AccountObject> accounts;
//    AccountObject accountObject;
//    accountObject.setTitle("Test");
//    accountObject.setStatus(Status::online);
//    accountObject.setUnread(11);
//    accountObject.setIcon("px-user");
//    for (int i=0;i<6;i++)
//        accounts.push_back(accountObject);
    //Finish test

    mainMenu->setFixedWidth(MAIN_MENU_SIZE_W);
    mainMenu->addAction(createTitle(tr("YOUR ACCOUNTS"), ""));
    for(const auto &s : accounts){
        mainMenu->addAction(buildAccountItem(s));
        mainMenu->addSeparator();
    }
  //  mainMenu->addSeparator();
//    QAction* separator1 = mainMenu->addSeparator();
//    separator1->setObjectName("separator1");
//    mainMenu->setStyleSheet("QToolBar::separator#separator1 {color: white; background-color: white; width: 30; height: 30px; }");
    auto seperator1 = new LineSeperator();
    auto sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(seperator1);
    mainMenu->addAction(sWidgetAction);

    mainMenu->addAction(createTitle(tr("PANTHER HUB"), "px-updates"));
    // get messages

    vector<MessageObject> messageList = rpcHubClient.getMessageList(6);
    //Test
//    vector<MessageObject> messageList;
//    MessageObject messageObject;
//    messageObject.setSender("Fakhri");
//    messageObject.setMessage("I hate U");
//    messageObject.setTime("1577891410");
//    messageObject.setIcon("px-user");
//    for(int i=0; i<5;i++)
//       messageList.push_back(messageObject);
    // End Test

    for(auto &m : messageList){
        mainMenu->addAction(buildMessageItem(m));
        mainMenu->addSeparator();
    }
    //mainMenu->setObjectName("LXQtMountPopup");
    mainMenu->addSeparator();
    mButton.setStyleSheet("QToolButton::menu-indicator { image: none; }");
    mButton.setMenu(mainMenu);
    mButton.setPopupMode(QToolButton::InstantPopup);
    mButton.setAutoRaise(true);
    mButton.setIcon(QIcon::fromTheme("panther"));
}

QLabel *hub::buildIconFromTheme(QString icon, QSize size){
    QIcon qicon = QIcon::fromTheme(icon);
    QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
    auto iconLabel = new QLabel;
    iconLabel->setAttribute(Qt::WA_TranslucentBackground);
    iconLabel->setPixmap(pixmap);
    iconLabel->setFixedSize(size);
    return iconLabel;
}

QLabel *hub::buildIconFromFile(QString file, QSize size){
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

QWidgetAction* hub::buildAccountItem(AccountObject account) {
    QString statusIconFile;
    if(account.getStatus() == Status::online)
        statusIconFile=":resources/icon/status_0_green_o";
    else if(account.getStatus() == Status::offline)
        statusIconFile=":resources/icon/status_0_red";
    else if(account.getStatus() == Status::none)
        statusIconFile=":resources/icon/status_0_grey";
    auto statusLabel = buildIconFromFile(statusIconFile,QSize(ACCOUNT_STATUS_ICON_SIZE,ACCOUNT_STATUS_ICON_SIZE));
    auto accountIcon = buildIconFromTheme(account.getIcon().c_str(),QSize(ACCOUNT_ICON_SIZE,ACCOUNT_ICON_SIZE));
    accountIcon->setContentsMargins(3,0,0,0);
    auto accountTitle = new QLabel;
    string acc = account.getTitle();
    if(acc.size()>MAX_ACCOUNT_SIZE)
        acc = acc.substr(0,MAX_ACCOUNT_SIZE)+"...";
    accountTitle->setText(acc.c_str());
    accountTitle->setMargin(0);
    accountTitle->setContentsMargins(5,0,0,0);

    auto unreadCount = new QLabel;
    unreadCount->setText(to_string(account.getUnread()).c_str());
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

    auto  widget = new QWidget;
    widget->setObjectName("PxHubItem");
    widget->setLayout(qlayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}

QWidgetAction *hub::createTitle(QString title, QString icon) {
    auto subject = new QLabel;
    subject->setText(title);
    subject->setFont(QFont("Helvetica",11,QFont::Bold));

    auto slayout = new QHBoxLayout;
    slayout->setAlignment(Qt::AlignLeft);
    slayout->addWidget(subject);
    auto rlayout = new QHBoxLayout;

    if(!icon.isEmpty()){
        auto qPushButton = new QPushButton();
        qPushButton->setIcon(QIcon::fromTheme(icon));
        qPushButton->setIconSize(QSize(UPDATE_ICON_SIZE,UPDATE_ICON_SIZE));
        qPushButton->setStyleSheet("QPushButton {background-color: transparent; border:0px;}");
        connect(qPushButton,SIGNAL(released()),this,SLOT(updateButtonHandler()));
        rlayout->addWidget(qPushButton);
        rlayout->setAlignment(Qt::AlignRight);
    }
    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(slayout);
    mainLayout->addLayout(rlayout);

    auto sWidget = new QWidget;
    sWidget->setObjectName("PxHubItem");
    sWidget->setLayout(mainLayout);
    sWidget->setContentsMargins(0,0,0,5);
    auto sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(sWidget);

    return sWidgetAction;
}

void hub::updateButtonHandler() {
    refresh();
}

void hub::hubEventsHandler(EventObject *eventObject){
    string popup;
    auto params = eventObject->getParams();
    if(eventObject->getEvent() == "Status_Change"){
       popup = params["account"].toStdString() + " is " + params["new-status"].toStdString();
        LXQt::Notification::notify(popup.c_str());
    }else if(eventObject->getEvent() == "Service_refresh"){
        popup = params["account"].toStdString() + " has 1 new message";
        LXQt::Notification::notify(popup.c_str());
    }
    refresh();
}

QWidgetAction *hub::buildMessageItem(MessageObject message) {
    auto messageSender = new QLabel;
    string acc = message.getSender();
    if(acc.size()>MAX_ACCOUNT_SIZE)
        acc = acc.substr(0,MAX_ACCOUNT_SIZE)+"...";
    messageSender->setText(acc.c_str());
    messageSender->setFont(QFont("Helvetica",9,QFont::Bold));

    auto llayout = new QHBoxLayout;
    llayout->addWidget(messageSender);
    llayout->setAlignment(Qt::AlignLeft);
    llayout->setMargin(0);
    llayout->setSpacing(0);
    llayout->setContentsMargins(0,0,0,0);

    auto messageTime = new QLabel;
    messageTime->setText(message.getTime().c_str());
    messageTime->setFont(QFont("Helvetica",8));

    auto rlayout = new QHBoxLayout;
    rlayout->addWidget(messageTime);
    rlayout->setAlignment(Qt::AlignRight);
    rlayout->setMargin(0);
    rlayout->setSpacing(0);
    rlayout->setContentsMargins(0,0,3,0);

    auto qlayout = new QHBoxLayout;
    qlayout->addLayout(llayout);
    qlayout->addLayout(rlayout);
    qlayout->setMargin(0);
    qlayout->setSpacing(0);
    qlayout->setContentsMargins(0,0,0,0);

    auto messagePreview = new QLabel;
    messagePreview->setText(message.getMessage().c_str());
    messagePreview->setFont(QFont("Helvetica",8));
    messagePreview->setContentsMargins(0,2,0,0);

    auto Tlayout = new QVBoxLayout;
    Tlayout->addLayout(qlayout);
    Tlayout->addWidget(messagePreview);
    Tlayout->setAlignment(Qt::AlignTop);
    Tlayout->setMargin(0);
    Tlayout->setSpacing(0);
    Tlayout->setContentsMargins(7,0,0,0);
    auto messageIcon = buildIconFromTheme(message.getIcon().c_str(), QSize(MESSAGE_ICON_SIZE,MESSAGE_ICON_SIZE));
    auto ilayout = new QHBoxLayout;
    ilayout->addWidget(messageIcon);
    ilayout->setMargin(0);
    ilayout->setSpacing(0);
    ilayout->setContentsMargins(3,0,0,0);

    auto glayout = new QHBoxLayout;
    glayout->addLayout(ilayout);
    glayout->addLayout(Tlayout);
//    glayout->setMargin(0);
    glayout->setSpacing(0);
//    glayout->setContentsMargins(0,0,0,0);

    auto  resultWidget = new QWidget;
    resultWidget->setObjectName("PxHubItem");
    resultWidget->setLayout(glayout);
    resultWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);

    auto gWidgetAction = new QWidgetAction(this);
    gWidgetAction->setDefaultWidget(resultWidget);
    return gWidgetAction;
}
