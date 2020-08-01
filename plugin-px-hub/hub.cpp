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
#if 0  //Test
    vector<AccountObject> accounts;
    AccountObject accountObject;
    accountObject.setTitle("Test");
    accountObject.setStatus(Status::online);
    accountObject.setUnread(11);
    accountObject.setIcon("px-user");
    for (int i=0;i<6;i++)
        accounts.push_back(accountObject);
#endif//Finish test

    mainMenu->setFixedWidth(MAIN_MENU_SIZE_W);
    mainMenu->addAction(createTitle(tr("YOUR ACCOUNTS"), QString::fromStdString("")));
    for(const auto &s : accounts){
        mainMenu->addAction(buildAccountItem(s));
        mainMenu->addSeparator();
    }
    auto seperator1 = new LineSeperator();
    auto sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(seperator1);
    mainMenu->addAction(sWidgetAction);

    mainMenu->addAction(createTitle(tr("PANTHER HUB"), QStringLiteral("px-updates")));
    // get messages

    vector<MessageObject> messageList = rpcHubClient.getMessageList(6);
#if 0 //Test
    vector<MessageObject> messageList;
    MessageObject messageObject;
    messageObject.setSender("Fakhri");
    messageObject.setMessage("This is test");
    messageObject.setTime("1577891410");
    messageObject.setIcon("px-user");
    for(int i=0; i<5;i++)
       messageList.push_back(messageObject);
#endif
    for(auto &m : messageList){
        mainMenu->addAction(buildMessageItem(m));
        mainMenu->addSeparator();
    }
    mainMenu->addSeparator();
    mButton.setStyleSheet(QString::fromStdString(("QToolButton::menu-indicator { image: none; }")));
    mButton.setMenu(mainMenu);
    mButton.setPopupMode(QToolButton::InstantPopup);
    mButton.setAutoRaise(true);
    mButton.setIcon(QIcon::fromTheme(QString::fromStdString("panther")));
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

    auto  widget = new QWidget;
    widget->setObjectName(QString::fromStdString("PxHubItem"));
    widget->setLayout(qlayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}

QWidgetAction *hub::createTitle(QString title, QString icon) {
    auto subject = new QLabel;
    auto titleFont = subject->font();
    titleFont.setPointSize(HUB_TITLE_FONT_SIZE);

    subject->setText(title);
    subject->setFont(titleFont);

    auto slayout = new QHBoxLayout;
    slayout->setAlignment(Qt::AlignLeft);
    slayout->addWidget(subject);
    auto rlayout = new QHBoxLayout;

    if(!icon.isEmpty()){
        auto qPushButton = new QPushButton();
        qPushButton->setIcon(QIcon::fromTheme(icon));
        qPushButton->setIconSize(QSize(UPDATE_ICON_SIZE,UPDATE_ICON_SIZE));
        qPushButton->setStyleSheet(QString::fromStdString("QPushButton {background-color: transparent; border:0px;}"));
        connect(qPushButton,SIGNAL(released()),this,SLOT(updateButtonHandler()));
        rlayout->addWidget(qPushButton);
        rlayout->setAlignment(Qt::AlignRight);
    }
    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(slayout);
    mainLayout->addLayout(rlayout);

    auto sWidget = new QWidget;
    sWidget->setObjectName(QString::fromStdString("PxHubTitle"));
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
    QString popup;
    auto params = eventObject->getParams();
    if(eventObject->getEvent() == QString::fromStdString("Status_Change")){
       popup = params[QString::fromStdString("account")] + QString::fromStdString(" is ") + params[QString::fromStdString("new-status")];
       LXQt::Notification::notify(popup);
    }else if(eventObject->getEvent() == QString::fromStdString("Service_refresh")){
        popup = params[QString::fromStdString("account")] + QString::fromStdString(" has 1 new message");
        LXQt::Notification::notify(popup);
    }
    refresh();
}

QWidgetAction *hub::buildMessageItem(MessageObject message) {
    auto messageSender = new QLabel;
    QFont messageSenderFont = messageSender->font();
    messageSenderFont.setPointSize(MSG_SENDER_FONT_SIZE);
    
    QFont messageTimeFont = messageSender->font();
    messageTimeFont.setPointSize(MSG_TIME_FONT_SIZE);

    QFont messagePreviewFont = messageSender->font();
    messagePreviewFont.setPointSize(MSG_PREVIEW_FONT_SIZE);
    if(message.isUnread()){
        messageSenderFont.setItalic(true);
        messageTimeFont.setItalic(true);
        messagePreviewFont.setPointSize(MSG_UNREAD_PREVIEW_FONT_SIZE);
        messagePreviewFont.setItalic(true);
    }
    string acc = message.getSender();
    if(acc.size()>MAX_ACCOUNT_SIZE)
        acc = acc.substr(0,MAX_ACCOUNT_SIZE)+"...";
    messageSender->setText(QString::fromStdString(acc));
    messageSender->setFont(messageSenderFont);

    auto llayout = new QHBoxLayout;
    llayout->addWidget(messageSender);
    llayout->setAlignment(Qt::AlignLeft);
    llayout->setMargin(0);
    llayout->setSpacing(0);
    llayout->setContentsMargins(0,0,0,0);

    auto messageTime = new QLabel;
    QDateTime dt = QDateTime::fromString(QString::fromStdString(message.getTime()), 
                                         QString::fromStdString("yyyy-MM-ddThh:mm:ss.zzzZ"));
    messageTime->setText(dt.toString(QString::fromStdString("yyyy-mm-dd hh:mm")));
    messageTime->setFont(messageTimeFont);

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


    QFontMetrics fm(messagePreviewFont);
    int textWidth =fm.width(QString::fromStdString(message.getMessage()));
    auto _msg = message.getMessage();
    while(textWidth > MAX_MESSAGE_SIZE_WIDTH){
        _msg = _msg.substr(0, _msg.size() - 6);
        _msg += "...";
        textWidth =fm.width(QString::fromStdString(_msg));
    }
    
    auto messagePreview = new QLabel;
    messagePreview->setText(QString::fromStdString(_msg));
    messagePreview->setFont(messagePreviewFont);
    messagePreview->setContentsMargins(0,2,0,0);

    auto Tlayout = new QVBoxLayout;
    Tlayout->addLayout(qlayout);
    Tlayout->addWidget(messagePreview);
    Tlayout->setAlignment(Qt::AlignTop);
    Tlayout->setMargin(0);
    Tlayout->setSpacing(0);
    Tlayout->setContentsMargins(7,0,0,0);
    auto messageIcon = buildIconFromTheme(QString::fromStdString(message.getIcon()), QSize(MESSAGE_ICON_SIZE,MESSAGE_ICON_SIZE));
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
    resultWidget->setObjectName(QString::fromStdString("PxHubItem"));
    resultWidget->setLayout(glayout);
    resultWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);

    auto gWidgetAction = new QWidgetAction(this);
    gWidgetAction->setDefaultWidget(resultWidget);
    return gWidgetAction;
}
