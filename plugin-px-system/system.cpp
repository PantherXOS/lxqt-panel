#include "system.h"
#include "QDebug"
//
System::System(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo),
    mHidden(false)
{
    mainMenu = new QMenu;
    realign();
    mButton.setStyleSheet(QString::fromStdString("QToolButton::menu-indicator { image: none; }"));
    mButton.setMenu(mainMenu);
    mButton.setAutoRaise(true);
    mButton.setPopupMode(QToolButton::DelayedPopup);
    mButton.setIcon(QIcon::fromTheme(QString::fromStdString("px-user")));
    connect(&mButton, SIGNAL(pressed()),this,SLOT(refresh()));
}

void System::realign()
{
    mButton.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QLayout * System::netInfoRecordLayout(QString text, QString icon) {
    auto statusLabel = new PxIcon(icon,QSize(ACCOUNT_STATUS_ICON_SIZE,ACCOUNT_STATUS_ICON_SIZE));
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

void System::netInfoParser(const QVector <NetworkInformation> &internetInfo){
    auto llayout = new QVBoxLayout;
    auto Tlayout = new QVBoxLayout;
    int i=0;
    for(auto m:internetInfo){
        if(i==0){
            if(m.status)
                llayout->addLayout(netInfoRecordLayout(m.name, QString::fromStdString(":resources/icon/status_2_green_d")));
            else
                llayout->addLayout(netInfoRecordLayout(m.name, QString::fromStdString(":resources/icon/status_2_grey_d")));
        }else{
            if(m.status)
                llayout->addLayout(netInfoRecordLayout(m.name, QString::fromStdString(":resources/icon/status_3_green_ud")));
            else
                llayout->addLayout(netInfoRecordLayout(m.name, QString::fromStdString(":resources/icon/status_3_grey_ud")));
        }
        i++;
    }
    llayout->addLayout(netInfoRecordLayout(QString::fromStdString("YOU"), QString::fromStdString(":resources/icon/status_1_green_u")));
    llayout->setAlignment(Qt::AlignTop);
    llayout->setMargin(0);
    llayout->setSpacing(0);
    llayout->setContentsMargins(0,0,0,0);
    for(auto m:internetInfo){
        auto detail = new QLabel;
        detail->setText(m.value);
        detail->setMargin(0);
        detail->setContentsMargins(5,0,0,0);
        Tlayout->addWidget(detail);
    }
    Tlayout->setAlignment(Qt::AlignTop);
    Tlayout->setMargin(0);
    Tlayout->setSpacing(0);
    Tlayout->setContentsMargins(15,0,0,0);

    auto mainLayout = new QHBoxLayout;
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(llayout);
    mainLayout->addLayout(Tlayout);
    
    auto widget = new QWidget;
    widget->setObjectName(QString::fromStdString("PxSystemItem"));
    widget->setLayout(mainLayout);
    
    refresh(widget);
}

void System::refresh(QWidget *netInfo) {
    mainMenu->hide();
    mainMenu->clear();
    mainMenu->setFixedWidth(MAIN_MENU_SIZE_W);
    mainMenu->addAction(getUser());
    mainMenu->addSeparator();
    ///mainMenu->addAction(getFirewallStatus());
    auto fireWallWidget = getFirewallStatus();
    connect(fireWallWidget, SIGNAL(triggered()), this, SLOT(fireWallTriggered()));
    mainMenu->addAction(fireWallWidget);
    mainMenu->addSeparator();
    if(netInfo!=nullptr){
        auto qWidgetAction = new QWidgetAction(this);
        qWidgetAction->setDefaultWidget(netInfo);
        mainMenu->addAction(qWidgetAction);
        mainMenu->addSeparator();
        listVpnStatus.clear();
        listWifiStatus.clear();
        listLanStatus.clear();
        getConnectionState();
        setVpnStatus(listVpnStatus);
        setWifiStatus(listWifiStatus);   
        setLanStatus(listLanStatus);  
    } else {
        networkInspection = new NetworkInspection(this);
        connect(networkInspection,SIGNAL(networkInfoIsReady(const QVector <NetworkInformation> &)),this,SLOT(netInfoParser(const QVector <NetworkInformation> &)));
        networkInspection->run();

        mainMenu->addAction(networkInspection);
    }
    mainMenu->addSeparator();
    mainMenu->addSeparator();
    // mainMenu->addAction(getBTStatus());
    // mainMenu->addSeparator();
    //mainMenu->addAction(getUpdateStat());
    auto statWidget = getUpdateStat();
    connect(statWidget, SIGNAL(triggered()), this, SLOT(updateTriggered()));
    mainMenu->addAction(statWidget);
    mButton.showMenu();
    realign();
}
void System::updateTriggered(){
    QDesktopServices::openUrl(QUrl(QString::fromStdString("px-software:list=user_updates")));
}

void System::fireWallTriggered(){
    QString filDir = QDir::homePath()+QString::fromStdString("/.local/share/lxqt-panel/systemPlugin/");
    QString filename = filDir + QString::fromStdString("firewall.sh");
    if(!QDir(filDir).exists())
        QDir().mkpath(filDir);
    QFile file(filename);
    if(!file.exists()){
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << "#!/run/current-system/profile/bin/bash \nsudo nft list ruleset \n/run/current-system/profile/bin/bash" << Qt::endl;
            exec(("chmod +x "+filename.toStdString()).c_str());
        }
    }
    string cmd = "qterminal -e " + filename.toStdString()+ " &";
    exec(cmd.c_str());
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
    subject->setFont(QFont(QString::fromStdString("Helvetica"),11,QFont::Bold));

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
    sWidget->setLayout(mainLayout);
    sWidget->setContentsMargins(0,0,0,5);
    auto sWidgetAction = new QWidgetAction(this);
    sWidgetAction->setDefaultWidget(sWidget);

    return sWidgetAction;
}

QWidgetAction *System::getUser() {

    auto accountIcon = buildIconFromTheme(QString::fromStdString("px-user"),QSize(ACCOUNT_ICON_SIZE,ACCOUNT_ICON_SIZE));
    accountIcon->setContentsMargins(0,0,0,0);
    auto accountTitle = new QLabel;
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    string acc = homePath.first().split(QDir::separator()).last().toStdString();
    string displayCmd = "lslogins -o GECOS "+acc+" -u | cut -f2 -d\":\"";
    QString displayName =QString::fromStdString(exec(displayCmd.c_str()).c_str());
    if(!displayName.trimmed().isEmpty()){
        acc = displayName.trimmed().toStdString();
    }
    acc = acc.substr(0, acc.find("\n", 0));
    if(acc.size()>MAX_ACCOUNT_SIZE)
        acc = acc.substr(0,MAX_ACCOUNT_SIZE)+"...";
    accountTitle->setText(QString::fromStdString(acc));
    accountTitle->setMargin(0);
    accountTitle->setContentsMargins(7,0,0,0);

    auto llayout = new QHBoxLayout;
    llayout->addWidget(accountIcon);
    llayout->addWidget(accountTitle);
    llayout->setAlignment(Qt::AlignLeft);
    //llayout->setMargin(0);
    llayout->setSpacing(0);
//    llayout->setContentsMargins(8,0,0,0);

    auto  widget = new QWidget;
    widget->setObjectName(QString::fromStdString("PxSystemItem"));
    widget->setLayout(llayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}

QWidgetAction *System::getFirewallStatus() {
    return generalItems(QString::fromStdString("FIREWALL"),QString::fromStdString(""),false,QString::fromStdString("px-firewall"));
}


void System::setVpnStatus(QVector<NetworkInformation> listInfo) {
    for(auto inf: listInfo){
        if (inf.vpnStatus){              
            mainMenu->addAction(generalItems(QString::fromStdString("VPN"),inf.profileName,inf.status,QString::fromStdString("px-vpn")));
            
        }
    }
    mainMenu->addSeparator();
}

void System::setWifiStatus(QVector<NetworkInformation> listInfo) {
    for(auto inf: listInfo){
        if (inf.wifiStatus){
            mainMenu->addAction(generalItems(QString::fromStdString("WIFI"),inf.profileName,inf.status,QString::fromStdString("px-wifi")));
       
        }
    }  
     mainMenu->addSeparator();  
}

void System::setLanStatus(QVector<NetworkInformation> listInfo) {
    for(auto inf: listInfo){
        if (inf.lanStatus){
        mainMenu->addAction(generalItems(QString::fromStdString("LAN"),inf.profileName,inf.status,QString::fromStdString("px-lan")));
       
        }
    } 
     mainMenu->addSeparator();   
}

QWidgetAction *System::getBTStatus() {
    return generalItems(QString::fromStdString("BT"),QString::fromStdString("Logitech z533"),true,QString::fromStdString("px-bluetooth"));
}



QWidgetAction* System::generalItems(QString name,QString information,bool stat,QString icon){
    QString statusIconFile;
    if(stat){
        statusIconFile=QString::fromStdString(":resources/icon/status_0_green_o");
    }
    else{
        statusIconFile=QString::fromStdString(":resources/icon/status_0_grey");
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
   // qlayout->setMargin(0);
    qlayout->setSpacing(0);
   // qlayout->setContentsMargins(8,0,3,0);

    auto  widget = new QWidget;
    widget->setObjectName(QString::fromStdString("PxSystemItem"));
    widget->setLayout(qlayout);
    auto qWidgetAction = new QWidgetAction(this);
    qWidgetAction->setDefaultWidget(widget);
    return qWidgetAction;
}

QWidgetAction *System::getUpdateStat() {
    CheckUpdate *checkUpdate = new CheckUpdate();
    connect(checkUpdate,SIGNAL(checkUpdateReady(QString )),this,SLOT(updateHandler(QString )));
    checkUpdate->run();
    auto updatelIcon = buildIconFromTheme(QString::fromStdString("panther"),QSize(20,20));
    updatelIcon->setContentsMargins(0,0,0,0);
    updateTextLabel = new QLabel;
    updateTextLabel->setText(QString::fromStdString("Checking for updates . . ."));
    updateTextLabel->setMargin(0);
    updateTextLabel->setFont(QFont(QString::fromStdString("Helvetica"), 8));
    updateTextLabel->setContentsMargins(8, 0, 0, 0);

    auto llayout = new QHBoxLayout;
    llayout->addWidget(updateTextLabel);
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

    auto updateLayout = new QHBoxLayout;
    updateLayout->addLayout(llayout);
    updateLayout->addLayout(rlayout);
    updateLayout->setSpacing(0);

    QWidget *widgetAction = new QWidget;
    widgetAction->setObjectName(QString::fromStdString("PxSystemItem"));
    widgetAction->setLayout(updateLayout);
    auto updateWidgetAction = new QWidgetAction(this);
    updateWidgetAction->setDefaultWidget(widgetAction);
    return updateWidgetAction;
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

void System::updateHandler(QString packages) {
    string out = packages.toStdString().c_str();
    if(!packages.isEmpty()){
        std::string delimiter = ",";
        std::string token;
        size_t pos = 0;
        int count = 0;
        while((pos = out.find(delimiter)) != std::string::npos) {
                token = out.substr(0, pos);
                out.erase(0, pos + delimiter.length());
                count++;
            }
        string message;
        if (count == 1)
            message = to_string(count) +" package needs to be updated.";
        else
            message = to_string(count) +" packages need to be updated.";
        string tooltip = packages.toStdString().substr(0, packages.toStdString().size()-1);
        tooltip += " need to be updated.";
        updateTextLabel->setToolTip(QString::fromStdString(tooltip));
        updateTextLabel->setText(QString::fromStdString(message));
    }else{
        updateTextLabel->setText(QString::fromStdString("Your panther is secure and up to date."));
    }   

}


vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  return internal;
}

void System::getConnectionState(){
        string result = exec("nmcli -m tabular --terse connection show --active");   
        std::stringstream ss(result.c_str());
        std::string line;
        std::string delimiter = ":";
        std::vector<string> internal;

         if (!result.empty() )
        {
            while(std::getline(ss,line,'\n')){    
                 vector<string> sep = split(line, ':');
                 string connectionType;
                 if(sep.at(2).find('-') != std::string::npos){
                 vector<string> type = split(sep.at(2), '-');
                 connectionType = type.at(2);
                 }else{
                    connectionType = sep.at(2);
                 }
                if(sep.at(0).size()>MAX_TYPE_NAME_SIZE)
                    sep.at(0) = sep.at(0).substr(0,MAX_TYPE_NAME_SIZE)+"...";
                 if(connectionType == "wireless"){
                     NetworkInformation wifi;
                     wifi.profileName = QString::fromStdString(sep.at(0));
                     wifi.wifiStatus = true;
                     wifi.status = true;
                     listWifiStatus.push_back(wifi);
                 }
                 if(connectionType == "ethernet"){
                     NetworkInformation lan;
                     lan.profileName = QString::fromStdString(sep.at(0));
                     lan.lanStatus = true;
                     lan.status = true;
                     listLanStatus.push_back(lan);
                 }
                 if(connectionType == "vpn"){
                     NetworkInformation vpn;
                     vpn.profileName = QString::fromStdString(sep.at(0));
                     vpn.vpnStatus = true;
                     vpn.status = true;
                     listVpnStatus.push_back(vpn);
                 }
                 if(connectionType == "wireguard"){
                     NetworkInformation vpn;
                     vpn.profileName = QString::fromStdString(sep.at(0));
                     vpn.vpnStatus = true;
                     vpn.status = true;
                     listVpnStatus.push_back(vpn);
                 }

            }
        }   
}


