#include <QHostAddress>
#include <QMessageBox>
#include <QByteArray>
#include <QDataStream>
#include <QTime>

#include "mainwindow.h"
#include "ui_mainwindow.h"

static const std::vector<std::string> NOCOMMANDS {
    //----OPTIONS
    "028180811b4330303030303003",
    "028180811b4131303003",
    //----DATA
    "028180813103",
    "028180813203",
    "028180813303",
    "028180813403",
    "028180813503",
    "028180813603",
    "028180813703"
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("LINER");
    this->srv_socket_ = new QTcpSocket(this);
    this->srv_socket_->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    ui->inDATA->setReadOnly(1);
    ui->lineEditIP->setInputMask("000.000.000.000");
    ui->lineEditPORT->setInputMask("00000");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::connecttoHost() {
    this->srv_socket_->connectToHost(QHostAddress(this->ip_host_), this->host_port_);
    connect(this->srv_socket_, SIGNAL(connected()), SLOT(cliConn()));
    ui->inDATA->append("Socket state: " + QString::number(this->srv_socket_->state()));
}
void MainWindow::cliConn() {
    ui->inDATA->append("Client connected to display!");
}

void MainWindow::cliErr() {

}

void MainWindow::cliRead() {
    QByteArray datas = this->srv_socket_->readAll();
    std::cout << datas.toStdString()  << std::endl;
    ui->inDATA->append("Read from socket: " + QString(datas));
}

void MainWindow::on_pushButtonCONN_clicked() {
    if(this->addrPortIsValid()) {
        this->connecttoHost();
    } else {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","An error has occured !");
        messageBox.setFixedSize(500,200);
    }   
}

bool MainWindow::addrPortIsValid() {
    QHostAddress chkIP;
    if(chkIP.setAddress(this->ip_host_) && this->host_port_ > 0) {
        return true;
    }
    this->ip_host_.clear();
    this->host_port_ = 0;
    return false;
}

void MainWindow::on_lineEditIP_textChanged(const QString &arg1) {
    this->ip_host_ = arg1;
}

void MainWindow::on_lineEditPORT_textChanged(const QString &arg1) {
    try {
        this->host_port_ = std::stoi(arg1.toStdString());
    } catch (...) {
        this->host_port_ = 0;
    }
}

void MainWindow::on_pushButtonDICONN_clicked() {
    this->srv_socket_->disconnectFromHost();
    ui->inDATA->append("Socket state: " + QString::number(this->srv_socket_->state()));
}

void MainWindow::on_pushButtonSEND_clicked() {
    this->writeReadBytes(this->out_data_);
}

void MainWindow::writeReadBytes(const QString& data) {
    QByteArray arrBlock = QByteArray::fromHex(data.toLatin1());
    this->srv_socket_->write(arrBlock);
    this->srv_socket_->waitForBytesWritten();
    this->srv_socket_->waitForReadyRead();
    this->cliRead();
}

void MainWindow::migraInit() {
    ui->inDATA->append("DISPLAY START INIT");
    QString cmd = "028180811b626f6f7403";
    this->writeReadBytes(cmd);

    cmd = "2305818004020c24";
    this->writeReadBytes(cmd);

    cmd = "2305818003060f24";
    this->writeReadBytes(cmd);

    cmd = "2305818004030d24";
    this->writeReadBytes(cmd);

    cmd = "028180811b4130303003";   //ЦВЕТА курсора 028180811b41 33 303003, фона 028180811b4133 30, мерцание 30 03
    this->writeReadBytes(cmd);

    cmd = "028180811b5a303303";     //ТИП ШРИФТА 028180811b5a30 30(размер шрифта) 03
    this->writeReadBytes(cmd);

    cmd = "028180811b4503";
    this->writeReadBytes(cmd);

    cmd = "028180811b4c3003";
    this->writeReadBytes(cmd);

    cmd = "028180811b533030313030313503";
    this->writeReadBytes(cmd);

    cmd = "028180811b463003";
    this->writeReadBytes(cmd);

    cmd = "028180811b4330303030303003";     //СМЕЩЕНИЕ X и Y - 028180811b433030 30(X) 3030 30(Y) 03
    this->writeReadBytes(cmd);

    cmd = "028180811b4503";
    this->writeReadBytes(cmd);

    cmd = "028180811b533030313030363303";
    this->writeReadBytes(cmd);

    cmd = "028180811b4c3003";
    this->writeReadBytes(cmd);
}

void MainWindow::on_outDATA_textChanged() {
    this->out_data_.clear();
    this->out_data_ = ui->outDATA->toPlainText();
}

void MainWindow::on_pushButtonINIT_clicked() {
    this->migraInit();
}

void MainWindow::on_pushButtonDONT_clicked() {
    for(const auto& comm : NOCOMMANDS) {
        this->writeReadBytes(QString::fromStdString(comm));
    }
}

void MainWindow::on_pushButton_29_clicked() {
    this->ui->inDATA->clear();
    this->in_data_.clear();
}

void MainWindow::on_pushButton_30_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b463003"));               //СБРОС
    this->writeReadBytes(QString::fromStdString("028180811b4330303030303003"));     //СМЕЩЕНИЕ X и Y 0|0
    this->writeReadBytes(QString::fromStdString("028180811b5a303303"));             //выбор шрифта
    this->writeReadBytes(QString::fromStdString("028180811b4131303003"));           //выбор цвета зеленый
}
//----------------------1 колонка
void MainWindow::on_pushButton_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330303030303003"));     //СМЕЩЕНИЕ X и Y 0|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303003"));           //выбор цвета зеленый
    this->writeReadBytes(QString::fromStdString("028180813103"));                   //номер колонки
}

