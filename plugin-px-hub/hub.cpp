#include <QtWidgets/QPushButton>
#include "hub.h"

hub::hub(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo),
    mHidden(false)
{
    realign();
    
    mButton.setStyleSheet(QString::fromStdString(("QToolButton::menu-indicator { image: none; }")));
    mButton.setMenu(mainMenu);
    mButton.setPopupMode(QToolButton::InstantPopup);
    mButton.setAutoRaise(true);
    mButton.setIcon(QIcon::fromTheme(QString::fromStdString("px-hub")));

    EventSubscriber * eventSubscriber = new EventSubscriber("hub");
    connect(eventSubscriber,SIGNAL(hubEvents(EventObject *)),this,SLOT(hubEventsHandler(EventObject *)));
    eventSubscriber->run();
    connect(mainMenu,SIGNAL(triggered(QAction*)),this,SLOT(mainMenuTriggeredHandler(QAction*)));
    connect(&refreshTimer,SIGNAL(timeout()),this,SLOT(refresh()));
    connect(mainMenu,&QMenu::aboutToShow,[&](){
        refreshTimer.start(60000);
    });
    connect(mainMenu,&QMenu::aboutToHide,[&](){
        refreshTimer.stop();  
    });
}

void hub::mainMenuTriggeredHandler(QAction* action){
   //auto accountAction = (AccountItem*)action;
    auto accountAction = qobject_cast<AccountItem *>(action);
    if(accountAction != NULL){
        std::string command = "px-hub-gui:account="+accountAction->getAccount().getTitle();
        QDesktopServices::openUrl(QUrl(QString::fromStdString(command)));
    }
    
}

void hub::realign()
{
    mButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
#include <QLabel>
void hub::refresh() {
    bool menuIsVisible = mainMenu->isVisible();    
    //sleep(1);
    // get accounts
    RPCHubClient rpcHubClient;
    vector<AccountObject> accounts= rpcHubClient.getAccountList();
    // get messages
    vector<MessageObject> messageList = rpcHubClient.getMessageList(MAX_MESSAGE_COUNT);
    auto hubItemList = new HubItem(messageList,this);
    //get calendar detail
    auto calendarList = getCalendar();      
    auto calcWidget = new CalendarBoxWidgetAction(calendarList,this);
    //mainMenu->hide();
    auto acts = mainMenu->actions();
    for(auto obj: acts)
        obj->deleteLater();
    mainMenu->clear();
    mainMenu->setFixedWidth(MAIN_MENU_SIZE_W);
    mainMenu->addAction(createTitle(tr("YOUR ACCOUNTS"), QString::fromStdString("")));
    if(accounts.size() != 0){
        for(auto &s : accounts){
            auto accountItem = new AccountItem(s);
            mainMenu->addAction(accountItem);
            mainMenu->addSeparator();
        }
    }else{
        mainMenu->addAction(createAccountButton());
    }    
    
    auto seperator1 = new LineSeperator();
    auto sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(seperator1);
    mainMenu->addAction(sWidgetAction);
    
    mainMenu->addAction(calcWidget);
    mainMenu->addSeparator();
    mainMenu->addAction(createTitle(tr("PANTHERX HUB"), QStringLiteral("px-updates")));
    
    mainMenu->addAction(hubItemList);

    mainMenu->addSeparator();
    if(menuIsVisible) 
        mButton.showMenu();
}

QWidgetAction *hub::createTitle(QString title, QString icon) {
    auto subject = new QPushButton();;
    auto titleFont = subject->font();
    titleFont.setPointSize(HUB_TITLE_FONT_SIZE);

    subject->setText(title);
    subject->setFont(titleFont);
    subject->setStyleSheet(QString::fromStdString("QPushButton { border:0px;Text-align:left;}"));
    subject->setObjectName(QString::fromStdString("PxHubItem"));
    subject->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);    
    connect(subject,SIGNAL(released()),this,SLOT(pantherButtonHandler()));

    auto slayout = new QHBoxLayout;
    slayout->setAlignment(Qt::AlignLeft);
    slayout->addWidget(subject);
    slayout->setObjectName(QString::fromStdString("PxHubItem"));
    auto rlayout = new QHBoxLayout;

    if(!icon.isEmpty()){
        qPushButton = new QPushButton();      
        qPushButton->setIcon(QIcon::fromTheme(icon));
        qPushButton->setIconSize(QSize(UPDATE_ICON_SIZE,UPDATE_ICON_SIZE));
        qPushButton->setStyleSheet(QString::fromStdString("QPushButton {border:0px;}"));
        qPushButton->setObjectName(QString::fromStdString("PxHubItem"));
        qPushButton->setFixedSize(UPDATE_ICON_SIZE+10,UPDATE_ICON_SIZE+10);
        connect(qPushButton,SIGNAL(released()),this,SLOT(updateButtonHandler()));
        rlayout->addWidget(qPushButton);
        rlayout->setAlignment(Qt::AlignRight); 
    }
    auto mainLayout = new QHBoxLayout;
    mainLayout->addLayout(slayout);
    mainLayout->addLayout(rlayout);

    auto sWidget = new QWidget;
    //sWidget->setObjectName(QString::fromStdString("PxHubItem"));
    sWidget->setLayout(mainLayout);
    sWidget->setContentsMargins(0,0,0,5);
    sWidget->setAttribute(Qt::WA_Hover);
    auto sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(sWidget);

    return sWidgetAction;
}

