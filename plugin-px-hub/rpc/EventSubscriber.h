//
// Created by Fakhri Sajadi on 2/17/19.
//

#ifndef PX_HUB_SERVICE_EVENTSUBSCRIBER_H
#define PX_HUB_SERVICE_EVENTSUBSCRIBER_H

using namespace std;

#include <string>

#include <capnp/serialize-packed.h>
#include "interfaces/event.capnp.h"

#include <nng/nng.h>
#include <nng/protocol/pipeline0/push.h>
#include <nng/protocol/pubsub0/sub.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <map>
#include <pwd.h>
#include <unistd.h>
#include "Utils.h"
#include "EventHandler.h"
#include <QToolButton>
#include <QMenu>
#include <qobjectdefs.h>
#include <hub.h>

#define CHANNEL_BASE    "/.userdata/event/channels/"
class hub;

class EventSubscriber {

public:
    EventSubscriber(string service, EventHandler *eventHandler, hub *hub);
    void run();
    void stop();

    nng_socket subSock;

private:
    bool isRun;
    std::thread statThread;
    size_t sz;
    unsigned char* buff;
    EventHandler *eventHandler;
    hub *_hub;
    QToolButton *mButton;
};


#endif //PX_HUB_SERVICE_EVENTSUBSCRIBER_H