void MainWindow::on_pushButton_2_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330303030303003"));     //СМЕЩЕНИЕ X и Y 0|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303003"));           //выбор цвета красный
    this->writeReadBytes(QString::fromStdString("028180813103"));                   //номер колонки
}

void MainWindow::on_pushButton_3_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330303030303003"));     //СМЕЩЕНИЕ X и Y 0|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303103"));           //выбор цвета красный мерцающий
    this->writeReadBytes(QString::fromStdString("028180813103"));                   //номер колонки
}

void MainWindow::on_pushButton_4_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330303030303003"));     //СМЕЩЕНИЕ X и Y 0|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303103"));           //выбор цвета ЗЕЛЕНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813103"));                   //номер колонки
    QThread::msleep(200);
    this->writeReadBytes(QString::fromStdString("028180811b4330303030303003"));     //СМЕЩЕНИЕ X и Y 0|0
    this->writeReadBytes(QString::fromStdString("028180812003"));                   //ТУХНЕМ ПРОБЕЛОМ %)
}
//----------------------2 колонка
void MainWindow::on_f2_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330303930303003"));     //СМЕЩЕНИЕ X и Y 9|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303003"));           //выбор цвета зеленый
    this->writeReadBytes(QString::fromStdString("028180813203"));                   //номер колонки
}

void MainWindow::on_a2_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330303930303003"));     //СМЕЩЕНИЕ X и Y 9|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303003"));           //выбор цвета КРАСНЫЙ
    this->writeReadBytes(QString::fromStdString("028180813203"));                   //номер колонки
}

void MainWindow::on_p2_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330303930303003"));     //СМЕЩЕНИЕ X и Y 9|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303103"));           //выбор цвета КРАСНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813203"));                   //номер колонки
}

void MainWindow::on_d2_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330303930303003"));     //СМЕЩЕНИЕ X и Y 9|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303103"));           //выбор цвета ЗЕЛЕНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813203"));                   //номер колонки
    QThread::msleep(200);
    this->writeReadBytes(QString::fromStdString("028180811b4330303930303003"));     //СМЕЩЕНИЕ X и Y 9|0
    this->writeReadBytes(QString::fromStdString("028180812003"));                   //ТУХНЕМ ПРОБЕЛОМ %)
}
//----------------------3 колонка
void MainWindow::on_f3_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330313830303003"));     //СМЕЩЕНИЕ X и Y 18|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303003"));           //выбор цвета зеленый
    this->writeReadBytes(QString::fromStdString("028180813303"));                   //номер колонки
}

void MainWindow::on_a3_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330313830303003"));     //СМЕЩЕНИЕ X и Y 18|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303003"));           //выбор цвета КРАСНЫЙ
    this->writeReadBytes(QString::fromStdString("028180813303"));                   //номер колонки
}

void MainWindow::on_p3_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330313830303003"));     //СМЕЩЕНИЕ X и Y 18|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303103"));           //выбор цвета КРАСНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813303"));                   //номер колонки
}

void MainWindow::on_d3_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330313830303003"));     //СМЕЩЕНИЕ X и Y 18|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303103"));           //выбор цвета ЗЕЛЕНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813303"));                   //номер колонки
    QThread::msleep(200);
    this->writeReadBytes(QString::fromStdString("028180811b4330313830303003"));     //СМЕЩЕНИЕ X и Y 18|0
    this->writeReadBytes(QString::fromStdString("028180812003"));                   //ТУХНЕМ ПРОБЕЛОМ %)
}
//----------------------4 колонка
void MainWindow::on_f4_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330323730303003"));     //СМЕЩЕНИЕ X и Y 27|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303003"));           //выбор цвета зеленый
    this->writeReadBytes(QString::fromStdString("028180813403"));                   //номер колонки
}

void MainWindow::on_a4_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330323730303003"));     //СМЕЩЕНИЕ X и Y 27|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303003"));           //выбор цвета КРАСНЫЙ
    this->writeReadBytes(QString::fromStdString("028180813403"));                   //номер колонки
}

