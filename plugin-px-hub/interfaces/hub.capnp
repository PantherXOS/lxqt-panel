@0xc893ae030f1e9308;

struct AccountStatus{
	    time	@0 : UInt64;
        type    @1 : Text;
        icon    @2 : Text;
        link    @3 : Text;
        altLink @4 : Text;
        title   @5 : Text;
        unread  @6 : UInt16;
        status  @7 : Status;

	enum Status {
		none 	@0;
		online 	@1;
		offline @2;
		error 	@3;
		}
}

struct Message{
	    time	    @0 : UInt64;
	    msgId	    @1 : UInt64;
	    account     @2 : Text;
        source      @3 : Text;
        type        @4 : Text;
        icon        @5 : Text;
        link        @6 : Text;
        sender      @7 : Text;
        senderName  @8 : Text;
        title       @9 : Text;
        message     @10: Text;
}

interface HubReader{
        getLastMessages @0 (count: Int8 ) -> (messages:List(Message));
        getLastMessage @1 (account: Text ) -> (message: Message);
        getAccountStatus @2 (account: Text) -> (accountStatus: AccountStatus);
        getAccountsStatus @3 () -> (accountsStatus:List(AccountStatus));
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