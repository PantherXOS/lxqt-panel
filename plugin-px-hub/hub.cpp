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
        cout << m.toString() << endl;
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
    iconLabel->setPixmap(pixmap);
    iconLabel->setFixedSize(size);
}

QWidgetAction* hub::buildAccountItem(AccountObject account) {
    auto statusIcon=new QIcon;
    if(account.getStatus() == Status::online)
        statusIcon=new QIcon(":resources/icon/online");
    else if(account.getStatus() == Status::offline)
        statusIcon=new QIcon(":resources/icon/offline");
    else if(account.getStatus() == Status::none)
        statusIcon=new QIcon(":resources/icon/none");
    auto statusButton = new QToolButton;
    statusButton->setIcon(*statusIcon);

    // auto accountIcon = buildIconFromFile(account.getIcon,QSize(ACCOUNT_ICON_SIZE,ACCOUNT_ICON_SIZE));
    auto accountIcon = new QToolButton;
    accountIcon->setIcon(XdgIcon::fromTheme("email", "email"));

    auto accountTitle = new QLabel;
    accountTitle->setText(account.getTitle().c_str());
    accountTitle->setObjectName("NoDiskLabel");

    auto unreadCount = new QLabel;
    unreadCount->setText(to_string(account.getUnread()).c_str());
    unreadCount->setObjectName("NoDiskLabel");

    auto llayout = new QHBoxLayout;
    llayout->addWidget(accountIcon);
    llayout->addWidget(accountTitle);
    llayout->setAlignment(Qt::AlignLeft);

    auto rlayout = new QHBoxLayout;
    rlayout->addWidget(unreadCount);
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
    subject->setObjectName("NoDiskLabel");

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
    messageSender->setObjectName("NoDiskLabel");

    auto llayout = new QHBoxLayout;
    llayout->addWidget(messageSender);
    llayout->setAlignment(Qt::AlignLeft);

    auto messageTime = new QLabel;
    messageTime->setText(message.getTime().c_str());
    messageTime->setObjectName("NoDiskLabel");

    auto rlayout = new QHBoxLayout;
    rlayout->addWidget(messageTime);
    rlayout->setAlignment(Qt::AlignRight);

    auto qlayout = new QHBoxLayout;
    qlayout->addLayout(llayout);
    qlayout->addLayout(rlayout);

    auto messagePreview = new QLabel;
    messagePreview->setText(message.getMessage().c_str());
    messagePreview->setObjectName("NoDiskLabel");

    auto Tlayout = new QVBoxLayout;
    Tlayout->addLayout(qlayout);
    Tlayout->addWidget(messagePreview);
    Tlayout->setAlignment(Qt::AlignTop);

    // auto messageIcon = buildIconFromFile(message.getIcon, QSize(MESSAGE_ICON_SIZE,MESSAGE_ICON_SIZE));
    auto messageIcon = new QToolButton;
    messageIcon->setIcon(XdgIcon::fromTheme("email", "email"));

    auto glayout = new QHBoxLayout;
    glayout->addWidget(messageIcon);
    glayout->addLayout(Tlayout);

    auto  resultWidget = new QWidget;
    resultWidget->setLayout(glayout);
    auto gWidgetAction = new QWidgetAction(this);
    gWidgetAction->setDefaultWidget(resultWidget);
    return gWidgetAction;
}
