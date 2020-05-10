#include <QtGui/QDesktopServices>
#include "menu.h"
#include "QDebug"
//
Menu::Menu(const ILXQtPanelPluginStartupInfo &startupInfo) :
    QObject(),
    ILXQtPanelPlugin(startupInfo),
    mHidden(false)
{
    realign();
    refresh();
    qDebug()<<QDesktopServices::openUrl(QUrl::fromLocalFile("file:///home/panther/flower"));
    mainMenu->setObjectName(QString::fromStdString("LXQtMountPopup"));
   }

void Menu::realign()
{
  mButton.setFixedHeight(panel()->iconSize());
  mButton.setFixedWidth(panel()->iconSize());
}

void Menu::refresh() {
    mainMenu->clear();

    mainMenu->setFixedWidth(MAIN_MENU_SIZE_W);
    mainMenu->addAction(createTitle(tr("YOUR FILES"), QString::fromStdString("")));

    mainMenu->addSeparator();

    mainMenu->addAction(createTitle(tr("YOUR APPLICATIONS"), QString::fromStdString("")));

    mainMenu->setObjectName(QString::fromStdString("LXQtMountPopup");
    mainMenu->addSeparator();
    mButton.setStyleSheet(QString::fromStdString("QToolButton::menu-indicator { image: none; }"));
    mButton.setMenu(mainMenu);
    mButton.setPopupMode(QToolButton::InstantPopup);
    mButton.setAutoRaise(true);
    mButton.setIcon(QIcon::fromTheme(QString::fromStdString("px-user")));
}

QWidgetAction *Menu::createTitle(QString title, QString icon) {
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
