void on_checkBox2_2_clicked();
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QString>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    signals:
    void my_send_signals(bool);

    private slots:
    void on_OpenSerialButton_clicked();

    void ReadData();

    void on_btnSend_clicked();

    //检测通讯端口槽函数
    void btn_serial_check(bool);
    //打开选择端口槽函数
    void btn_open_port(bool);
    //关闭选择端口槽函数
    void btn_close_port(bool);
    //发送数据槽函数
    void btn_send_data(bool);
    //接收数据槽函数
    void receive_data(void);
    //清空接收槽函数
    void btn_clear_rev(bool);

    //自动发送复选框槽函数
    void on_checkBox_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;//全局的串口对象

    /*--------funtion---------------------*/
    //用户系统初始化
    void system_init();
    //字符串转16进制
    QByteArray QString2Hex(QString str);
    //字符转16进制
    char ConvertHexChar(char ch);
    /*--------variable--------------------*/
    //串口全局类声明
    QSerialPort global_port;
    //自动发送定时器声明
    QTimer *timer;
};

#endif // MAINWINDOW_H
