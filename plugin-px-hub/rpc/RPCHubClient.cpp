//
// Created by fakhri on 12/15/19.
//

#include "RPCHubClient.h"

#define HUB_SERVER_ADDRESS string(getpwuid(getuid())->pw_dir) + "/.userdata/rpc/hub"

vector<AccountObject> RPCHubClient::getAccountList() {
    vector<AccountObject> rsult;
    string addr = string("unix:") + HUB_SERVER_ADDRESS;
    capnp::EzRpcClient rpcClient(addr);
    auto &waitScope = rpcClient.getWaitScope();
    HubReader::Client client = rpcClient.getMain<HubReader>();
    try {
        auto actReq = client.getAccountsStatusRequest();
        auto actRes = actReq.send().wait(waitScope);
        for(auto act : actRes.getAccountsStatus()) {
            AccountObject account;
            switch (act.getStatus()){
                case AccountStatus::Status::NONE:
                    account.setStatus(Status::none);
                    break;
                case AccountStatus::Status::ERROR:
                    account.setStatus(Status::error);
                    break;
                case AccountStatus::Status::ONLINE:
                    account.setStatus(Status::online);
                    break;
                case AccountStatus::Status::OFFLINE:
                    account.setStatus(Status::offline);
                    break;
            }
            account.setTime(act.getTime());
            account.setUnread(act.getUnread());
            account.setType(act.getType().cStr());
            account.setIcon(act.getIcon().cStr());
            account.setLink(act.getLink().cStr());
            account.setAltLink(act.getAltLink().cStr());
            account.setTitle(act.getTitle().cStr());

            rsult.push_back(account);
        }
    } catch (kj::Exception){

    }
    return rsult;
}

vector<MessageObject> RPCHubClient::getMessageList(int n) {
    vector<MessageObject> messageList;
    string addr = string("unix:") + HUB_SERVER_ADDRESS;
    capnp::EzRpcClient rpcClient(addr);
    auto &waitScope = rpcClient.getWaitScope();
    HubReader::Client client = rpcClient.getMain<HubReader>();
    try {
        auto msgReq = client.getLastMessagesRequest();
        msgReq.setCount(n);
        auto msgRes = msgReq.send().wait(waitScope);
        for(auto msg : msgRes.getMessages()){
            MessageObject msgObj;
            msgObj.setIcon(msg.getIcon().cStr());
            msgObj.setMessage(msg.getMessage().cStr());
            msgObj.setSender(msg.getSender().cStr());
            msgObj.setLink(msg.getLink().cStr());
            msgObj.setAltLink(msg.getAltLink().cStr());
            msgObj.setTime(UTILS::timestampToString(msg.getTime()));
            messageList.push_back(msgObj);
        }
    } catch (kj::Exception e){
        cout << e.getDescription().cStr() << endl;
    }
    return messageList;
}
