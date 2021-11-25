#include "mainwindow.h"
#include "ui_mainwindow.h"

//添加串口通信需要用到的两个串口头文件
#include "QtSerialPort/QSerialPort"
#include "QtSerialPort/QSerialPortInfo"

int Hexget_switch = 0;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //查找可用的串口
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->portBox->addItem(serial.portName());
            serial.close();
        }
    }
    //设置波特率下拉菜单的第0项默认值
    ui->baudBox->setCurrentIndex(0);

    ui->baudBox->addItem("1200");
    ui->baudBox->addItem("2400");
    ui->baudBox->addItem("4800");
    ui->baudBox->addItem("9600");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenSerialButton_clicked()
{
    if(ui->OpenSerialButton->text()==tr("打开串口"))
    {
     serial=new QSerialPort;
     //设置串口名
     serial->setPortName(ui->portBox->currentText());
     //打开串口
     serial->open(QIODevice::ReadWrite);
     //设置波特率
     switch(ui->dataBox->currentIndex())
     {
         case 0:
         serial->setBaudRate(QSerialPort::Baud1200);
         break;
         case 1:
         serial->setBaudRate(QSerialPort::Baud2400);
         break;
         case 2:
         serial->setBaudRate(QSerialPort::Baud4800);
         break;
         case 3:
         serial->setBaudRate(QSerialPort::Baud9600);
         break;

         case 4:
         serial->setBaudRate(QSerialPort::Baud19200);
         break;

         case 5:
         serial->setBaudRate(QSerialPort::Baud57600);
         break;
         case 6:
         serial->setBaudRate(QSerialPort::Baud115200);
         break;

         default:
         break;
     }

     //设置数据为
     switch(ui->dataBox->currentIndex())
     {
     case 0:
     serial->setDataBits(QSerialPort::Data8);
     break;
     default:
     break;
     }
     //设置校验位
     switch (ui->checkBox->currentIndex())
     {
     case 0:
     serial->setParity(QSerialPort::NoParity);
     break;
     default:
     break;
     }
     //设置停止为
     switch(ui->stopBox->currentIndex())
     {
     case 0:
     serial->setStopBits(QSerialPort::OneStop);
     break;
     case 1:
     serial->setStopBits(QSerialPort::TwoStop);
     break;
     default:
     break;
     }
     //设置流控制
     serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制

     //关闭设置菜单使能
     ui->portBox->setEnabled(false);
     ui->dataBox->setEnabled(false);
     ui->checkBox->setEnabled(false);
     ui->stopBox->setEnabled(false);
     ui->baudBox->setEnabled(false);
     ui->OpenSerialButton->setText("关闭串口");


    QObject::connect(serial,&QSerialPort::readyRead,this,&MainWindow::ReadData);
    }
    else
    {
    //关闭串口
    serial->clear();
    serial->close();
    serial->deleteLater();

    //恢复使能
    ui->portBox->setEnabled(true);
    ui->baudBox->setEnabled(true);
    ui->dataBox->setEnabled(true);
    ui->stopBox->setEnabled(true);
    ui->OpenSerialButton->setText("打开串口");
    }

}


void MainWindow::on_btnSend_clicked()
{
    serial->write(ui->txtWrite->toPlainText().toLatin1());
}

//读取接收到的消息
void MainWindow::ReadData()
{
    QByteArray buf;

    buf=serial->readAll();

    if(buf<=0)
        return;

    if(ui->HEXget_checkBox->isChecked()){
        QDataStream out (&buf,QIODevice::ReadWrite);
        while(!out.atEnd()){
           qint8 outChar = 0;
           out>>outChar;

           QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
           ui->txtRead->insertPlainText(" ");//每发送两个字符后添加一个空格
           ui->txtRead->insertPlainText(str.toUpper());//大写
           ui->txtRead->moveCursor(QTextCursor::End);
        }
        ui->txtRead->appendPlainText(tr(" "));
    }else{
        ui->txtRead->appendPlainText(tr(" ")+tr(buf));
    }
    buf.clear();
}

