#ifndef TOPFRAME_H
#define TOPFRAME_H

#include <QFrame>
//#include "commonsrc/stripframetriple.h"
#include "deifregisters.h"

namespace Ui {
class TopFrame;
}

class TopFrame : public QFrame
{
    Q_OBJECT

public:
    explicit TopFrame(QWidget *parent = nullptr);
    ~TopFrame();

    void displayGridData(UniversalAEParams ae);
    void displayPVData(UniversalAEParams ae);
    void displayBatteryData(UniversalAEParams ae);
    void displayLoadData(UniversalAEParams ae);

private slots:

private:
    Ui::TopFrame *ui;
    //StripFrameTriple* strpf;

};

#endif // TOPFRAME_H
