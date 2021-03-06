#include "SerialTerm.h"
#include "ui_SerialTerm.h"

#include "QTermWidget/QTermWidget.h"
#include "SendSave/SendSave.h"

#include <QSerialPort>

SerialTerm::SerialTerm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SerialTerm)
{
    ui->setupUi(this);

    initTerm();

    statusBar()->addWidget(ui->btConnect);
    statusBar()->addWidget(ui->btRecord);

    initSendSave();
    initSerial();
}

SerialTerm::~SerialTerm()
{
    delete ui;
}

void SerialTerm::initTerm()
{
    term = new QTermWidget;

    connect(term, SIGNAL(outData(QByteArray)), this, SLOT(writeData(QByteArray)));
    setCentralWidget((QWidget*)term);
}

void SerialTerm::initSendSave()
{
    dlgSS = new SendSave;

    statusBar()->addWidget(dlgSS->toolButton(0));
    statusBar()->addWidget(dlgSS->toolButton(1));
    statusBar()->addWidget(dlgSS->toolButton(2));

    connect(dlgSS, SIGNAL(outData(QByteArray)), this, SLOT(writeData(QByteArray)));
}

void SerialTerm::initSerial()
{
    serial = new QSerialPort;

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

bool SerialTerm::openSerial()
{
    bool ret;

    serial->setPortName(settings["dev"]);
    serial->setBaudRate(settings["speed"].toInt());

    ret = serial->open(QIODevice::ReadWrite);

    return ret;
}

void SerialTerm::setSettings(SessionSetting &ss)
{
    settings = ss;
}

void SerialTerm::writeData(const QByteArray &data)
{
    serial->write(data);
}

void SerialTerm::readData()
{
    QByteArray data;

    data = serial->readAll();

    term->putData(data);
}

void SerialTerm::on_btRecord_clicked()
{
    dlgSS->show();
    dlgSS->activateWindow();
}

void SerialTerm::on_btConnect_clicked()
{
    if (serial->isOpen())
    {
        serial->close();
        ui->btConnect->setText(QString("连接"));
    }
    else if (openSerial())
    {
        ui->btConnect->setText(QString("断开"));
    }
}
