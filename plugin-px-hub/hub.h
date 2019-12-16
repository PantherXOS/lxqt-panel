
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
#include <rpc/ServiceEventHandler.h>
#include <rpc/EventSubscriber.h>
#include <QtWidgets/QWidgetAction>
#include <QScrollBar>
#include <QFrame>
#include <QFont>
#include "AccountObject.h"
#include "Settings.h"
#include "MessageObject.h"

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

    string style = "border: none;";
    QWidgetAction* buildAccountItem(AccountObject account);
    QWidgetAction* buildMessageItem(MessageObject message);
    QWidgetAction* createTitle(QString title);

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
