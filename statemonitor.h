#ifndef STATEMONITOR_H
#define STATEMONITOR_H

#include <QTcpSocket>
#include <QThread>
#include <QSqlDatabase>
#include <QMutex>

typedef enum {
    THREAD_IDLE,       //线程空闲
    THREAD_START,      //线程开启
    THREAD_RUNING,     //线程工作
    THREAD_PAUSE,      //线程暂停
} ThreadState;

class StateMonitor : public QThread
{
    Q_OBJECT

public:
    explicit StateMonitor(QObject *parent = nullptr);
    virtual ~StateMonitor();

    ThreadState get_thread_state();
    void thread_start(Priority pri = InheritPriority);
    void thread_stop();
    void thread_pause();
    void thread_resume();

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
    QMutex pauseMutex;
    ThreadState monitor_state = THREAD_IDLE;

protected:
    virtual void run() override;
};

#endif // WORKER_H
