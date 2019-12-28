
#ifndef LXQT_HUB_H
#define LXQT_HUB_H
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
#include "../panel/ilxqtpanelplugin.h"

#include "rpc/RPCHubClient.h"
#include "AccountObject.h"
#include "Settings.h"
#include "MessageObject.h"
#include <rpc/ServiceEventHandler.h>
#include <rpc/EventSubscriber.h>

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

public slots:
    void realign();

private:
    QLabel *buildIconFromFile(QString file, QSize size);
    QWidgetAction* buildAccountItem(AccountObject account);
    QWidgetAction* buildMessageItem(MessageObject message);
    QWidgetAction* createTitle(QString title);
    void refresh();
    void run();

    QToolButton mButton;
    bool mHidden;
    bool isRun= false;
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
