#include "statemonitor.h"
#include "supplywriter.h"

#include <QDebug>
#include <QThread>
#include <QSqlError>

extern SupplyWriter* writer;

StateMonitor::StateMonitor()
{
    db_thread = QSqlDatabase::addDatabase("QODBC", "thread");

    connect(this, SIGNAL(DB_Connect_Signal(quint8)), writer, SLOT(slotGetDBStatus(quint8)));
    connect(writer, SIGNAL(send_db_config(QString, QString, QString, QString)),
            this, SLOT(on_get_sql_config(QString, QString, QString, QString)));
}

void StateMonitor::run()
{
    while(1)
    {
        if (writer->checkIpValid(writer->checkIPversion(db_ip), db_ip) &&
            db_user.length() &&
            db_pwd.length() &&
            db_ds.length())
        {
            this->open_sql_server();
            emit DB_Connect_Signal(odbc_status);
        }
        else
        {
            emit DB_Connect_Signal(_INVALID_PARA);
        }

        this->sleep(3);
    }
}

void StateMonitor::on_get_sql_config(QString _db_ip, QString _db_user, QString _db_pwd, QString _db_ds)
{
    db_ip = _db_ip;
    db_user = _db_user;
    db_pwd = _db_pwd;
    db_ds = _db_ds;
}

void StateMonitor::open_sql_server()
{
    if (db_thread.isOpen())
        db_thread.close();

    db_thread.setHostName(db_ip);
    db_thread.setUserName(db_user);
    db_thread.setPassword(db_pwd);
    db_thread.setDatabaseName(db_ds);   //这里填写数据源名称

    db_thread.setConnectOptions("SQL_ATTR_LOGIN_TIMEOUT=1");
    if (db_thread.open(db_user, db_pwd))
    {
//        qDebug() << "open success";
        odbc_status = _SUCCESS_STATUS;
    }
    else
    {
        qDebug() << "error open database because" << db_thread.lastError().text();
        odbc_status = _FAILED_STATUS;
    }
}
