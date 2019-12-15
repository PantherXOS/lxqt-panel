//
// Created by fakhri on 2/17/19.
//

#include <rpc/EventSubscriber.h>
#include <XdgIcon>
#include "hub.h"


EventSubscriber::EventSubscriber(string service, EventHandler *eventHandler, hub *hub) {
    subSock = { 0x00 };
    string ipcSock = "ipc://"+string(getpwuid(getuid())->pw_dir) + UTILS::FILE::fullpath(CHANNEL_BASE) + service;

    nng_sub0_open(&subSock);
    nng_setopt(subSock, NNG_OPT_SUB_SUBSCRIBE, "", 0);
    nng_dial(subSock, ipcSock.c_str(), nullptr, 0);
    this->eventHandler = eventHandler;
    this->isRun = false;
    this->_hub = hub;
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
                this->_hub->putEvent(eventObject);
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
