#ifndef LXQT_PANEL_HUBITEM_H
#define LXQT_PANEL_HUBITEM_H

#include <QDebug>
#include <QString>
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QtGui/QDesktopServices>
#include <QUrl>
#include <QMouseEvent>
#include <QDateTime>
#include <XdgIcon>
#include <string>
#include <QtWidgets/QWidgetAction>
#include <QScrollBar>
#include <QFrame>
#include <QFont>
#include <QIcon>
#include "MessageObject.h"
#include "Settings.h"

class HubItemWidget : public QWidget {
public:
    HubItemWidget(MessageObject &message, QWidget *parent = nullptr) : QWidget(parent) {
        auto messageSender = new QLabel;
        QFont messageSenderFont = messageSender->font();
        messageSenderFont.setPointSize(MSG_SENDER_FONT_SIZE);
        
        QFont messageTimeFont = messageSender->font();
        messageTimeFont.setPointSize(MSG_TIME_FONT_SIZE);

        QFont messagePreviewFont = messageSender->font();
        messagePreviewFont.setPointSize(MSG_PREVIEW_FONT_SIZE);
        if(message.isUnread()){
            messageSenderFont.setItalic(true);
            messageTimeFont.setItalic(true);
            messagePreviewFont.setPointSize(MSG_UNREAD_PREVIEW_FONT_SIZE);
            messagePreviewFont.setItalic(true);
        }
        string acc = message.getSender();
        if(acc.size()>MAX_ACCOUNT_SIZE)
            acc = acc.substr(0,MAX_ACCOUNT_SIZE)+"...";
        messageSender->setText(QString::fromStdString(acc));
        messageSender->setFont(messageSenderFont);

        auto llayout = new QHBoxLayout;
        llayout->addWidget(messageSender);
        llayout->setAlignment(Qt::AlignLeft);
        llayout->setMargin(0);
        llayout->setSpacing(0);
        llayout->setContentsMargins(0,0,0,0);

        auto messageTime = new QLabel;
        QDateTime dt = QDateTime::fromString(QString::fromStdString(message.getTime()), 
                                            Qt::ISODate).toLocalTime();
        messageTime->setText(dt.toString(QString::fromStdString("yyyy-MM-dd hh:mm")));
        messageTime->setFont(messageTimeFont);

        auto rlayout = new QHBoxLayout;
        rlayout->addWidget(messageTime);
        rlayout->setAlignment(Qt::AlignRight);
        rlayout->setMargin(0);
        rlayout->setSpacing(0);
        rlayout->setContentsMargins(0,0,3,0);

        auto qlayout = new QHBoxLayout;
        qlayout->addLayout(llayout);
        qlayout->addLayout(rlayout);
        qlayout->setMargin(0);
        qlayout->setSpacing(0);
        qlayout->setContentsMargins(0,0,0,0);


        QFontMetrics fm(messagePreviewFont);
        int textWidth =fm.width(QString::fromStdString(message.getMessage()));
        auto _msg = message.getMessage();
        while(textWidth > MAX_MESSAGE_SIZE_WIDTH){
            _msg = _msg.substr(0, _msg.size() - 6);
            _msg += "...";
            textWidth =fm.width(QString::fromStdString(_msg));
        }
        
        auto messagePreview = new QLabel;
        messagePreview->setText(QString::fromStdString(_msg));
        messagePreview->setFont(messagePreviewFont);
        messagePreview->setContentsMargins(0,2,0,0);

        auto Tlayout = new QVBoxLayout;
        Tlayout->addLayout(qlayout);
        Tlayout->addWidget(messagePreview);
        Tlayout->setAlignment(Qt::AlignTop);
        Tlayout->setMargin(0);
        Tlayout->setSpacing(0);
        Tlayout->setContentsMargins(7,0,0,0);
        auto messageIcon = buildIconFromTheme(QString::fromStdString(message.getIcon()), QSize(MESSAGE_ICON_SIZE,MESSAGE_ICON_SIZE));
        auto ilayout = new QHBoxLayout;
        ilayout->addWidget(messageIcon);
        ilayout->setMargin(0);
        ilayout->setSpacing(0);
        ilayout->setContentsMargins(3,0,0,0);

        auto glayout = new QHBoxLayout;
        glayout->addLayout(ilayout);
        glayout->addLayout(Tlayout);
    //    glayout->setMargin(0);
        glayout->setSpacing(0);
    //    glayout->setContentsMargins(0,0,0,0);

        setObjectName(QString::fromStdString("PxHubItem"));
        setLayout(glayout);
        setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);
        setAttribute(Qt::WA_Hover);
        setMouseTracking(true);
    }

    QLabel *buildIconFromTheme(QString icon, QSize size){
        QIcon qicon = QIcon::fromTheme(icon);
        QPixmap pixmap = qicon.pixmap(size, QIcon::Normal, QIcon::On);
        auto iconLabel = new QLabel;
        iconLabel->setAttribute(Qt::WA_TranslucentBackground);
        iconLabel->setPixmap(pixmap);
        iconLabel->setFixedSize(size);
        return iconLabel;
    }
};

class HubItem : public QWidgetAction{
Q_OBJECT
public:
    HubItem(MessageObject &message, QObject *parent = nullptr)
            : QWidgetAction(parent) {
        auto widget = new HubItemWidget(message);
        setDefaultWidget(widget);
    }
};



#endif //LXQT_PANEL_HUBITEM_H
