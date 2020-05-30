//
// Created by fakhri on 5/11/20.
//

#include "CheckUpdate.h"


void CheckUpdate::run() {

    if(!isRun) {
        isRun=true;
        statThread = std::thread([&]() {

          string result = exec("px-software-update-check");
           // string result = "A:v1>v2\nB:v1>v2\n";
            QString pkglist;
            std::istringstream f(result.c_str());
            std::string line;
            std::string delimiter = ":";
            while (std::getline(f, line)) {
                std::string token;
                size_t pos = 0;
                if ((pos = line.find(delimiter)) != std::string::npos) {
                    token = line.substr(0, pos);
                    pkglist+=QString::fromStdString(token)+QString::fromStdString(",");
                }
            }
            emit checkUpdateReady(pkglist);
        });
    }
}

void CheckUpdate::stop() {
    if(isRun) {
        isRun=false;
        statThread.detach();
    }
}

string CheckUpdate::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}