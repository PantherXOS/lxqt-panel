//
// Created by fakhri on 2/17/19.
//

#ifndef PX_HUB_SERVICE_UTILS_H
#define PX_HUB_SERVICE_UTILS_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <wordexp.h>
#include <dirent.h>
#include <stdarg.h>

using namespace std;

namespace UTILS {

    namespace FILE {

        string filename(const string &path);

        bool exists(const string &path);

        string fullpath(const string &path);
    }
}



#endif //PX_HUB_SERVICE_UTILS_H
