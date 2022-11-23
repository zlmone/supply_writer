#include "statemonitor.h"
#include "supplywriter.h"

#include <QDebug>
#include <QThread>
#include <QSqlError>

extern SupplyWriter* writer;

StateMonitor::StateMonitor()
{
    db = QSqlDatabase::addDatabase("QODBC", "thread");

    connect(this, SIGNAL(DB_Connect_Signal(bool)), writer, SLOT(slotGetDBStatus(bool)));
    connect(this, SIGNAL(Fixture_Connect_Signal(bool)), writer, SLOT(slotGetFixtureStatus(bool)));

    connect(writer, SIGNAL(send_serv_config(QString, QString, QString, QString)),
            this, SLOT(on_get_sql_config(QString, QString, QString, QString)));

    connect(writer, SIGNAL(send_fixture_config(QString)), this, SLOT(on_get_fixture_config(QString)));
}

void StateMonitor::run()
{
    while(1)
    {
        this->sleep(3);

        if (writer->checkIpValid(writer->checkIPversion(serv_ip), serv_ip))
        {
//            qDebug() << "----serv_ip" << serv_ip;
            check_server_status(serv_ip, TCP_PORT);
            emit Fixture_Connect_Signal(server_status);
        }
        else
        {
            emit Fixture_Connect_Signal(_FAILED_STATUS);
        }

        if (writer->checkIpValid(writer->checkIPversion(db_ip), db_ip) &&
            db_user.length() &&
            db_pwd.length() &&
            db_ds.length())
        {
            open_sql_server();
            emit DB_Connect_Signal(odbc_status);
        }
        else
        {
            emit DB_Connect_Signal(_FAILED_STATUS);
        }
    }
}

void StateMonitor::on_get_sql_config(QString _db_ip, QString _db_user, QString _db_pwd, QString _db_ds)
{
    db_ip = _db_ip;
    db_user = _db_user;
    db_pwd = _db_pwd;
    db_ds = _db_ds;
}

void StateMonitor::on_get_fixture_config(QString _serv_ip)
{
//    qDebug() << _serv_ip;
    serv_ip = _serv_ip;
}

void StateMonitor::slotConnected()
{
//    qDebug() << "connected";
    server_status = _SUCCESS_STATUS;
}

void StateMonitor::check_server_status(const QString serverIP, const int port)
{
    QTcpSocket tcpSocket;
    this->server_status = _FAILED_STATUS;

    connect(&tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    tcpSocket.connectToHost(serverIP, port);
//    qDebug() << "try to connect";

    writer->Sleep(100);
    tcpSocket.close();
}

void StateMonitor::open_sql_server()
{
    if (db.isOpen())
    {
        //如果数据库句柄已打开，则先关闭
        db.close();
    }

//    qDebug() << db_ip << db_user << db_pwd << db_ds;
    if (db.isValid() == false)
    {
//        qDebug() << "------" << db_ip << db_user << db_pwd << db_ds;
        odbc_status = _FAILED_STATUS;
    }
    db.setHostName(db_ip);
    db.setDatabaseName(db_ds);   //这里填写data source name
    db.setUserName(db_user);
    db.setPassword(db_pwd);

    if (db.open())
    {
//        qDebug() << "success";
        odbc_status = _SUCCESS_STATUS;
    }
    else
    {
//        qDebug() << "failed";
        qDebug() << "error open database because" << db.lastError().text();
        odbc_status = _FAILED_STATUS;
    }
}
