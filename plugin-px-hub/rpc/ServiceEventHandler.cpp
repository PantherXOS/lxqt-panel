//
// Created by fakhri on 2/20/19.
//

#include "ServiceEventHandler.h"


void ServiceEventHandler::execute(EventObject eventObject){
//
//        string imapAccounts = eventObject.params.find("accounts")->second;
//        string service = eventObject.params.find("service")->second;
//        string delimiter = ",";
//        size_t pos = 0;
//        vector<string> accounts;
//        while ((pos = imapAccounts.find(delimiter)) != string::npos) {
//            accounts.push_back(imapAccounts.substr(0, pos));
//            imapAccounts.erase(0, pos + delimiter.length());
//        }
//
//        for(auto acc:accounts){
//          for(auto sysAcc : Common::instance().getSystemAccounts()){
//              if(sysAcc.getName() == acc){
//                  if(PluginManager::Instance().exists(service)) {
//                      auto plugin = PluginManager::Instance().plugins()[service];
//                      HubStruct hubStruct = plugin.refresh(acc);
//                      PluginDataContainer pluginDataContainer(hubStruct, plugin);
//                      for (auto msg:pluginDataContainer.getMessages()) {
//                          HubDatabase::instance().insertMessage(msg);
//                      }
//                      EventHandler::EventObject  eventObject;
//                      eventObject.event = "hub";
//                      eventObject.topic = "Service_refresh";
//                      eventObject.source = "px-hub-service";
//                      eventObject.time = time(nullptr);
//                      eventObject.params["account"] = acc;
//                      eventObject.params["service"] = service;
//                      EventPublisher eventPublisher;
//                      eventPublisher.emit(eventObject);
//                  }
//              }
//          }
//        }
}