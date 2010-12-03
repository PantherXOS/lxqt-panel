#ifndef RAZORPLUGINMANAGER_H
#define RAZORPLUGINMANAGER_H

#include "defs.h"
#include <razorqt/readsettings.h>
#include "razorplugin.h"

/**
 * @file razorpluginmanager.cpp
 * @author Christopher "VdoP" Regali
 * @brief implements Razorpluginmanager
 */


/**
 * @brief manages the plugins
 */
class RazorPluginManager
{
public:
    RazorPluginManager();
    ~RazorPluginManager();
    void addPlugin(QString _plugin, RazorBar * panel);
private:
    ReadSettings* pluginsettings;
    QList<QLibrary*> dllList;
};




#endif
