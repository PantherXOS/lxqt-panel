set(PLUGIN "system")

set(HEADERS
        system.h
		Settings.h
		)

set(SOURCES
		system.cpp
		)


set(LIBRARIES
    lxqt
)

add_definitions( -Wall -O2 -fexceptions )

BUILD_LXQT_PLUGIN(${PLUGIN})
