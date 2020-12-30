@0x90f740e38f16890d;

struct HUBAccountInfo {
        id      @0 : Text;
        title   @1 : Text;
}

struct HUBAccountStatus{
	    time	    @0 : Text;
        type        @1 : Text;
        icon        @2 : Text;
        link        @3 : Text;
        altLink     @4 : Text;
        accountInfo @5 : HUBAccountInfo;
        unread      @6 : UInt16;
        status      @7 : Status;
        subType     @8 : Text;

	enum Status {
		none 	@0;
		online 	@1;
		offline @2;
		error 	@3;
		}
}

struct HUBMessage{
	    time	    @0 : Text;
	    msgId	    @1 : Text;
        accountInfo @2 : HUBAccountInfo;
        source      @3 : Text;
        type        @4 : Text;
        icon        @5 : Text;
        link        @6 : Text;
        altLink     @7 : Text;
        sender      @8 : Text;
        senderName  @9 : Text;
        title       @10: Text;
        message     @11: Text;
        subType     @12: Text;
        unread      @13: Bool;
}

interface HubReader{
        getLastMessages @0 (count: Int8 ) -> (messages:List(HUBMessage));
        getLastMessage @1 (accountID: Text ) -> (message: HUBMessage);
        getAccountStatus @2 (accountID: Text) -> (accountStatus: HUBAccountStatus);
        getAccountsStatus @3 () -> (accountsStatus:List(HUBAccountStatus));
}

struct HubEvent {
    struct HubEventStruct {
        source  @0 : Text;
        type    @1 : EventType;
        params  @2 : List(Param);
    }

    enum EventType {
        none         @0;
        accountStatusEvent @1;
        messageEvent @2;
    }

    struct Param {
        key   @0 : Text;
        value @1 : Text;
    }
}