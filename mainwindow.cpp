#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "engineer.h"
#include "QThread"
#include "QStringList"
#include "messageparser.h"
#include <stdint.h>
#include <QSerialPort>
#include <QSerialPortInfo>

QString getHexLineArray(int16_t *data);



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    parseMes();
    genMes();

}

MainWindow::~MainWindow()
{
    ui->label->hide();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    genMes();
}

void MainWindow::on_pushButton_2_clicked()
{
    parseMes();
}



void MainWindow::genMes()
{
    bool ok;
    QStringList comandTextList = ui->lineEditComandOut->text().split(" ");
    uint16_t command = (comandTextList[0]+comandTextList[1]).toUInt(&ok, 16);

    QStringList dataTextList = ui->lineEditDataOut->text().split(" ");
    uint8_t data[dataTextList.length()];
    for(uint16_t i = 0; i< dataTextList.length(); i++)
    {
        data[i] = dataTextList[i].toUInt(&ok, 16);

    }
    int16_t *messageData=generateMessageOut(command, data, dataTextList.length());
    QString line = getHexLineArray(messageData);
    ui->label->setText(line);
}
void MainWindow::parseMes()
{
    bool ok;
    QStringList textList = (ui->lineEditDataIn->text()).split(" ");
    uint8_t data[textList.length()];
    for(uint16_t i = 0; i< textList.length(); i++)
    {
        data[i] = textList[i].toUInt(&ok, 16);

    }

    //uint8_t data2[] = {0x01, 0x00, 0x00 , 0x00, 0x00, 0x07, 0x12, 0x34, 0x03, 0x02 , 0x04 , 0x06 , 0x08 , 0x10 , 0x12, 0x02, 0x2b};

    uint8_t check = checkMessageIn(data);
    ui->label_3->setText("Check result "+QString::number(check));
    uint16_t commandIn = setMessageIn(data);
    ui->label_3->setText("Check result "+QString::number(check, 16)+ " Comand: " + QString::number(commandIn, 16));
    int16_t *messageData2= getDataIn();
    QString line = getHexLineArray(messageData2);
    ui->label_2->setText(line);

}
QString getHexLineArray(int16_t *data)
{
    QString line = "";
    uint16_t i =0;
    while(data[i]>=0)
    {
        QString b = QString::number(data[i], 16);
        if(b.length()==1)
        {
            line = line +" 0x0"+ QString::number(data[i], 16);
        }
        else
        {
            line = line +" 0x"+ QString::number(data[i], 16);
        }
        i++;
    }
    return line;
}

void MainWindow::on_pushButtonSendMessage_clicked()
{
    QSerialPort serial;
    serial->setPortName("/dev/ttyUSB0");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite)) {

        showStatusMessage("Connectedd");

    } else {

        showStatusMessage(tr("Open error"));
    }

    bool ok;
    QStringList comandTextList = ui->lineEditComandOut->text().split(" ");
    uint16_t command = (comandTextList[0]+comandTextList[1]).toUInt(&ok, 16);

    QStringList dataTextList = ui->lineEditDataOut->text().split(" ");
    uint8_t data[dataTextList.length()];
    for(uint16_t i = 0; i< dataTextList.length(); i++)
    {
        data[i] = dataTextList[i].toUInt(&ok, 16);

    }
    int16_t *messageData=generateMessageOut(command, data, dataTextList.length());
    QByteArray sending;
    for(int i=0; i<dataTextList.length(); i++ )
    {
        sending.append(messageData[i]);
    }
    serial->write(sending);
    QByteArray dataRead = serial->readAll();
    QString lineRX="";
    for(int i=0; i<dataRead.length(); i++ )
    {
        lineRX=lineRX+" "+dataRead.number(i);
    }

    serial->close();
     ui->label_2->setText(lineRX);


}
