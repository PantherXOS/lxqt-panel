//
// Created by fakhri on 2/17/19.
//

#ifndef PX_HUB_SERVICE_EVENTHANDLER_H
#define PX_HUB_SERVICE_EVENTHANDLER_H
using namespace std;

#include <string>
#include <map>

class EventHandler {
public:

    struct EventObject {
        std::string topic;
        std::string source;
        uint64_t time;
        std::string event;
        std::map<std::string, std::string> params;
    };

    string toString(EventObject eventObject){
        string str = "Event: \n\r\ttopic : "+ eventObject.topic + "\n\r\t" +
                     "time  : " + to_string(eventObject.time) + "\n\r\t" +
                     "source: " + eventObject.source + "\n\r\t" +
                     "event : " + eventObject.event  + "\n\r\t" +
                     "params: " + "\n\r\t\t";
        for (auto it=eventObject.params.begin();it != eventObject.params.end(); ++it)
            str += "{ " + it->first + " : " + it->second + " }\n\r\t\t";
        return str;
    }

    virtual void execute(EventObject eventObject)=0;
};
#endif //PX_HUB_SERVICE_EVENTHANDLER_H
