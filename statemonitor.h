#ifndef STATEMONITOR_H
#define STATEMONITOR_H

#include <QTcpSocket>
#include <QThread>
#include <QSqlDatabase>

class StateMonitor : public QThread
{
    Q_OBJECT

public:
    StateMonitor();

signals:
    void DB_Connect_Signal(bool _odbc_status);
    void Fixture_Connect_Signal(bool _server_status);

private slots:
    void slotConnected();
    void on_get_sql_config(QString _db_ip, QString _db_user, QString _db_pwd, QString _db_ds);
    void on_get_fixture_config(QString _serv_ip);

private:
    QString db_ip, db_user, db_pwd, db_ds, serv_ip;
    bool odbc_status, server_status;
    QSqlDatabase db;
    void check_server_status(const QString serverIP, const int port);
    void open_sql_server();
    bool is_pause = false;

protected:
    virtual void run() override;
};

#endif // WORKER_H
