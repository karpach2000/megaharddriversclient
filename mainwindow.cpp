#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "engineer.h"
#include "QThread"
#include "messageparser.h"
#include <stdint.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    ui->label->hide();
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Engineer engineer;
    ui->label->setText(engineer.tazi(ui->textEdit->toPlainText()));
    int16_t data[] = {0x02, 0x04, 0x06, 0x08, 0x10, 0x12, -1};
    uint16_t command = 0x1234;
    int16_t *messageData=generateMessageOut(command, data);
    QString line;
    int i=0;
    while(messageData[i]>=0)
    {
        line = line +" "+ QString::number(messageData[i], 16);
        i++;
    }
    ui->label->setText(line);


    uint8_t data2[] = {0x01, 0x00, 0x00 , 0x00, 0x00, 0x07, 0x12, 0x34, 0x03, 0x02 , 0x04 , 0x06 , 0x08 , 0x10 , 0x12, 0x02, 0x01};

    uint8_t check = checkMessageIn(data2);
    ui->label_3->setText("Check result "+QString::number(check));
    uint16_t commandIn = setMessageIn(data2);
    ui->label_3->setText("Check result "+QString::number(check, 16)+ " Comand: " + QString::number(commandIn, 16)+"  "+QString::number(data2[data2[5]+10-2]));
    int16_t *messageData2= getDataIn();

    i=0;
    line = "";
    while(messageData2[i]>=0)
    {
        line = line +" "+ QString::number(messageData2[i], 16);
        i++;
    }
    ui->label_2->setText(line);
}
