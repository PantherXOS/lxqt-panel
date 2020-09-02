//
// Created by fakhri on 3/31/20.
//

#include "NetworkInspection.h"

NetworkInspection::NetworkInspection(QObject *parent) : QWidgetAction(parent){
    auto updateTextLabel = new QLabel;
    updateTextLabel->setText(QString::fromStdString("Updating . . ."));
    updateTextLabel->setMargin(0);
    updateTextLabel->setFont(QFont(QString::fromStdString("Helvetica"), 8));
    updateTextLabel->setContentsMargins(8, 0, 0, 0);

    auto llayout = new QHBoxLayout;
    llayout->addWidget(updateTextLabel);
    llayout->setAlignment(Qt::AlignLeft);
    llayout->setMargin(0);
    llayout->setSpacing(0);
    llayout->setContentsMargins(0,0,0,0);

    QWidget *widgetAction = new QWidget;
    widgetAction->setObjectName(QString::fromStdString("PxSystemItem"));
    widgetAction->setLayout(llayout);
    setDefaultWidget(widgetAction);
}

void NetworkInspection::run(){
    statThread = std::thread([&]() {
        string data = exec("px-network-inspection");
        auto internetInfo = parseResponse(data);
        emit networkInfoIsReady(internetInfo);
    });
}

QVector<NetworkInformation> NetworkInspection::parseResponse(string data){
    QVector<NetworkInformation> _internetInfo;
    Document document;
    document.Parse(data.c_str());
    if (document["primary"].IsArray()) {
        try {
            for (rapidjson::Value::ConstValueIterator itr = document["primary"].Begin(); itr != document["primary"].End(); ++itr) {
                const rapidjson::Value& attribute = *itr;
                if(attribute.HasMember("method")&&attribute.HasMember("ip4")) {
                    if(attribute["pos"].GetInt() == 0) {
                        NetworkInformation networkInformation;
                        networkInformation.name = QString::fromStdString(attribute["adapter"].GetString());
                        networkInformation.value = QString::fromStdString(attribute["ip4"].GetString());
                        if(attribute["status"].GetString() == string("ACTIVE"))
                        networkInformation.status =true;
                        _internetInfo.push_back(networkInformation);
                    } else {
                        NetworkInformation networkInformation;
                        networkInformation.name = QString::fromStdString(attribute["method"].GetString());
                        networkInformation.value = QString::fromStdString(attribute["ip4"].GetString());
                        if(attribute["status"].GetString() == string("ACTIVE"))
                            networkInformation.status =true;
                        if(attribute["type"].GetString() == string("virtual")){
                            networkInformation.vpnStatus = true;
                            networkInformation.profileName = QString::fromStdString(attribute["profile"].GetString());
                            vpnStatus = networkInformation;
                        }
                        if(attribute["method"].GetString() == string("WIFI")){
                            networkInformation.wifiStatus = true;
                            networkInformation.profileName = QString::fromStdString(attribute["essid"].GetString());
                            wifiStatus = networkInformation;
                        }
                        _internetInfo.push_back(networkInformation);
                    }
                }
            }
        } catch (exception e) {
            qDebug()<<"Error in json parser!!! ";
        }
    }
    return _internetInfo;
}

NetworkInformation NetworkInspection::getVpnStatus() {
    return vpnStatus;
}

NetworkInformation NetworkInspection::getWifiStatus() {
    return wifiStatus;
}

string NetworkInspection::exec(const char* cmd) {
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
