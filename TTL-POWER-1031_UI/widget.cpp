#include "widget.h"
#include "ui_widget.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>

#define APP_DATA_BUFF_SIZE 9 * 1024

struct dev_info
{
    QString dev_name;
    QSerialPort serialPort;
};

int app_data_count = 0;
char* app_data = NULL;
QList<QSerialPort*> serialPorts; // 存储已连接的串口对象
QList<dev_info*> devices;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetUpgrade)
{
    ui->setupUi(this);
    this->setLayout(ui->gridLayout_2);
    QByteArray flieData;
    serialPort = new QSerialPort(this);
    on_refreshCOM_clicked();
    //ui->textEditRev->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

void close_all_serial(void)
{
    for (QSerialPort *serial : serialPorts) {
        if(serial->isOpen()){
            serial->close();
            serialPorts.removeOne(serial);
        }
    }
}

void Widget::send_file_app()
{
    serialPort->open(QIODevice::ReadWrite);
    app_data_count = 0;
    if(flieData.size() - app_data_count >= APP_DATA_BUFF_SIZE){
        serialPort->write(&app_data[app_data_count], APP_DATA_BUFF_SIZE);
        app_data_count += APP_DATA_BUFF_SIZE;
    }else{
        serialPort->write(&app_data[app_data_count], flieData.size() - app_data_count);
        app_data_count += (flieData.size() - app_data_count);
    }
}

void Widget::on_serialData_readyToRead()
{
    QString revMessagee;
    for (QSerialPort *serial : serialPorts) {
        qDebug() << "portName:" << serial->portName();
        revMessagee = serial->readAll();
        if (strstr(revMessagee.toStdString().c_str(), (char*)"info") != NULL){
            revMessagee = revMessagee.mid(4, revMessagee.size() - 4);
            ui->comboBoxUART->addItem(revMessagee);
            dev_info* dev = new dev_info();
            dev->dev_name = revMessagee;
            memcpy((void*)&dev->serialPort, (void*)serial, sizeof(QSerialPort));
            devices.append(dev);
            serial->close();
            serialPorts.removeOne(serial);
            return;
        }
    }
    revMessagee = serialPort->readAll();
    if(revMessagee.isEmpty()) {
        return;
    }
    qDebug() << "msg:" << revMessagee;
    //ui->textEditRev->append(revMessagee);
    if (revMessagee == "ok"){
        ui->textEditRev->append("已入升级模式.");
        ui->startUpgrade->setEnabled(true);
        serialPort->close();
    }else if(revMessagee == "write"){
        ui->textEditRev->append("ACK.");
    }else if(revMessagee == "done"){
        app_data_count = 0;
        ui->startUpgrade->setEnabled(true);
        ui->loadFile->setEnabled(true);
        ui->startUpgrade->setEnabled(false);
        ui->textEditRev->append("升级成功.");
    }else if(revMessagee == "error"){
        ui->textEditRev->append("升级失败.");
    }else if(revMessagee == "read"){
        ui->textEditRev->append("ACK.");
        if(flieData.size() - app_data_count >= APP_DATA_BUFF_SIZE){
            serialPort->write(&app_data[app_data_count], APP_DATA_BUFF_SIZE);
            app_data_count += APP_DATA_BUFF_SIZE;
        }else{
            serialPort->write(&app_data[app_data_count], flieData.size() - app_data_count);
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
    if(app_data == NULL)
    {
        ui->textEditRev->append("请先加载固件！！！");
        return;
    }
    for (dev_info *dev : devices) {
        if (dev->dev_name == ui->comboBoxUART->currentText()) {
            serialPort = &dev->serialPort;
            serialPort->setPortName(dev->serialPort.portName());
            serialPort->setBaudRate(QSerialPort::Baud115200);
            serialPort->setDataBits(QSerialPort::DataBits(QSerialPort::Data8));
            serialPort->setStopBits(QSerialPort::StopBits(QSerialPort::OneStop));
            serialPort->setFlowControl(QSerialPort::NoFlowControl);
            serialPort->setParity(QSerialPort::NoParity);
            if(serialPort->open(QIODevice::ReadWrite)){
                qDebug() << "serial open";
                ui->startUpgrade->setEnabled(false);
                ui->loadFile->setEnabled(false);
                ui->textEditRev->append("开始升级...");
                serialPort->write("upgrade app");
                QTimer::singleShot(1000, this, send_file_app);
            }else{
                qDebug() << "serial error";
            }
        }
    }
}


void Widget::on_loadFile_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(NULL,"",".","*.bin");
    qDebug() << file_name;
    QFile file(file_name);
    // ui->textEditRev->setText(file_name);
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
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    if (portList.isEmpty()) {
        qDebug() << "No serial ports found!";
        return;
    }
    for(dev_info* dev : devices){
        devices.removeOne(dev);
    }
    for(QSerialPortInfo serialInfo : portList){
        qDebug() << serialInfo.portName();
        QSerialPort *serial = new QSerialPort(this);

        serial->setPortName(serialInfo.portName());
        serial->setBaudRate(QSerialPort::Baud115200);
        serial->setDataBits(QSerialPort::DataBits(QSerialPort::Data8));
        serial->setStopBits(QSerialPort::StopBits(QSerialPort::OneStop));
        serial->setFlowControl(QSerialPort::NoFlowControl);
        serial->setParity(QSerialPort::NoParity);
        // 尝试打开串口
        if (serial->open(QIODevice::ReadWrite)) {
            qDebug() << "Connected to:" << serialInfo.portName();
            serialPorts.append(serial);
            connect(serial, &QSerialPort::readyRead, this, &Widget::on_serialData_readyToRead);
            serial->write("PDPsearch 1");
        } else {
            qDebug() << "Failed to open" << serialInfo.portName()
            << "Error:" << serial->errorString();
            delete serial;
        }
    }
    QTimer::singleShot(1000, this, close_all_serial);
    ui->textEditRev->append("刷新设备成功.");
}
