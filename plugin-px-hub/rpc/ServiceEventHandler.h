//
// Created by fakhri on 2/20/19.
//

#ifndef PX_HUB_SERVICE_SERVICEEVENTHANDLER_H
#define PX_HUB_SERVICE_SERVICEEVENTHANDLER_H

#include <ctime>
#include "EventHandler.h"

class ServiceEventHandler: public EventHandler {
public:
    void execute(EventObject eventObject);

};


#endif //PX_HUB_SERVICE_SERVICEEVENTHANDLER_H