void MainWindow::on_p4_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330323730303003"));     //СМЕЩЕНИЕ X и Y 27|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303103"));           //выбор цвета КРАСНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813403"));                   //номер колонки
}

void MainWindow::on_d4_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330323730303003"));     //СМЕЩЕНИЕ X и Y 27|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303103"));           //выбор цвета ЗЕЛЕНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813403"));                   //номер колонки
    QThread::msleep(200);
    this->writeReadBytes(QString::fromStdString("028180811b4330323730303003"));     //СМЕЩЕНИЕ X и Y 27|0
    this->writeReadBytes(QString::fromStdString("028180812003"));                   //ТУХНЕМ ПРОБЕЛОМ %)
}
//----------------------5 колонка
void MainWindow::on_f5_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330333630303003"));     //СМЕЩЕНИЕ X и Y 36|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303003"));           //выбор цвета зеленый
    this->writeReadBytes(QString::fromStdString("028180813503"));                   //номер колонки
}

void MainWindow::on_a5_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330333630303003"));     //СМЕЩЕНИЕ X и Y 36|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303003"));           //выбор цвета КРАСНЫЙ
    this->writeReadBytes(QString::fromStdString("028180813503"));                   //номер колонки
}

void MainWindow::on_p5_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330333630303003"));     //СМЕЩЕНИЕ X и Y 36|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303103"));           //выбор цвета КРАСНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813503"));                   //номер колонки
}

void MainWindow::on_d5_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330333630303003"));     //СМЕЩЕНИЕ X и Y 36|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303103"));           //выбор цвета ЗЕЛЕНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813503"));                   //номер колонки
    QThread::msleep(200);
    this->writeReadBytes(QString::fromStdString("028180811b4330333630303003"));     //СМЕЩЕНИЕ X и Y 36|0
    this->writeReadBytes(QString::fromStdString("028180812003"));                   //ТУХНЕМ ПРОБЕЛОМ %)
}
//----------------------6 колонка
void MainWindow::on_f6_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330343530303003"));     //СМЕЩЕНИЕ X и Y 45|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303003"));           //выбор цвета зеленый
    this->writeReadBytes(QString::fromStdString("028180813603"));                   //номер колонки
}

void MainWindow::on_a6_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330343530303003"));     //СМЕЩЕНИЕ X и Y 45|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303003"));           //выбор цвета КРАСНЫЙ
    this->writeReadBytes(QString::fromStdString("028180813603"));                   //номер колонки
}

void MainWindow::on_p6_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330343530303003"));     //СМЕЩЕНИЕ X и Y 45|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303103"));           //выбор цвета КРАСНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813603"));                   //номер колонки
}

void MainWindow::on_d6_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330343530303003"));     //СМЕЩЕНИЕ X и Y 45|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303103"));           //выбор цвета ЗЕЛЕНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813603"));                   //номер колонки
    QThread::msleep(200);
    this->writeReadBytes(QString::fromStdString("028180811b4330343530303003"));     //СМЕЩЕНИЕ X и Y 45|0
    this->writeReadBytes(QString::fromStdString("028180812003"));                   //ТУХНЕМ ПРОБЕЛОМ %)
}
//----------------------7 колонка
void MainWindow::on_f7_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330353430303003"));     //СМЕЩЕНИЕ X и Y 54|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303003"));           //выбор цвета зеленый
    this->writeReadBytes(QString::fromStdString("028180813703"));                   //номер колонки
}

void MainWindow::on_a7_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330353430303003"));     //СМЕЩЕНИЕ X и Y 54|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303003"));           //выбор цвета КРАСНЫЙ
    this->writeReadBytes(QString::fromStdString("028180813703"));                   //номер колонки
}

void MainWindow::on_p7_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330353430303003"));     //СМЕЩЕНИЕ X и Y 54|0
    this->writeReadBytes(QString::fromStdString("028180811b4132303103"));           //выбор цвета КРАСНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813703"));                   //номер колонки
}

void MainWindow::on_d7_clicked() {
    this->writeReadBytes(QString::fromStdString("028180811b4330353430303003"));     //СМЕЩЕНИЕ X и Y 54|0
    this->writeReadBytes(QString::fromStdString("028180811b4131303103"));           //выбор цвета ЗЕЛЕНЫЙ мерцающий
    this->writeReadBytes(QString::fromStdString("028180813703"));                   //номер колонки
    QThread::msleep(200);
    this->writeReadBytes(QString::fromStdString("028180811b4330353430303003"));     //СМЕЩЕНИЕ X и Y 54|0
    this->writeReadBytes(QString::fromStdString("028180812003"));                   //ТУХНЕМ ПРОБЕЛОМ %)
}
