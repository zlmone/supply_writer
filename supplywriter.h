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
#include <QListWidgetItem>
#include <QSettings>
#include <QPaintEvent>
#include <QImage>
#include <QThread>
#include <QUdpSocket>
#include <QMediaPlayer>
#include <QTimer>

#include "common.h"
#include "readback.h"
#include "statemonitor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SupplyWriter; }
QT_END_NAMESPACE

//监听的广播端口
#define BC_UDP_PORT    8890
#define HB_UDP_PORT    8891

#define DATABASE_NAME   "cgprintech"
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
    void clear_main_page();

    void set_dialog_style();
    bool checkIpValid(int version, QString ip);
    int checkIPversion(QString IP);
    void update_timestamp();
    void Sleep(int msec);

private slots:
    void on_ReadTonerInfo_clicked();
    void on_ReadDrumInfo_clicked();
    void on_DeleteSqlButton_clicked();
    void on_QuerySqlButton_clicked();

    void slotConnected();
    void dataReceived();
    void statusReceived();

    void on_lineEdit_1_textChanged(const QString &arg1);
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);
//    void slotUpdateWaterMark();
    void on_lineEdit_14_textChanged(const QString &arg1);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_lineEdit_4_textChanged(const QString &arg1);
    void on_lineEdit_5_textChanged(const QString &arg1);

    void scan_the_fixtures();
    void check_fixture_online();
    void udp_data_recv();
    void udp_hb_recv();
    void slotGetDBStatus(quint8 _odbc_status);
    void update_connect_fixture();
    void on_RecommendFixtures_itemDoubleClicked(QListWidgetItem *item);
    void on_lineEdit_22_textChanged(const QString &arg1);

signals:
    void sendChipInfo(struct cgprintech_supply_info_readback* info);
    void sendSqlInfo(struct cgprintech_supply_sqlinfo* info);
    void sendThemeMode(int state);
    void send_db_config(QString _db_ip, QString _db_user, QString _db_pwd, QString _db_ds);

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
    int year, month, day;

private:
    Ui::SupplyWriter *ui;
    StateMonitor* worker = NULL;
    QTimer* timer[3] = {NULL};
    bool udp_hb_status = false;
    QString login_user;
    QString resetpwd_username;
    QSettings setting;
    QPalette palette;
    QPixmap pixmap[5];
    QMediaPlayer *player = NULL;
    bool working_mode = _AUTO_WRITE_MODE;  //默认采用自动写入模式
    quint8 odbc_status = _INVALID_PARA;    //odbc数据库连接
    quint8 server_status = _INVALID_PARA;  //治具连接
    QUdpSocket* udpSocket[2] = {NULL};
    QTcpSocket *tcpSocket[2] = {NULL};
    QSqlDatabase db;
    QSqlQuery query;
    struct cgprintech_supply_info supply_info;

private:
    void open_sql_server();
    void Update_FixtureStatus();
    void play_mp3_sound(QString file);
    bool Insert_SupplyInfo_Sql();
    void try_connect_db();
    QImage adjust_bright(int bright, const QImage& image);
    void adjust_bright_pixmap(int bright);
    void write_supplyinfo2chip();
    void set_style_sheet(QString filename);
    bool check_input_valid();

    int StringToHex(char *str, unsigned char *out, unsigned int *outlen);
    void Pack32(unsigned char* dst, unsigned int val);
    void Pack16(unsigned char* dst, unsigned int val);
    unsigned int Unpack32(unsigned char* src);
    unsigned int Unpack16(unsigned char* src);
    void hex_dump(const unsigned char *src, size_t length);

    void send_udp_hb_pack(QString ipaddr);
    bool check_modelid_valid(QString modelid);
    bool check_serialno_valid(QString serialno);
    bool check_server_status();
    bool fill_supplyinfo_data();
    bool sendData(int cmd, void* data, int data_len);
    void print_chip_info(struct cgprintech_supply_info_readback* supply_info);
};

#endif // SUPPLYWRITER_H