void hub::updateButtonHandler() {
    qPushButton->setIcon(QIcon());
    qPushButton->setText(QString::fromStdString("..."));

    refresh();
}

void hub::pantherButtonHandler() {
    QDesktopServices::openUrl(QUrl(QString::fromStdString("px-hub-gui:")));
}

void hub::accountButtonHandler(){
   QDesktopServices::openUrl(QUrl(QString::fromStdString("px-settings-online-accounts:")));   
}

QWidgetAction *hub::createAccountButton(){
    auto accountTitle = new QPushButton();
    auto accountTitleFont = accountTitle->font();
    accountTitleFont.setPointSize(ACCOUNT_TITLE_BUTTON_FONT);

    accountTitle->setText(QString::fromStdString("You have not added any supported online accounts yet.\n Add an account now"));
    accountTitle->setFont(accountTitleFont);
    accountTitle->setStyleSheet(QString::fromStdString("QPushButton {background-color: transparent; border:0px;}"));
    connect(accountTitle,SIGNAL(released()),this,SLOT(accountButtonHandler()));

    auto slayout = new QHBoxLayout;
    slayout->setAlignment(Qt::AlignLeft);
    slayout->addWidget(accountTitle);

    auto sWidget = new QWidget;
    sWidget->setObjectName(QString::fromStdString("accountButtonTitle"));
    sWidget->setLayout(slayout);
    sWidget->setContentsMargins(0,0,0,5);
    auto sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(sWidget);

    return sWidgetAction;
}


void hub::hubEventsHandler(EventObject *eventObject){
    QString popup;
    auto params = eventObject->getParams();
    if(eventObject->getEvent() == QString::fromStdString("status_change")){
        popup = params[QString::fromStdString("account_title")] + QString::fromStdString(" is ") + params[QString::fromStdString("new-status")];
        LXQt::Notification::notify(popup);
    }else if(eventObject->getEvent() == QString::fromStdString("service_refresh")){
        auto newmsg = eventObject->getParam(QString::fromStdString("new_message"));
        if(!newmsg.isEmpty()){
            popup = params[QString::fromStdString("account_title")] + 
                    QString::fromStdString(" has ") + 
                    newmsg + 
                    QString::fromStdString(" new message");
            LXQt::Notification::notify(popup);
        }
    }
    refresh();
}

QVector<TimeBoxObject> hub::getCalendar(){

    QVector<TimeBoxObject> timeBoxObjectList;
    //TODO FILL vector

    return timeBoxObjectList;
}

