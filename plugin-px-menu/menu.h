
#ifndef LXQT_Menu_H
#define LXQT_Menu_H
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

using namespace std;

class Menu : public QObject, public ILXQtPanelPlugin
{
    Q_OBJECT
public:
    explicit Menu(const ILXQtPanelPluginStartupInfo &startupInfo);

    virtual QWidget *widget() { return &mButton; }
    virtual QString themeId() const { return "px-menu"; }
    virtual ILXQtPanelPlugin::Flags flags() const { return HaveConfigDialog; }
    bool isSeparate() const { return true; }
    QMenu *mainMenu = new QMenu;

public slots:
    void realign();

private slots:
//    void hubEventsHandler(EventObject *eventObject);
//    void updateButtonHandler();
private:

    void refresh();
      QWidgetAction *createTitle(QString title, QString icon);


    QToolButton mButton;
    bool mHidden;
//    bool isRun= false;
//    std::thread statThread;
};


class MenuPlugin: public QObject, public ILXQtPanelPluginLibrary
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "lxqt.org/Panel/PluginInterface/3.0")
    Q_INTERFACES(ILXQtPanelPluginLibrary)
public:
    ILXQtPanelPlugin *instance(const ILXQtPanelPluginStartupInfo &startupInfo) const
    {
        return new Menu(startupInfo);
    }
};

#endif
