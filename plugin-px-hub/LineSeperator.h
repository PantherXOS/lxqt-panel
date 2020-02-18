//
// Created by fakhri on 2/8/20.
//

#ifndef LXQT_PANEL_LINESEPERATOR_H
#define LXQT_PANEL_LINESEPERATOR_H
#include <QFrame>

using namespace std;
class LineSeperator : public QFrame{
Q_OBJECT
public:
    LineSeperator(QWidget *parent = nullptr) : QFrame(parent){
        setGeometry(QRect(320, 150, 118, 11));
        setFrameShape(QFrame::VLine);
        setFrameShadow(QFrame::Plain);
        setContentsMargins(5,5,5,5);

        QPalette p = palette();
        p.setColor(QPalette::WindowText, Qt::red);
        setPalette(p);
    }
};

#endif //LXQT_PANEL_LINESEPERATOR_H
