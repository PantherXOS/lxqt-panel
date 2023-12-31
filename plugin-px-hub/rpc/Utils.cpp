//
// Created by fakhri on 2/17/19.
//

#include <ctime>
#include <iomanip>
#include "Utils.h"



string UTILS::FILE::filename(const string &path) {
    string fname;
    size_t lastpos = path.rfind('/');
    if (lastpos != std::string::npos) {
        fname = path.substr(lastpos + 1);
    }
    return fname;

}

bool UTILS::FILE::exists(const string &fpath) {
    struct stat buff{};
    return (stat(fpath.c_str(), &buff) == 0);
}

string UTILS::FILE::fullpath(const string &path) {
    string res;
    wordexp_t w;
    if (wordexp(path.c_str(), &w, 0) == 0) {
        res = string(*w.we_wordv);
        wordfree(&w);
    }
    return res;
}

string UTILS::timestampToString(int time) {
    std::time_t _time = time;
    std::tm* t = std::gmtime(&_time);
    std::stringstream ss;
    ss << std::put_time(t,"%H:%M");
    return ss.str();
}
