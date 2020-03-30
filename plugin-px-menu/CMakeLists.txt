set(PLUGIN "menu")

set(HEADERS
        menu.h
		Settings.h
		)

set(SOURCES
		menu.cpp
		)

set(RESOURCES
		resources.qrc
		)

set(LIBRARIES
    lxqt
)

add_definitions( -Wall -O2 -fexceptions )

BUILD_LXQT_PLUGIN(${PLUGIN})
