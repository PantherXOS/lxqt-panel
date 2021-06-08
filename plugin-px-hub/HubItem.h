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
#include <QListWidget>
#include "MessageObject.h"
#include "Settings.h"

class HubItemWidget : public QWidget {
public:
    HubItemWidget(MessageObject &message, int width, QWidget *parent = nullptr) : QWidget(parent) {
        messageLink = QString::fromStdString(message.getLink());
        auto messageSender = new QLabel;
        QFont messageSenderFont = messageSender->font();
        messageSenderFont.setPointSize(MSG_SENDER_FONT_SIZE);
        
        QFont messageTimeFont = messageSender->font();
        messageTimeFont.setPointSize(MSG_TIME_FONT_SIZE);

        QFont messagePreviewFont = messageSender->font();
        messagePreviewFont.setPointSize(MSG_PREVIEW_FONT_SIZE);
        if(message.isUnread()){
            messageSenderFont.setItalic(false);
            messageTimeFont.setItalic(false);
            messagePreviewFont.setPointSize(MSG_UNREAD_PREVIEW_FONT_SIZE);
            messagePreviewFont.setItalic(false);
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
        getVisibleTime(QString::fromStdString(message.getTime()));
        messageTime->setText(getVisibleTime(QString::fromStdString(message.getTime())));
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
        int textWidth =fm.horizontalAdvance(QString::fromStdString(message.getMessage()));
        auto _msg = message.getMessage();
        _msg.erase(std::remove(_msg.begin(), _msg.end(), '\n'),
            _msg.end());
        while(textWidth > width){
            _msg = _msg.substr(0, _msg.size() - 6);
            _msg += "...";
            textWidth =fm.horizontalAdvance(QString::fromStdString(_msg));
        }
        
        auto messagePreview = new QLabel;
        messagePreview->setText(QString::fromStdString(_msg));
        messagePreview->setFont(messagePreviewFont);
        messagePreview->setContentsMargins(0,0,0,0);

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

        
        QFrame * frame = new QFrame();
        frame->setFrameShape(QFrame::HLine);
        frame->setFrameShadow(QFrame::Plain);
        frame->setLineWidth(0);
        frame->setMidLineWidth(0);
        auto sepraterlayout = new QVBoxLayout;
        //sepraterlayout->addLayout(qlayout);
        sepraterlayout->addWidget(frame);
        sepraterlayout->setAlignment(Qt::AlignBottom);

        auto glayout = new QHBoxLayout;
        glayout->addLayout(ilayout);
        glayout->addLayout(Tlayout);
        glayout->setMargin(5);
        // glayout->setSpacing(0);
        // glayout->setContentsMargins(0,0,0,0);

        auto flayout = new QVBoxLayout;
        flayout->addLayout(glayout);
        flayout->addLayout(sepraterlayout);
        flayout->setMargin(0);
        flayout->setSpacing(0);
        flayout->setContentsMargins(0,0,0,0);



        setObjectName(QString::fromStdString("PxHubItem"));
        setLayout(flayout);
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

    QString getMessageLink(){
        return messageLink;
    }

private:
    QString messageLink;
    QString getVisibleTime(QString messageTime){
        QDateTime dt = QDateTime::fromString(messageTime, 
                                            Qt::ISODate).toLocalTime();
        qint64 difference = dt.daysTo(QDateTime::currentDateTime());
        if(difference>=1){
            return dt.toString(QString::fromStdString("ddd  dd.MM  h:mm a"));
        }else{
            QString result;
            qint64 secDifference = dt.secsTo(QDateTime::currentDateTime());
            if((secDifference/60)>90){
               result= QString::number(secDifference/3600)+QString::fromStdString("h ago"); 
            }else{
                 result= QString::number(secDifference/60)+QString::fromStdString("m ago");
            }
            return result;
        }

    }
};

class HubItem : public QWidgetAction{
Q_OBJECT
public:
    HubItem(vector<MessageObject> &messageList, QObject *parent = nullptr)
            : QWidgetAction(parent) {
        //_url = QString::fromStdString(message.getLink());
       // auto widget = new HubItemWidget(message);
        listWidget = new QListWidget();
        listWidget->setStyleSheet(QString::fromStdString("QListWidget {background-color:transparent;}"));
        for(auto &m : messageList){
            auto widgwtItem = new QListWidgetItem();
            listWidget->addItem(widgwtItem);
            auto messageItem = new HubItemWidget(m,(MAIN_MENU_SIZE_W - listWidget->horizontalScrollBar()->size().height()-50)); 
            
            messageHeightSize = messageItem->sizeHint().height();
            listWidget->setItemWidget(widgwtItem,messageItem);
            widgwtItem->setSizeHint(messageItem->size()); 
        }
        int listSize;
        if(messageList.size()< MESSAGE_VIEW_COUNT)
            listSize = messageHeightSize*messageList.size();
        else
            listSize = messageHeightSize*MESSAGE_VIEW_COUNT;
        listWidget->setFixedHeight(listSize);
        
        setDefaultWidget(listWidget);
        connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(triggered(QListWidgetItem*)));
    }

private slots:
    void triggered(QListWidgetItem* item){
        QDesktopServices::openUrl(QUrl(((HubItemWidget*)(listWidget->itemWidget(item)))->getMessageLink()));
    }

private:
    // QString _url;
    int messageHeightSize;
    int frameWidth;
    QListWidget* listWidget;
};



#endif //LXQT_PANEL_HUBITEM_H
