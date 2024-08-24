#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>

#define APP_DATA_BUFF_SIZE 9 * 1024
int app_data_count = 0;
char *app_data = NULL;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetUpgrade)
{
    ui->setupUi(this);
    serialStatus = false;
    QByteArray flieData;
    setialPort = new QSerialPort(this);
    connect(setialPort, &QSerialPort::readyRead, this, &Widget::on_serialData_readyToRead);
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo serialInfo : serialList){
        qDebug() << serialInfo.portName();
        ui->comboBoxUART->addItem(serialInfo.portName());
    }
    //ui->textEditRev->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonUART_clicked()
{
    if(!serialStatus){
        setialPort->setPortName(ui->comboBoxUART->currentText());
        setialPort->setBaudRate(QSerialPort::Baud115200);
        setialPort->setDataBits(QSerialPort::DataBits(QSerialPort::Data8));
        setialPort->setStopBits(QSerialPort::StopBits(QSerialPort::OneStop));
        setialPort->setFlowControl(QSerialPort::NoFlowControl);
        setialPort->setParity(QSerialPort::NoParity);
        if(setialPort->open(QIODevice::ReadWrite)){
            qDebug() << "serial open";
            serialStatus = true;
            ui->comboBoxUART->setEnabled(false);
            ui->enterUpgrade->setEnabled(true);
            ui->refreshCOM->setEnabled(false);
            ui->quitUpgrade->setEnabled(true);
            ui->pushButtonUART->setText("关闭串口");
        }else{
            qDebug() << "serial error";
            QMessageBox msgBox;
            msgBox.setWindowTitle("打开串口错误");
            msgBox.setText("打开失败，串口可能被占用！");
            msgBox.exec();
        }
    }else{
        setialPort->close();
        ui->comboBoxUART->setEnabled(true);
        ui->enterUpgrade->setEnabled(false);
        ui->startUpgrade->setEnabled(false);
        ui->refreshCOM->setEnabled(true);
        ui->quitUpgrade->setEnabled(false);
        serialStatus = false;
        ui->pushButtonUART->setText("打开串口");
    }
}


void Widget::on_enterUpgrade_clicked()
{
    setialPort->close();
    setialPort->setPortName(ui->comboBoxUART->currentText());
    setialPort->setBaudRate(QSerialPort::Baud115200);
    setialPort->setDataBits(QSerialPort::DataBits(QSerialPort::Data8));
    setialPort->setStopBits(QSerialPort::StopBits(QSerialPort::OneStop));
    setialPort->setFlowControl(QSerialPort::NoFlowControl);
    setialPort->setParity(QSerialPort::NoParity);
    if(setialPort->open(QIODevice::ReadWrite)){
        setialPort->write("upgrade app");
    }else{
        setialPort->write("upgrade app");
    }
}

void Widget::on_serialData_readyToRead()
{
    QString revMessagee = setialPort->readAll();
    qDebug() << "msg:" << revMessagee;
    //ui->textEditRev->append(revMessagee);
    if (revMessagee == "ok"){
        ui->textEditRev->append("已入升级模式.");
        ui->startUpgrade->setEnabled(true);
        ui->enterUpgrade->setEnabled(false);
        setialPort->close();
    }else if(revMessagee == "write"){
        ui->textEditRev->append("ACK.");
    }else if(revMessagee == "done"){
        app_data_count = 0;
        ui->enterUpgrade->setEnabled(true);
        ui->startUpgrade->setEnabled(true);
        ui->pushButtonUART->setEnabled(true);
        ui->loadFile->setEnabled(true);
        ui->startUpgrade->setEnabled(false);
        ui->textEditRev->append("升级成功.");
    }else if(revMessagee == "error"){
        ui->textEditRev->append("升级失败.");
    }else if(revMessagee == "read"){
        ui->textEditRev->append("ACK.");
        if(flieData.size() - app_data_count >= APP_DATA_BUFF_SIZE){
            setialPort->write(&app_data[app_data_count], APP_DATA_BUFF_SIZE);
            app_data_count += APP_DATA_BUFF_SIZE;
        }else{
            setialPort->write(&app_data[app_data_count], flieData.size() - app_data_count);
            app_data_count += (flieData.size() - app_data_count);
        }
    }
}


void Widget::on_clearMessage_clicked()
{
    ui->textEditRev->clear();
}


void Widget::on_startUpgrade_clicked()
{
    // app_data_count = 0;
    // if(flieData.size() - app_data_count >= APP_DATA_BUFF_SIZE){
    //     setialPort->write(&app_data[app_data_count], APP_DATA_BUFF_SIZE);
    //     app_data_count += APP_DATA_BUFF_SIZE;
    // }else{
    //     setialPort->write(&app_data[app_data_count], flieData.size() - app_data_count);
    //     app_data_count += (flieData.size() - app_data_count);
    // }
    if(app_data == NULL)
    {
        ui->textEditRev->append("请先加载固件！！！");
        return;
    }
    setialPort->setPortName(ui->comboBoxUART->currentText());
    setialPort->setBaudRate(QSerialPort::Baud115200);
    setialPort->setDataBits(QSerialPort::DataBits(QSerialPort::Data8));
    setialPort->setStopBits(QSerialPort::StopBits(QSerialPort::OneStop));
    setialPort->setFlowControl(QSerialPort::NoFlowControl);
    setialPort->setParity(QSerialPort::NoParity);
    if(setialPort->open(QIODevice::ReadWrite)){
        qDebug() << "serial open";
        ui->startUpgrade->setEnabled(false);
        ui->pushButtonUART->setEnabled(false);
        ui->loadFile->setEnabled(false);
        ui->textEditRev->append("开始升级...");
        app_data_count = 0;
        if(flieData.size() - app_data_count >= APP_DATA_BUFF_SIZE){
            setialPort->write(&app_data[app_data_count], APP_DATA_BUFF_SIZE);
            app_data_count += APP_DATA_BUFF_SIZE;
        }else{
            setialPort->write(&app_data[app_data_count], flieData.size() - app_data_count);
            app_data_count += (flieData.size() - app_data_count);
        }
    }else{
        qDebug() << "serial error";
    }
}


void Widget::on_loadFile_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(NULL,"",".","*.bin");
    qDebug() << file_name;
    QFile file(file_name);
    ui->textEditRev->setText(file_name);
    if (file.open(QFile::ReadOnly  | QFile::Truncate))
    {
        flieData = file.readAll();
        app_data = flieData.data();
        ui->textEditRev->append("固件加载成功!!!");
        file.close();
    }else{
        ui->textEditRev->append("固件加载失败!!!");
    }
}

void Widget::on_refreshCOM_clicked()
{
    ui->comboBoxUART->clear();
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo serialInfo : serialList){
        qDebug() << serialInfo.portName();
        ui->comboBoxUART->addItem(serialInfo.portName());
    }
}


void Widget::on_quitUpgrade_clicked()
{
    ui->enterUpgrade->setEnabled(true);
    setialPort->close();
    setialPort->setPortName(ui->comboBoxUART->currentText());
    setialPort->setBaudRate(QSerialPort::Baud115200);
    setialPort->setDataBits(QSerialPort::DataBits(QSerialPort::Data8));
    setialPort->setStopBits(QSerialPort::StopBits(QSerialPort::OneStop));
    setialPort->setFlowControl(QSerialPort::NoFlowControl);
    setialPort->setParity(QSerialPort::NoParity);
    if(setialPort->open(QIODevice::ReadWrite)){
        setialPort->write("upgrade quit");
    }else{
        setialPort->write("upgrade quit");
    }
    ui->textEditRev->append("已发送退出升级模式指令.");
}

