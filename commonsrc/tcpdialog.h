#ifndef TCPDIALOG_H
#define TCPDIALOG_H

#include <QDialog>
#include <QUrl>

namespace Ui {
class TcpDialog;
}

class TcpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TcpDialog(QWidget *parent = nullptr);
    ~TcpDialog();

    struct PortParameters {
        QString name = QString();
        QUrl url = QUrl("127.0.0.0:502");
        int responseTime = 1000;
        int numberOfRetries = 3;
    };

    PortParameters getUp() const;

private slots:
    void apply();
private:
    Ui::TcpDialog *ui;
    PortParameters up;
    void fillPortsParameters();
    void updateParameters();
};

#endif // TCPDIALOG_H
