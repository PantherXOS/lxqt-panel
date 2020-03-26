#include "system.h"
#include "QDebug"
//
System::System(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo),
    mHidden(false)
{
    realign();
//    refresh();
    mainMenu->setObjectName("LXQtMountPopup");
    //mainMenu->addSeparator();
    mButton.setStyleSheet("QToolButton::menu-indicator { image: none; }");
    mButton.setMenu(mainMenu);
    mButton.setPopupMode(QToolButton::DelayedPopup);
//    mButton.setAutoRaise(true);
//    mButton.setCheckable(true);
    mButton.setIcon(QIcon::fromTheme("px-user"));
    mButton.setText("UerTest");
    connect(&mButton, SIGNAL(pressed()),this,SLOT(refresh()));
//    EventSubscriber * eventSubscriber = new EventSubscriber("hub");
//    connect(eventSubscriber,SIGNAL(hubEvents(EventObject *)),this,SLOT(hubEventsHandler(EventObject *)));
//    eventSubscriber->run();
}

void System::realign()
{
  mButton.setFixedHeight(panel()->iconSize());
  mButton.setFixedWidth(panel()->iconSize());
}

void System::refresh() {
    qDebug()<< "Start REFRESH";
    mainMenu->clear();
    mainMenu->setFixedWidth(MAIN_MENU_SIZE_W);
    mainMenu->addAction(getUser());
    mainMenu->addSeparator();
    mainMenu->addAction(getFirewallStatus());
    mainMenu->addSeparator();
    mainMenu->addAction(getInternet());
    mainMenu->addSeparator();
    mainMenu->addAction(getVpnStatus());
    mainMenu->addSeparator();
    mainMenu->addAction(getWifiStatus());
    mainMenu->addSeparator();
    mainMenu->addAction(getBTStatus());
    mainMenu->addSeparator();
    mainMenu->addAction(getUpdateStat());
    mButton.showMenu();
}

QLabel *System::buildIconFromTheme(QString icon, QSize size){
    QIcon qicon = QIcon::fromTheme(icon);
    QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
    auto iconLabel = new QLabel;
    iconLabel->setAttribute(Qt::WA_TranslucentBackground);
    iconLabel->setPixmap(pixmap);
    iconLabel->setFixedSize(size);
    return iconLabel;
}

