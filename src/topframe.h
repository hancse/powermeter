#ifndef TOPFRAME_H
#define TOPFRAME_H

#include <QFrame>
#include "commonsrc/stripframetriple.h"

namespace Ui {
class TopFrame;
}

class TopFrame : public QFrame
{
    Q_OBJECT

public:
    explicit TopFrame(QWidget *parent = nullptr);
    ~TopFrame();

private:
    Ui::TopFrame *ui;
    StripFrameTriple* strpf;
};

#endif // TOPFRAME_H
