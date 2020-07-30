@0xc893ae030f1e9308;

struct AccountStatus{
	    time	@0 : Text;
        type    @1 : Text;
        icon    @2 : Text;
        link    @3 : Text;
        altLink @4 : Text;
        title   @5 : Text;
        unread  @6 : UInt16;
        status  @7 : Status;
        subType @8 : Text;

	enum Status {
		none 	@0;
		online 	@1;
		offline @2;
		error 	@3;
		}
}

struct Message{
	    time	    @0 : Text;
	    msgId	    @1 : Text;
	    account     @2 : Text;
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
        getLastMessages     @0 (count: Int8 )   -> (messages:List(Message));
        getLastMessage      @1 (account: Text ) -> (message: Message);
        getAccountStatus    @2 (account: Text)  -> (accountStatus: AccountStatus);
        getAccountsStatus   @3 ()               -> (accountsStatus:List(AccountStatus));
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