QLabel *System::buildIconFromFile(QString file, QSize size){
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

QWidgetAction *System::createTitle(QString title, QString icon) {
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
    sWidget->setLayout(mainLayout);
    sWidget->setContentsMargins(0,0,0,5);
    auto sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(sWidget);

    return sWidgetAction;
}

QWidgetAction *System::getUser() {

    auto accountIcon = buildIconFromTheme("px-user",QSize(ACCOUNT_ICON_SIZE,ACCOUNT_ICON_SIZE));
    accountIcon->setContentsMargins(0,0,0,0);
    auto accountTitle = new QLabel;
    string acc = exec("whoami").c_str();
    acc = acc.substr(0, acc.find("\n", 0));
    if(acc.size()>MAX_ACCOUNT_SIZE)
        acc = acc.substr(0,MAX_ACCOUNT_SIZE)+"...";
    accountTitle->setText(acc.c_str());
    accountTitle->setMargin(0);
    accountTitle->setContentsMargins(7,0,0,0);

    auto llayout = new QHBoxLayout;
    llayout->addWidget(accountIcon);
    llayout->addWidget(accountTitle);
    llayout->setAlignment(Qt::AlignLeft);
    llayout->setMargin(0);
    llayout->setSpacing(0);
    llayout->setContentsMargins(5,0,0,0);

    auto  widget = new QWidget;
    widget->setLayout(llayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}

QWidgetAction *System::getFirewallStatus() {
    return generalItems("FIREWALL","",false,"px-firewall");
}

QWidgetAction *System::getInternet() {
    map<string,string> internetData; //TODO should be fill by system
    internetData.insert(pair<string, string>("INTERNET", "192.168.10.10"));
    internetData.insert(pair<string, string>("WIFI", "192.168.0.12"));
    internetData.insert(pair<string, string>("VPN", "123.123.123.123"));
    auto llayout = new QVBoxLayout;
    auto Tlayout = new QVBoxLayout;
    int i=0;
    for(auto m:internetData){
        if(i==0){
            llayout->addLayout(internetLayout(m.first.c_str(), ":resources/icon/status_2_green_d"));
        }else{
            llayout->addLayout(internetLayout(m.first.c_str(), ":resources/icon/status_3_green_ud"));
        }
        i++;
    }
    llayout->addLayout(internetLayout("YOU", ":resources/icon/status_1_green_u"));
    llayout->setAlignment(Qt::AlignTop);
    llayout->setMargin(0);
    llayout->setSpacing(0);
    llayout->setContentsMargins(0,0,0,0);
    for(auto m:internetData){
        auto detail = new QLabel;
        detail->setText(m.second.c_str());
        detail->setMargin(0);
        detail->setContentsMargins(5,0,0,0);
        Tlayout->addWidget(detail);
    }
    Tlayout->setAlignment(Qt::AlignTop);
    Tlayout->setMargin(0);
    Tlayout->setSpacing(0);
    Tlayout->setContentsMargins(15,0,0,0);

    auto rlayout = new QHBoxLayout;
    rlayout->addLayout(llayout);
    rlayout->addLayout(Tlayout);
    rlayout->setAlignment(Qt::AlignLeft);
    rlayout->setMargin(0);
    rlayout->setSpacing(0);
    rlayout->setContentsMargins(3,0,0,0);

    auto  widget = new QWidget;
    widget->setLayout(rlayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}
//
//void hub::updateButtonHandler() {
//    refresh();
//}
//
//void hub::hubEventsHandler(EventObject *eventObject){
//    string popup;
//    auto params = eventObject->getParams();
//    if(eventObject->getEvent() == "Status_Change"){
//       popup = params["account"].toStdString() + " is " + params["new-status"].toStdString();
//        LXQt::Notification::notify(popup.c_str());
//    }else if(eventObject->getEvent() == "Service_refresh"){
//        popup = params["account"].toStdString() + " has 1 new message";
//        LXQt::Notification::notify(popup.c_str());
//    }
//    refresh();
//}
//
//QWidgetAction *hub::buildMessageItem(MessageObject message) {
//    auto messageSender = new QLabel;
//    string acc = message.getSender();
//    if(acc.size()>MAX_ACCOUNT_SIZE)
//        acc = acc.substr(0,MAX_ACCOUNT_SIZE)+"...";
//    messageSender->setText(acc.c_str());
//    messageSender->setFont(QFont("Helvetica",9,QFont::Bold));
//
//    auto llayout = new QHBoxLayout;
//    llayout->addWidget(messageSender);
//    llayout->setAlignment(Qt::AlignLeft);
//    llayout->setMargin(0);
//    llayout->setSpacing(0);
//    llayout->setContentsMargins(0,0,0,0);
//
//    auto messageTime = new QLabel;
//    messageTime->setText(message.getTime().c_str());
//    messageTime->setFont(QFont("Helvetica",8));
//
//    auto rlayout = new QHBoxLayout;
//    rlayout->addWidget(messageTime);
//    rlayout->setAlignment(Qt::AlignRight);
//    rlayout->setMargin(0);
//    rlayout->setSpacing(0);
//    rlayout->setContentsMargins(0,0,3,0);
//
//    auto qlayout = new QHBoxLayout;
//    qlayout->addLayout(llayout);
//    qlayout->addLayout(rlayout);
//    qlayout->setMargin(0);
//    qlayout->setSpacing(0);
//    qlayout->setContentsMargins(0,0,0,0);
//
//    auto messagePreview = new QLabel;
//    messagePreview->setText(message.getMessage().c_str());
//    messagePreview->setFont(QFont("Helvetica",8));
//
//    auto Tlayout = new QVBoxLayout;
//    Tlayout->addLayout(qlayout);
//    Tlayout->addWidget(messagePreview);
//    Tlayout->setAlignment(Qt::AlignTop);
//    Tlayout->setMargin(0);
//    Tlayout->setSpacing(0);
//    Tlayout->setContentsMargins(7,0,0,0);
//    auto messageIcon = buildIconFromTheme(message.getIcon().c_str(), QSize(MESSAGE_ICON_SIZE,MESSAGE_ICON_SIZE));
//    auto ilayout = new QHBoxLayout;
//    ilayout->addWidget(messageIcon);
//    ilayout->setMargin(0);
//    ilayout->setSpacing(0);
//    ilayout->setContentsMargins(3,0,0,0);
//
//    auto glayout = new QHBoxLayout;
//    glayout->addLayout(ilayout);
//    glayout->addLayout(Tlayout);
//    glayout->setMargin(0);
//    glayout->setSpacing(0);
//    glayout->setContentsMargins(0,0,0,0);
//
//    auto  resultWidget = new QWidget;
//    resultWidget->setLayout(glayout);
//    resultWidget->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
//
//    auto gWidgetAction = new QWidgetAction(this);
//    gWidgetAction->setDefaultWidget(resultWidget);
//    return gWidgetAction;
//}

QLayout * System::internetLayout(QString text, QString icon) {
auto statusLabel = buildIconFromFile(icon,QSize(ACCOUNT_STATUS_ICON_SIZE,ACCOUNT_STATUS_ICON_SIZE));
statusLabel->setMargin(0);
statusLabel->setContentsMargins(0,0,0,0);
auto Hlayout = new QHBoxLayout;
auto title = new QLabel;
title->setText(text);
title->setMargin(0);
//title->setFont(QFont("default",11));
title->setContentsMargins(9,0,0,0);
Hlayout->addWidget(statusLabel);
Hlayout->addWidget(title);
Hlayout->setAlignment(Qt::AlignLeft);
Hlayout->setMargin(0);
Hlayout->setSpacing(0);
return Hlayout;
}

QWidgetAction *System::getVpnStatus() {
    return generalItems("VPN","OVPN-UK-1",true,"px-vpn");
}

QWidgetAction *System::getWifiStatus() {
    return generalItems("WIFI","HomeWifi",true,"px-wifi");
}
QWidgetAction *System::getBTStatus() {
    return generalItems("BT","Logitech z533",true,"px-bluetooth");
}

QWidgetAction* System::generalItems(QString name,QString information,bool stat,QString icon){
    QString statusIconFile;
    if(stat){
        statusIconFile=":resources/icon/status_0_green_o";
    }
    else{
        statusIconFile=":resources/icon/status_0_grey";
    }
    auto statusLabel = buildIconFromFile(statusIconFile,QSize(ACCOUNT_STATUS_ICON_SIZE,ACCOUNT_STATUS_ICON_SIZE));
    auto firewallIcon = buildIconFromTheme(icon,QSize(ACCOUNT_ICON_SIZE,ACCOUNT_ICON_SIZE));
    firewallIcon->setContentsMargins(0,0,0,0);
    auto title = new QLabel;
    title->setText(name);
    title->setMargin(0);
    title->setContentsMargins(8,0,0,0);

    auto detail = new QLabel;
    detail->setText(information); //TODO should be fill by system
    detail->setMargin(0);
    detail->setContentsMargins(8,0,0,0);

    auto llayout = new QHBoxLayout;
    llayout->addWidget(firewallIcon);
    llayout->addWidget(title);
    llayout->addWidget(detail);
    llayout->setAlignment(Qt::AlignLeft);
    llayout->setMargin(0);
    llayout->setSpacing(0);
    llayout->setContentsMargins(0,0,0,0);

    auto rlayout = new QHBoxLayout;
    rlayout->addWidget(statusLabel);
    rlayout->setAlignment(Qt::AlignRight);
    rlayout->setMargin(0);
    rlayout->setSpacing(0);
    rlayout->setContentsMargins(0,0,0,0);

    auto qlayout = new QHBoxLayout;
    qlayout->addLayout(llayout);
    qlayout->addLayout(rlayout);
    qlayout->setMargin(0);
    qlayout->setSpacing(0);
    qlayout->setContentsMargins(3,0,3,0);

    auto  widget = new QWidget;
    widget->setLayout(qlayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}

QWidgetAction *System::getUpdateStat() {
    auto updatelIcon = buildIconFromTheme("panther",QSize(20,20));
    updatelIcon->setContentsMargins(0,0,0,0);
    auto title = new QLabel;
    title->setText("Your panther is secure and up to date.");
    title->setMargin(0);
    title->setFont(QFont("Helvetica",8));
    title->setContentsMargins(8,0,0,0);

    auto llayout = new QHBoxLayout;
    llayout->addWidget(title);
    llayout->setAlignment(Qt::AlignLeft);
    llayout->setMargin(0);
    llayout->setSpacing(0);
    llayout->setContentsMargins(0,0,0,0);

    auto rlayout = new QHBoxLayout;
    rlayout->addWidget(updatelIcon);
    rlayout->setAlignment(Qt::AlignRight);
    rlayout->setMargin(0);
    rlayout->setSpacing(0);
    rlayout->setContentsMargins(0,0,0,0);

    auto qlayout = new QHBoxLayout;
    qlayout->addLayout(llayout);
    qlayout->addLayout(rlayout);
    qlayout->setMargin(0);
    qlayout->setSpacing(0);
    qlayout->setContentsMargins(0,0,3,0);

    auto  widget = new QWidget;
    widget->setLayout(qlayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}



string System::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
