#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QVariant>
#include <QDockWidget>

#define VERSION    "1.0.0"

#include "NewSession/NewSession.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dockWidget->setWidget(ui->twProject);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about(void)
{
    QMessageBox mbox;

    mbox.about(this, tr("关于"), tr("版本号: " VERSION));
}

void MainWindow::on_new_s_triggered()
{
    NewSession *ns = new NewSession;

    ns->show();
    ns->exec();

    if (ns->result() == 1)
    {
        SessionSetting set;

        ns->getSetting(set);

        addSession(set);
    }

    delete ns;
}

void MainWindow::addSession(SessionSetting &set)
{
    QTreeWidgetItem *child;

    child = addSessionProject(set);

    addSessionWindow(set, child);

    int cnt = dwlist.count();
    if (cnt > 1)
    {
        QDockWidget *f, *s;

        f = dwlist.at(cnt - 2);
        s = dwlist.at(cnt - 1);
        tabifyDockWidget(f, s);
    }
}

QTreeWidgetItem* MainWindow::addSessionProject(SessionSetting &set)
{
    QTreeWidgetItem *type = new QTreeWidgetItem;
    bool addtype = true;

    for (int i = 0; i < ui->twProject->topLevelItemCount(); i ++)
    {
        QTreeWidgetItem *tmptype = ui->twProject->topLevelItem(i);

        if (tmptype->text(0) == set["type"])
        {
            delete type;
            type = tmptype;
            addtype = false;
            break;
        }
    }

    type->setText(0, set["type"]);
    if (addtype)
    {
        QVariant var(0);

        type->setData(0, Qt::UserRole, var);
        ui->twProject->addTopLevelItem(type);
    }

    QTreeWidgetItem *child = new QTreeWidgetItem;
    child->setText(0, set["name"]);
    type->addChild(child);

    return child;
}

#include "Serial/SerialTerm.h"
#include "Telnet/TelnetTerm.h"

void MainWindow::addSessionWindow(SessionSetting &set, QTreeWidgetItem *item)
{
    QDockWidget *dock;
    QVariant var;
    bool add = false;

    dock= new QDockWidget(set["name"], this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);

    var.setValue(dock);

    if (set["type"] == "串口终端")
    {
        item->setData(0, Qt::UserRole, var);

        SerialTerm *term = new SerialTerm;
        term->setSettings(set);

        dock->setWidget(term);
        add = true;
    }

    if (set["type"] == "telnet")
    {
        item->setData(0, Qt::UserRole, var);

        TelnetTerm *term = new TelnetTerm;
        term->setSettings(set);

        dock->setWidget(term);
        add = true;
    }

    if (add)
    {
        dwlist.append(dock);
        addDockWidget(Qt::RightDockWidgetArea, dock);
    }
    else
    {
        delete dock;
    }
}

void MainWindow::on_twProject_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QDockWidget *dock;
    QVariant var;

    var = item->data(column, Qt::UserRole);
    if (var == 0)
        return;

    dock = var.value<QDockWidget*>();

    dock->show();
    dock->activateWindow();
}

void MainWindow::on_twProject_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem* curItem;

    curItem = ui->twProject->itemAt(pos);
    if (curItem == NULL)
        return;

    QVariant var = curItem->data(0, Qt::UserRole);
    if (var == 0)
        return;

    QMenu *popMenu =new QMenu(this);

    popMenu->addAction(ui->del_s);//往菜单内添加QAction   该action在前面用设计器定义了
    popMenu->exec(QCursor::pos());

    delete popMenu;
}
