#ifndef TOPFRAME_H
#define TOPFRAME_H

#include <QFrame>
#include "commonsrc/stripframe2.h"

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
    StripFrame2* strpf;
};

#endif // TOPFRAME_H
