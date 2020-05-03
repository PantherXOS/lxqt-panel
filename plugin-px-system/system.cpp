#include "system.h"
#include "QDebug"
//
System::System(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo),
    mHidden(false)
{
    realign();
    mainMenu->setObjectName("LXQtSystem");
    mButton.setStyleSheet("QToolButton::menu-indicator { image: none; }");
    mButton.setMenu(mainMenu);
    mButton.setPopupMode(QToolButton::DelayedPopup);
    mButton.setIcon(QIcon::fromTheme("px-user"));
    connect(&mButton, SIGNAL(pressed()),this,SLOT(refresh()));
}

void System::realign()
{
  mButton.setFixedHeight(panel()->iconSize());
  mButton.setFixedWidth(panel()->iconSize());
}

void System::refresh() {
    mainMenu->clear();
    mainMenu->setFixedWidth(MAIN_MENU_SIZE_W);
    mainMenu->addAction(getUser());
   // string data = exec("px-network-inspection");
   string data = " { \"primary\": [ { \"pos\": 0, \"adapter\": \"PUBLIC\", \"method\": \"NONE\", \"type\": \"display\", \"ip4\": \"37.59.236.227\", \"ip6\": \"\", \"dns\": \"\", \"gateway\": \"\", \"status\": \"ACTIVE\" }, { \"pos\": 1, \"adapter\": \"enp0s3\", \"method\": \"LAN\", \"type\": \"physical\", \"ip4\": \"10.0.2.15\", \"ip6\": \"\", \"dns\": \"\", \"gateway\": \"10.0.2.2\", \"status\": \"ACTIVE\" }, { \"pos\": 2, \"adapter\": \"tun0\", \"method\": \"OPENVPN\", \"type\": \"virtual\", \"ip4\": \"172.16.100.93\", \"ip6\": \"\", \"dns\": \"\", \"gateway\": \"37.59.236.227\", \"status\": \"ACTIVE\", \"profile\": \"client_sinap\" } ] }";
    networkDataParser(data);
    mainMenu->addSeparator();
    mainMenu->addAction(getFirewallStatus());
    mainMenu->addSeparator();
    mainMenu->addAction(getInternet());
    mainMenu->addSeparator();
    getVpnStatus();
    getWifiStatus();
    mainMenu->addSeparator();
    mainMenu->addAction(getBTStatus());
    mainMenu->addSeparator();
    mainMenu->addAction(getUpdateStat());   mButton.showMenu();
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
    llayout->setContentsMargins(8,0,0,0);

    auto  widget = new QWidget;
    widget->setObjectName("PxSystemItem");
    widget->setLayout(llayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}

QWidgetAction *System::getFirewallStatus() {
    return generalItems("FIREWALL","",false,"px-firewall");
}

QWidgetAction *System::getInternet() {
    auto llayout = new QVBoxLayout;
    auto Tlayout = new QVBoxLayout;
    int i=0;
    //for(auto it =internetInfo.rbegin();it!=internetInfo.rend();++it){
    for(auto m:internetInfo){
        if(i==0){
            if(m.status)
                llayout->addLayout(internetLayout(m.name.c_str(), ":resources/icon/status_2_green_d"));
            else
                llayout->addLayout(internetLayout(m.name.c_str(), ":resources/icon/status_2_grey_d"));
        }else{
            if(m.status)
                llayout->addLayout(internetLayout(m.name.c_str(), ":resources/icon/status_3_green_ud"));
            else
                llayout->addLayout(internetLayout(m.name.c_str(), ":resources/icon/status_3_grey_ud"));
        }
        i++;
    }
    llayout->addLayout(internetLayout("YOU", ":resources/icon/status_1_green_u"));
    llayout->setAlignment(Qt::AlignTop);
    llayout->setMargin(0);
    llayout->setSpacing(0);
    llayout->setContentsMargins(0,0,0,0);
    //for(auto it =internetInfo.rbegin();it!=internetInfo.rend();++it){
    for(auto m:internetInfo){
        auto detail = new QLabel;
        detail->setText(m.value.c_str());
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
    widget->setObjectName("PxSystemItem");
    widget->setLayout(rlayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}


QLayout * System::internetLayout(QString text, QString icon) {
auto statusLabel = buildIconFromFile(icon,QSize(ACCOUNT_STATUS_ICON_SIZE,ACCOUNT_STATUS_ICON_SIZE));
statusLabel->setMargin(0);
statusLabel->setContentsMargins(0,0,0,0);
auto Hlayout = new QHBoxLayout;
auto title = new QLabel;
title->setText(text);
title->setMargin(0);
title->setContentsMargins(9,0,0,0);
Hlayout->addWidget(statusLabel);
Hlayout->addWidget(title);
Hlayout->setAlignment(Qt::AlignLeft);
Hlayout->setMargin(0);
Hlayout->setSpacing(0);
return Hlayout;
}

void System::getVpnStatus() {
    for(auto info:internetInfo){
        if (info.vpnStatus){
            mainMenu->addAction(generalItems("VPN",info.profileName.c_str(),info.status,"px-vpn"));
            mainMenu->addSeparator();
        }
    }
}

void System::getWifiStatus() {
    for(auto info:internetInfo){
        if (info.wifiStatus){
            mainMenu->addAction(generalItems("WIFI",info.profileName.c_str(),info.status,"px-wifi"));
            mainMenu->addSeparator();
        }
    }
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
    qlayout->setContentsMargins(8,0,3,0);

    auto  widget = new QWidget;
    widget->setObjectName("PxSystemItem");
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
    widget->setObjectName("PxSystemItem");
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

bool System::networkDataParser(string data) {
    Document document;
    document.Parse(data.c_str());
    if (document["primary"].IsArray()) {
        internetInfo.clear();
        try {
            for (rapidjson::Value::ConstValueIterator itr = document["primary"].Begin(); itr != document["primary"].End(); ++itr) {
                const rapidjson::Value& attribute = *itr;
                if(attribute.HasMember("method")&&attribute.HasMember("ip4")) {
                    if(attribute["pos"].GetInt() == 0) {
                        NetworkInformation networkInformation;
                        networkInformation.name = attribute["adapter"].GetString();
                        networkInformation.value = attribute["ip4"].GetString();
                        if(attribute["status"].GetString() == string("ACTIVE"))
                        networkInformation.status =true;
                        internetInfo.push_back(networkInformation);
                    }
                    else {
                        NetworkInformation networkInformation;
                        networkInformation.name = attribute["method"].GetString();
                        networkInformation.value = attribute["ip4"].GetString();
                        if(attribute["status"].GetString() == string("ACTIVE"))
                            networkInformation.status =true;
                        if(attribute["type"].GetString() == string("virtual")){
                            networkInformation.vpnStatus = true;
                            networkInformation.profileName = attribute["profile"].GetString();
                        }
                        if(attribute["method"].GetString() == string("WIFI")){
                            networkInformation.wifiStatus = true;
                            networkInformation.profileName = attribute["essid"].GetString();
                        }
                        internetInfo.push_back(networkInformation);
                    }
                }
            }
        } catch (exception e) {
            qDebug()<<"Error in json parser!!! ";
        }
    }
    return true;

}
