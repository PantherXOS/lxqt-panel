
#ifndef LXQT_SYSTEM_H
#define LXQT_SYSTEM_H
#include <QMouseEvent>
#include <QResizeEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <QStandardPaths>
#include <QSettings>
#include <QLabel>
#include <QDateTime>
#include <QMessageBox>
#include <QDir>
#include <QFontDialog>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QtGui/QPainter>
#include <QtWidgets/QScrollArea>
#include <XdgIcon>
#include <string>
#include <QtWidgets/QWidgetAction>
#include <QScrollBar>
#include <QFrame>
#include <QFont>
#include <QIcon>
#include <QToolButton>
#include <QtWidgets/QPushButton>
#include "../panel/ilxqtpanelplugin.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include "Settings.h"
#include <lxqt/LXQt/Notification>
#include "rapidjson/document.h"
#include "NetworkInformation.h"
#include "CheckUpdate.h"
#include "NetworkInspection.h"
#include "PxIcon.h"

using namespace std;
using namespace rapidjson;

class System : public QObject, public ILXQtPanelPlugin
{
    Q_OBJECT
public:
    explicit System(const ILXQtPanelPluginStartupInfo &startupInfo);

    virtual QWidget *widget() { return &mButton; }
    virtual QString themeId() const { return QString::fromStdString("px-system"); }
    virtual ILXQtPanelPlugin::Flags flags() const { return HaveConfigDialog; }
    bool isSeparate() const { return true; }
    QMenu *mainMenu;

public slots:
    void realign();


private slots:
    void refresh(QWidget *netInfo=nullptr);
    void updateHandler(QString packages);
    void netInfoParser(const QVector <NetworkInformation> &netInfos);

private:
    QLayout *netInfoRecordLayout(QString text, QString icon);
    QLabel *buildIconFromFile(QString file, QSize size);
    QLabel *buildIconFromTheme(QString icon, QSize size);
    QWidgetAction *createTitle(QString title, QString icon);
    QWidgetAction* getUser();
    QWidgetAction* getFirewallStatus();
    void setVpnStatus(NetworkInformation vpnStatus);
    void setWifiStatus(NetworkInformation wifiStatus);
    QWidgetAction* getBTStatus();
    QWidgetAction* getUpdateStat();
    QLayout *internetLayout(QString text, QString icon);
    QWidgetAction* generalItems(QString name,QString information,bool stat,QString icon);
    string exec(const char* cmd);
    QToolButton mButton;
    bool mHidden;
    string vpnName;
    QLabel *updateTextLabel;
    NetworkInspection *networkInspection;
};


class SystemPlugin: public QObject, public ILXQtPanelPluginLibrary
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lxqt.org/Panel/PluginInterface/3.0")
    Q_INTERFACES(ILXQtPanelPluginLibrary)
public:
    ILXQtPanelPlugin *instance(const ILXQtPanelPluginStartupInfo &startupInfo) const
    {
        return new System(startupInfo);
    }
};

#endif
