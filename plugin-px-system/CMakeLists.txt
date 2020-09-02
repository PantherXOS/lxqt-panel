set(PLUGIN "system")

find_package(RapidJSON)

set(HEADERS
        system.h
		Settings.h
		NetworkInformation.h
		NetworkInspection.h
		CheckUpdate.h
		)

set(SOURCES
		system.cpp
		CheckUpdate.cpp
		NetworkInspection.cpp
		PxIcon.h
		)

set(RESOURCES
		resources.qrc
		)

set(LIBRARIES
    lxqt
)

add_definitions( -Wall -O2 -fexceptions )

BUILD_LXQT_PLUGIN(${PLUGIN})
