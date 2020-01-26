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
    subSock = { 0x00 };
    string ipcSock = "ipc://"+string(getpwuid(getuid())->pw_dir) + UTILS::FILE::fullpath(CHANNEL_BASE) + service;
    nng_sub0_open(&subSock);
    nng_setopt(subSock, NNG_OPT_SUB_SUBSCRIBE, "", 0);

    if(!isRun) {
        isRun=true;
        statThread = std::thread([&]() {
            while(nng_dial(subSock, ipcSock.c_str(), nullptr, 0) != 0){
                sleep(5);
            }
            while (isRun) {
                nng_recv(subSock, &buff, &sz, NNG_FLAG_ALLOC);
                kj::ArrayPtr<uint8_t> data(buff, sz);
                kj::ArrayInputStream strm(data);
                capnp::InputStreamMessageReader reader(strm);
                EventObject *eventObject=new EventObject;
                EventData::Reader evtData = reader.getRoot<EventData>();
                eventObject->setEvent(evtData.getEvent().cStr());
                eventObject->setSource(evtData.getSource().cStr());
                eventObject->setTime(evtData.getTime());
                eventObject->setTopic(evtData.getTopic().cStr());
                map<QString,QString> m;
                for (const auto &param : evtData.getParams()) {
                    m.insert(pair<QString, QString>(param.getKey().cStr(), param.getValue().cStr()));
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
