#include "NewSession.h"
#include "ui_NewSession.h"

#include "SerialSetting.h"
#include "TelnetSetting.h"

NewSession::NewSession(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSession)
{
    ui->setupUi(this);

    SerialSetting *sset = new SerialSetting;

    sset->setParent(ui->fmParam);
    ui->sesType->setCurrentRow(0);

    wSetting["串口终端"] = sset;
    wSetting["telnet"] = new TelnetSetting;
}

NewSession::~NewSession()
{
    delete ui;
    wSetting.clear();
}

void NewSession::getSetting(SessionSetting &s)
{
    QString type;
    Setting *cur;

    type = ui->sesType->currentItem()->text();
    cur = wSetting[type];
    nss["type"] = type;
    nss["name"] = ui->sesName->text();
    if (cur != NULL)
    {
        cur->getSetting(nss);
    }

    s = nss;
}

void NewSession::on_sesType_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Setting *cur, *pre = NULL;

    cur = wSetting[current->text()];
    if (previous)
    {
        pre = wSetting[previous->text()];
    }

    if (pre != NULL)
    {
        pre->hide();
    }

    if (cur != NULL)
    {
        cur->setParent(ui->fmParam);
        cur->show();
    }
}
