
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
#include <QIcon>
#include <QDateTime>
#include <QPushButton>
#include <rpc/EventSubscriber.h>
#include <lxqt/LXQt/Notification>
#include <QListWidget>
#include <QTimer>
#include "../panel/ilxqtpanelplugin.h"
#include "rpc/RPCHubClient.h"
#include "AccountObject.h"
#include "Settings.h"
#include "MessageObject.h"
#include "LineSeperator.h"
#include "HubItem.h"
#include "AccountItem.h"
#include "CalendarBox.h"
#include "CalendarItemWidget.h"
#include "TimeBoxObject.h"

using namespace std;

class hub : public QObject, public ILXQtPanelPlugin
{
    Q_OBJECT
public:
    explicit hub(const ILXQtPanelPluginStartupInfo &startupInfo);

    virtual QWidget *widget() { return &mButton; }
    virtual QString themeId() const { return QStringLiteral("px-hub"); }
    virtual ILXQtPanelPlugin::Flags flags() const { return HaveConfigDialog; }
    bool isSeparate() const { return true; }
    QMenu *mainMenu = new QMenu;

public slots:
    void realign();

private slots:
    void hubEventsHandler(EventObject *eventObject);
    void updateButtonHandler();
    void pantherButtonHandler();
    void accountButtonHandler();
    void mainMenuTriggeredHandler(QAction*);
    void refresh();
private:
    QWidgetAction *createTitle(QString title, QString icon);
    QWidgetAction  *createAccountButton();   
    QVector<TimeBoxObject> getCalendar();
    QPushButton* qPushButton;
    QToolButton mButton;
    bool mHidden;
    bool isRun= false;
    std::thread statThread;
    QTimer refreshTimer;
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
