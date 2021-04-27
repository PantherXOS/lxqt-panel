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
}

void hub::realign()
{
    mButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
#include <QLabel>
void hub::refresh() {
    bool menuIsVisible = mainMenu->isVisible();
    mainMenu->hide();
    mainMenu->clear();
    //sleep(1);
    // get accounts
    RPCHubClient rpcHubClient;
    vector<AccountObject> accounts= rpcHubClient.getAccountList();

    mainMenu->setFixedWidth(MAIN_MENU_SIZE_W);
    mainMenu->addAction(createTitle(tr("YOUR ACCOUNTS"), QString::fromStdString("")));
    for(auto &s : accounts){
        auto accountItem = new AccountItem(s);
        mainMenu->addAction(accountItem);
        mainMenu->addSeparator();
    }
    auto seperator1 = new LineSeperator();
    auto sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(seperator1);
    mainMenu->addAction(sWidgetAction);

    mainMenu->addAction(createTitle(tr("PANTHERX HUB"), QStringLiteral("px-updates")));
    // get messages
    vector<MessageObject> messageList = rpcHubClient.getMessageList(MAX_MESSAGE_COUNT);
    auto hubItemList = new HubItem(messageList);
    mainMenu->addAction(hubItemList);

    mainMenu->addSeparator();
    if(menuIsVisible) 
        mButton.showMenu();
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

