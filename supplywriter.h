#ifndef SUPPLYWRITER_H
#define SUPPLYWRITER_H

#include <QDialog>
#include <QIcon>
#include <QButtonGroup>
#include <QHostAddress>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMouseEvent>
#include <QPalette>
#include <QSettings>
#include <QPaintEvent>
#include <QImage>
#include <QThread>
#include <QSystemTrayIcon>
#include <QMediaPlayer>

#include "common.h"
#include "readback.h"
#include "statemonitor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SupplyWriter;
}
QT_END_NAMESPACE

#define DATABASE_NAME   "public"
#define TABLE_NAME      "supplyinfo"

#define _SUCCESS_STATUS  false
#define _FAILED_STATUS   true

#define _AUTO_WRITE_MODE      false
#define _MANUAL_READ_MODE     true

class SupplyWriter : public QDialog
{
    Q_OBJECT

public:
    SupplyWriter(QWidget *parent = nullptr);
    ~SupplyWriter();

    void main_page_init();
    void login_page_init();
    void newuser_page_init();
    void renewpwd_page_init();
    void resetpwd_page_init();

    void clear_main_page();
    void clear_login_page();
    void clear_newuser_page();
    void clear_renewpwd_page();
    void clear_resetpwd_page0();
    void clear_resetpwd_page1();
    void set_dialog_style();
    bool checkIpValid(int version, QString ip);
    int checkIPversion(QString IP);
    void Sleep(int msec);

private slots:
    void on_AboutButton_clicked();
    void on_CloseButton_clicked();
    void on_ReadTonerInfo_clicked();
    void on_ReadDrumInfo_clicked();
    void on_DeleteSqlButton_clicked();
    void on_QuerySqlButton_clicked();
    void on_HelpButton_clicked();

    void slotConnected();
    void slotDisconnected();
    void dataReceived();

    void on_CreateUser_clicked();
    void on_ResetPassword_clicked();
    void on_ModifyPassword_clicked();
    void on_LoginButton_clicked();
    void on_ExitButton_clicked();
    void on_Return_clicked();
    void on_Confirm_clicked();

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();

    void on_checkBox_stateChanged(int state);
    void on_lineEdit_1_textChanged(const QString &arg1);
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);
    void slotUpdateWaterMark();

    void on_lineEdit_14_textChanged(const QString &arg1);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_lineEdit_4_textChanged(const QString &arg1);
    void on_lineEdit_5_textChanged(const QString &arg1);

    void slotGetDBStatus(bool _odbc_status);
    void slotGetFixtureStatus(bool _server_status);
    void on_pushButton_clicked();
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void on_radioButton_2_clicked();
    void on_radioButton_clicked();

signals:
    void sendChipInfo(struct cgprintech_supply_info_readback* info);
    void sendSqlInfo(struct cgprintech_supply_sqlinfo* info);
    void sendThemeMode(int state);
    void send_serv_config(QString _db_ip, QString _db_user, QString _db_pwd, QString _db_ds);
    void send_fixture_config(QString _serv_ip);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    bool is_drag = false;
    QPoint mouse_start_point;
    QPoint window_start_point;
    int theme_state = 0;
    StateMonitor* worker = NULL;

private:
    Ui::SupplyWriter *ui;
    QSystemTrayIcon *trayIcon = NULL;
    QString login_user;
    QString resetpwd_username;
    QSettings setting;
    QPalette palette;
    QPixmap pixmap[5];
    QIcon icon;
    int year;
    int month;
    int day;

    QMediaPlayer *player = NULL;
    bool working_mode = _AUTO_WRITE_MODE;  //默认采用自动写入模式
    bool odbc_status = _FAILED_STATUS;    //odbc数据库连接
    bool server_status = _FAILED_STATUS;  //治具连接

    QString serverIP;
    QString databaseIP;
    QString username;
    QString password;
    QString datasource;

    QTcpSocket *tcpSocket = NULL;
    QSqlDatabase db;
    QSqlQuery query;
    struct cgprintech_supply_info supply_info;

private:
    void play_mp3_sound(QString file);
    void create_start_monitor();
    bool Insert_SupplyInfo_Sql();
    void open_sql_server();
    void try_connect_db();
    QImage adjust_bright(int bright, const QImage& image);
    void adjust_bright_pixmap(int bright);
    void check_connect_fixture();
    void write_supplyinfo2chip();
    void set_style_sheet(QString filename);
    void init_market_area();
    bool check_input_valid();
    int StringToHex(char *str, unsigned char *out, unsigned int *outlen);
    void Pack32(unsigned char* dst, unsigned int val);
    void Pack16(unsigned char* dst, unsigned int val);
    unsigned int Unpack32(unsigned char* src);
    unsigned int Unpack16(unsigned char* src);
    void hex_dump(const unsigned char *src, size_t length);

    bool check_modelid_valid(QString modelid);
    bool check_serialno_valid(QString serialno);
    bool check_server_status(const QString serverIP, const int port);
    void fill_supplyinfo_data();
    bool sendData(int cmd, void* data, int data_len);
    void print_chip_info(struct cgprintech_supply_info_readback* supply_info);
};

#endif // SUPPLYWRITER_H
