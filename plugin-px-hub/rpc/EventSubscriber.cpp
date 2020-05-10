//
// Created by fakhri on 2/17/19.
//

#include <rpc/EventSubscriber.h>
#include <XdgIcon>
#include "hub.h"


EventSubscriber::EventSubscriber(string service) {
    this->isRun = false;
    this->service = service;
}


void EventSubscriber::run() {
    buff = nullptr;
    sz = 0;

    if(!isRun) {
        isRun=true;
        statThread = std::thread([&]() {
            int res=-1;
            while(res != 0){
                subSock = { 0x00 };
                string ipcSock = "ipc://"+string(getpwuid(getuid())->pw_dir) + UTILS::FILE::fullpath(CHANNEL_BASE) + service;
                nng_sub0_open(&subSock);
                nng_setopt(subSock, NNG_OPT_SUB_SUBSCRIBE, "", 0);
                res = nng_dial(subSock, ipcSock.c_str(), nullptr, 0);
                std::this_thread::sleep_for(std::chrono::seconds (5));
            }
            while (isRun) {
                nng_recv(subSock, &buff, &sz, NNG_FLAG_ALLOC);
                kj::ArrayPtr<uint8_t> data(buff, sz);
                kj::ArrayInputStream strm(data);
                capnp::InputStreamMessageReader reader(strm);
                EventObject *eventObject=new EventObject;
                EventData::Reader evtData = reader.getRoot<EventData>();
                eventObject->setEvent(QString::fromStdString(evtData.getEvent()));
                eventObject->setSource(QString::fromStdString(evtData.getSource()));
                eventObject->setTime(evtData.getTime());
                eventObject->setTopic(QString::fromStdString(evtData.getTopic()));
                map<QString,QString> m;
                for (const auto &param : evtData.getParams()) {
                    m.insert(pair<QString, QString>(QString::fromStdString(param.getKey()), QString::fromStdString(param.getValue())));
                }
                eventObject->setParams(m);
                qDebug() <<eventObject->toString();
                emit hubEvents(eventObject);
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
