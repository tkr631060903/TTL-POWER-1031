#ifndef WIDGET_H
#define WIDGET_H

#include <QSerialPort>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class WidgetUpgrade;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_serialData_readyToRead();

    void on_clearMessage_clicked();

    void on_startUpgrade_clicked();

    void on_loadFile_clicked();

    void on_refreshCOM_clicked();

private:
    Ui::WidgetUpgrade *ui;
    QSerialPort *serialPort;
    bool serialStatus;
    QByteArray flieData;

    void send_file_app();
};
#endif // WIDGET_H
