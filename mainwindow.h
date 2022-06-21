#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QTcpSocket>
#include <QString>
#include <QThread>
#include <iostream>
#include <string>
#include <vector>

//Класс реализующий утилиту управления и базовый интерфейс для устройства MIGRA-3 MC типа "бегущая строка" на базе платы microSYST gmbh20.04.12 с ПО Am29F010
//В классе реализован простой пользовательский интерфейс позволяющий подключится к контроллеру с помощью TCP socket'а и предоставляющий свободное управление контроллером по средствам передачи комманд
//через поле SEND DATA. Так же добавлены базовые функции дефолтной инициализации контроллера и сброса параметров. Вторая часть интерфейса предоставляет собой набор управляющих кнопок
//имитирующих работу системы с набором необходимых событий. Т.е. кнопки собдержат в себе события системы, захардкоженные на определенные номера.
//Часть интерфейса инмитирующая работу с определенными событиями корректно работает со специальным шрифтом(шрифт будет находится в репе проекта)

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //подключения к контроллеру
    void connecttoHost();

    //простенькая проверка валидности адреса\порта. Не затрагивает проверку статуса контроллера
    bool addrPortIsValid();
private slots:
    //слот кнопки подключения к контроллеру
    void on_pushButtonCONN_clicked();

    //изменение поля редактирования адреса контроллера
    void on_lineEditIP_textChanged(const QString &arg1);

    //изменение поля редактирования порта контроллера
    void on_lineEditPORT_textChanged(const QString &arg1);
private slots:
    void cliConn();
    void cliErr();
    void cliRead();
    void on_pushButtonDICONN_clicked();
    void on_pushButtonSEND_clicked();
    void on_outDATA_textChanged();   
    void on_pushButtonINIT_clicked();
    void on_pushButtonDONT_clicked();

    //кнопки симулирующие события заправки(ЗАПРАВЛЯЕТСЯ|ЖДЕТ ОПЛАТЫ|ОПЛАЧИВАЕТ|ОПЛАЧЕНО)
    void on_pushButton_29_clicked();
    void on_pushButton_30_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_f2_clicked();
    void on_a2_clicked();
    void on_p2_clicked();
    void on_d2_clicked();
    void on_f3_clicked();
    void on_a3_clicked();
    void on_p3_clicked();
    void on_d3_clicked();
    void on_f4_clicked();
    void on_a4_clicked();
    void on_p4_clicked();
    void on_d4_clicked();
    void on_f5_clicked();
    void on_a5_clicked();
    void on_p5_clicked();
    void on_d5_clicked();
    void on_f6_clicked();
    void on_a6_clicked();
    void on_p6_clicked();
    void on_d6_clicked();
    void on_f7_clicked();
    void on_a7_clicked();
    void on_p7_clicked();
    void on_d7_clicked();
private:
    //базовый инит контроллера, с выставлением дефолтовых настроек, взят с утилиты MICON 5
    void migraInit();

    //пишем\читаем
    void writeReadBytes(const QString& data);

    int host_port_ = 0;
    quint16 block_ = 0;
    QString ip_host_ = "xxx.xxx.xxx.xxx";
    QString in_data_;
    QString out_data_;
    QTcpSocket* srv_socket_;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
