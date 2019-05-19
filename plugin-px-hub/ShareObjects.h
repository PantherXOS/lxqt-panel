//
// Created by Fakhri on 5/18/19.
//

#ifndef LXQT_PANEL_SHAREOBJECTS_H
#define LXQT_PANEL_SHAREOBJECTS_H


#include <QtWidgets/QToolButton>

class ShareObjects {
public:
    static ShareObjects & instance(void);
    QToolButton mButton;
private:
    ShareObjects();

};


#endif //LXQT_PANEL_SHAREOBJECTS_H
