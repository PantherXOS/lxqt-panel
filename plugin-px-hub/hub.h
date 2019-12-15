
#ifndef LXQT_NOTES_H
#define LXQT_NOTES_H

#include "../panel/ilxqtpanelplugin.h"
#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QFontMetrics>
#include <QTextEdit>
#include <QToolButton>
#include <QList>
#include <XdgIcon>
#include <string>
#include <capnp/ez-rpc.h>
#include <rpc/AccountParser.h>
#include <interfaces/hub.capnp.h>
#include <rpc/ServiceEventHandler.h>
#include <rpc/EventSubscriber.h>
#include <QtWidgets/QWidgetAction>
#include <QScrollBar>
#include "Account.h"
#include "Settings.h"

#define HUB_SERVER_ADDRESS string(getpwuid(getuid())->pw_dir) + "/.userdata/rpc/hub"

using namespace std;

class hub : public QObject, public ILXQtPanelPlugin
{
    Q_OBJECT
public:
    explicit hub(const ILXQtPanelPluginStartupInfo &startupInfo);

    virtual QWidget *widget() { return &mButton; }
    virtual QString themeId() const { return "px-hub"; }
    virtual ILXQtPanelPlugin::Flags flags() const { return HaveConfigDialog; }
    void putEvent(EventHandler::EventObject eventObject);

    bool isSeparate() const { return true; }
    QMenu *mainMenu = new QMenu;
    QDialog *configureDialog();

public slots:
    void realign();
    
protected slots:
    void settingsChanged();

private:
    QToolButton mButton;
    bool mHidden;

    string style = "background: transparent; border: none;";
    QWidgetAction* buildAccountItem(Account account);
    QWidgetAction* createTitle(QString title);

    vector<Account> getAccount();
    void refresh();
    void run();
    QMainWindow window;
    bool isRun;
    vector<EventHandler::EventObject> events;
    std::thread statThread;
};


class HubPlugin: public QObject, public ILXQtPanelPluginLibrary
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lxqt.org/Panel/PluginInterface/3.0")
    Q_INTERFACES(ILXQtPanelPluginLibrary)
public:
    ILXQtPanelPlugin *instance(const ILXQtPanelPluginStartupInfo &startupInfo) const
    {
        return new hub(startupInfo);
    }
};

#endif
