set(PLUGIN "hub")
find_package(CapnProto CONFIG REQUIRED)
find_package(nng REQUIRED)

capnp_generate_cpp(CapnpSources CapnpHeaders
		interfaces/hub.capnp interfaces/event.capnp)

include_directories(${CapnProto_INCLUDE_DIRS})

set(HEADERS
		hub.h
		AccountObject.h
		Settings.h
		MessageObject.h
		interfaces/hub.capnp.h
		interfaces/event.capnp.h
		rpc/AccountParser.h
		rpc/EventSubscriber.h
		rpc/RPCHubClient.h
		rpc/Utils.h
		EventObject.h
		LineSeperator.h
		)

set(SOURCES
		hub.cpp
		interfaces/hub.capnp.c++
		interfaces/event.capnp.c++
		rpc/EventSubscriber.cpp
		rpc/RPCHubClient.cpp
		rpc/Utils.cpp
		)


set(RESOURCES
    resources.qrc
)

set(LIBRARIES
    lxqt
    CapnProto::capnp-rpc
    nng::nng
)

add_definitions( -Wall -O2 -fexceptions )

BUILD_LXQT_PLUGIN(${PLUGIN})
