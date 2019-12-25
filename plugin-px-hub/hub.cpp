#include "hub.h"

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
    vector<MessageObject> messageList = rpcHubClient.getMessageList(6);
    for(auto &m : messageList){
        mainMenu->addAction(buildMessageItem(m));
    }
    //
    mainMenu->setObjectName("LXQtMountPopup");
    mainMenu->addSeparator();
    mButton.setMenu(mainMenu);
    mButton.setPopupMode(QToolButton::InstantPopup);
    mButton.setAutoRaise(true);
    //mButton.setIcon(XdgIcon::fromTheme(QLatin1String("view-refresh")));
}

QLabel *hub::buildIconFromFile(QString file, QSize size){
    QIcon qicon;
    QImage image(file);
    qicon.addPixmap(QPixmap::fromImage(image), QIcon::Normal, QIcon::On);
    QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
    auto iconLabel = new QLabel;
//    iconLable->setStyleSheet("border: 0px;");
    iconLabel->setAttribute(Qt::WA_TranslucentBackground);
    iconLabel->setPixmap(pixmap);
    iconLabel->setFixedSize(size);
    return iconLabel;
}

QWidgetAction* hub::buildAccountItem(AccountObject account) {
    QString statusIconFile;
    if(account.getStatus() == Status::online)
        statusIconFile=":resources/icon/online";
    else if(account.getStatus() == Status::offline)
        statusIconFile=":resources/icon/offline";
    else if(account.getStatus() == Status::none)
        statusIconFile=":resources/icon/none";
    auto statusLabel = buildIconFromFile(statusIconFile,QSize(ACCOUNT_STATUS_ICON_SIZE,ACCOUNT_STATUS_ICON_SIZE));
    auto accountIcon = buildIconFromFile(account.getIcon().c_str(),QSize(ACCOUNT_ICON_SIZE,ACCOUNT_ICON_SIZE));


    auto accountTitle = new QLabel;
    accountTitle->setText(account.getTitle().c_str());
    accountTitle->setStyleSheet("QLabel{ color: #D2D8E0;}");
    //accountTitle->setObjectName("NoDiskLabel");

    auto unreadCount = new QLabel;
    unreadCount->setText(to_string(account.getUnread()).c_str());
    unreadCount->setStyleSheet("QLabel{ color: #D2D8E0;}");
    if(account.getUnread() == 0)
        unreadCount->setVisible(false);
    //unreadCount->setObjectName("NoDiskLabel");

    auto llayout = new QHBoxLayout;
    llayout->addWidget(accountIcon);
    llayout->addWidget(accountTitle);
    llayout->setAlignment(Qt::AlignLeft);

    auto rlayout = new QHBoxLayout;
    rlayout->addWidget(unreadCount);
    rlayout->addWidget(statusLabel);
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
    subject->setStyleSheet("QLabel{color: #D2D8E0;}");
    //subject->setObjectName("NoDiskLabel");

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
    messageSender->setFont(QFont("default",10,QFont::Bold));
   //messageSender->setObjectName("NoDiskLabel");
   messageSender->setStyleSheet("QLabel{color: #D2D8E0;}");

    auto llayout = new QHBoxLayout;
    llayout->addWidget(messageSender);
    llayout->setAlignment(Qt::AlignLeft);
//    llayout->setMargin(0);
//    llayout->setSpacing(0);
//    llayout->setContentsMargins(0,0,0,0);

    auto messageTime = new QLabel;
    messageTime->setText(message.getTime().c_str());
   //messageTime->setObjectName("NoDiskLabel");
    messageTime->setStyleSheet("QLabel{ color: #D2D8E0;}");

    auto rlayout = new QHBoxLayout;
    rlayout->addWidget(messageTime);
    rlayout->setAlignment(Qt::AlignRight);
//    rlayout->setMargin(0);
//    rlayout->setSpacing(0);
//    rlayout->setContentsMargins(0,0,0,0);

    auto qlayout = new QHBoxLayout;
    qlayout->addLayout(llayout);
    qlayout->addLayout(rlayout);
//    qlayout->setMargin(0);
//    qlayout->setSpacing(0);
//    qlayout->setContentsMargins(0,0,0,0);

    auto messagePreview = new QLabel;
    messagePreview->setText(message.getMessage().c_str());
   // messagePreview->setObjectName("NoDiskLabel");
   messagePreview->setStyleSheet("QLabel{ color: #D2D8E0;}");

    auto Tlayout = new QVBoxLayout;
    Tlayout->addLayout(qlayout);
    Tlayout->addWidget(messagePreview);
    Tlayout->setAlignment(Qt::AlignTop);
//    Tlayout->setMargin(0);
//    Tlayout->setSpacing(0);
//    Tlayout->setContentsMargins(0,0,0,0);
    auto messageIcon = buildIconFromFile(message.getIcon().c_str(), QSize(MESSAGE_ICON_SIZE,MESSAGE_ICON_SIZE));
    auto ilayout = new QHBoxLayout;
    ilayout->addWidget(messageIcon);
//    ilayout->setMargin(0);
//    ilayout->setSpacing(0);
//    ilayout->setContentsMargins(0,0,0,0);

    auto glayout = new QHBoxLayout;
    glayout->addLayout(ilayout);
    glayout->addLayout(Tlayout);
//    glayout->setMargin(0);
//    glayout->setSpacing(0);
//    glayout->setContentsMargins(0,0,0,0);

    auto  resultWidget = new QWidget;
    resultWidget->setLayout(glayout);
    resultWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);

    auto gWidgetAction = new QWidgetAction(this);
    gWidgetAction->setDefaultWidget(resultWidget);
    return gWidgetAction;
}
