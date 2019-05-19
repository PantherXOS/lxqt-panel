//
// Created by Fakhri on 5/18/19.
//

#include "ShareObjects.h"

ShareObjects &ShareObjects::instance(void) {
    static ShareObjects instance;
    return instance;
}

