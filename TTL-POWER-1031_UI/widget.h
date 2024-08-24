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
    void on_pushButtonUART_clicked();

    void on_enterUpgrade_clicked();

    void on_serialData_readyToRead();

    void on_clearMessage_clicked();

    void on_startUpgrade_clicked();

    void on_loadFile_clicked();

    void on_loadFile_2_clicked();

    void on_refreshUART_clicked();

    void on_refreshCOM_clicked();

private:
    Ui::WidgetUpgrade *ui;
    QSerialPort *setialPort;
    bool serialStatus;
    QByteArray flieData;
};
#endif // WIDGET_H
