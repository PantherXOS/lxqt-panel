//
// Created by fakhri on 2/17/19.
//

#include <rpc/EventSubscriber.h>
#include <XdgIcon>


EventSubscriber::EventSubscriber(string service, EventHandler *eventHandler, QToolButton *mButton) {
    subSock = { 0x00 };
    string ipcSock = "ipc://"+string(getpwuid(getuid())->pw_dir) + UTILS::FILE::fullpath(CHANNEL_BASE) + service;

    nng_sub0_open(&subSock);
    nng_setopt(subSock, NNG_OPT_SUB_SUBSCRIBE, "", 0);
    nng_dial(subSock, ipcSock.c_str(), nullptr, 0);
    this->eventHandler = eventHandler;
    this->isRun = false;
    this->mButton= mButton;
}


void EventSubscriber::run() {
    buff = nullptr;
    sz = 0;
    if(!isRun) {
        isRun=true;
        statThread = std::thread([&]() {
            while (1) {
                nng_recv(subSock, &buff, &sz, NNG_FLAG_ALLOC);
                kj::ArrayPtr<uint8_t> data(buff, sz);
                kj::ArrayInputStream strm(data);
                capnp::InputStreamMessageReader reader(strm);
                EventHandler::EventObject eventObject;
                EventData::Reader evtData = reader.getRoot<EventData>();
                eventObject.event = evtData.getEvent();
                eventObject.source = evtData.getSource();
                eventObject.time = evtData.getTime();
                eventObject.topic = evtData.getTopic();
                for (const auto &param : evtData.getParams()) {
                    eventObject.params.insert(pair<string, string>(param.getKey().cStr(), param.getValue().cStr()));
                }
//                this->eventHandler->execute(eventObject);
//                if(Common::instance().isDebugMode)
//                    cout<<this->eventHandler->toString(eventObject)<<endl;
                QMenu *menu = new QMenu;
                string title = eventObject.event +" - " + eventObject.topic;
                QAction * qact= new QAction(tr(title.c_str()));
                menu->addAction(qact);
                this->mButton->setMenu(menu);
                this->mButton->setPopupMode(QToolButton::InstantPopup);
                this->mButton->setAutoRaise(true);
                this->mButton->setIcon(XdgIcon::fromTheme("date", "date"));
            }
        });
    }
}

void EventSubscriber::stop() {
    if(isRun) {
        isRun=false;
        statThread.detach();
        nng_free(buff, sz);
        nng_close(subSock);
    }

}
