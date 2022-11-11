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

#include "common.h"
#include "readback.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SupplyWriter;
}
QT_END_NAMESPACE

#define DATABASE_NAME   "public"
#define TABLE_NAME      "supplyinfo"

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

private slots:
    void on_ConcurrentButton_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_1_clicked();
    void on_AboutButton_clicked();
    void on_InsertSqlButton_clicked();
    void on_pushButton_5_clicked();
    void on_CloseButton_clicked();
    void on_ReadTonerInfo_clicked();
    void on_ReadDrumInfo_clicked();
    void on_DeleteSqlButton_clicked();
    void on_QuerySqlButton_clicked();
    void on_HelpButton_clicked();

    void slotConnected();
    void slotDisconnected();
    void dataReceived();

    void on_total_page_radio_clicked();
    void on_total_dot_radio_clicked();
    void on_beyond_printpage_radio_clicked();
    void on_beyond_printdot_radio_clicked();

    void on_CreateUser_clicked();
    void on_ResetPassword_clicked();
    void on_ModifyPassword_clicked();
    void on_LoginButton_clicked();
    void on_ExitButton_clicked();
    void on_ExitLogin_clicked();
    void on_Return_clicked();
    void on_Confirm_clicked();

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_checkBox_stateChanged(int state);
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);

signals:
    void sendChipInfo(struct cgprintech_supply_info_readback* info);
    void sendSqlInfo(struct cgprintech_supply_sqlinfo* info);
    void sendThemeMode(int state);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QButtonGroup* BtnGroup[2];
    bool is_drag = false;
    QPoint mouse_start_point;
    QPoint window_start_point;
    int theme_state = 0;

private:
    Ui::SupplyWriter *ui;

    QString login_user;
    QString resetpwd_username;
    QSettings setting;

    QPalette palette;
    QPixmap pixmap;
    QIcon icon;

    bool status;
    int year;
    int month;
    int day;
    bool server_status;

    QString serverIP;
    int serverIPversion;
    QString databaseIP;
    int databaseIPversion;

    QString username;
    QString password;
    QString datasource;

    QTcpSocket *tcpSocket;
    QSqlDatabase db;
    QSqlQuery query;
    struct cgprintech_supply_info supply_info;

private:
    void set_style_sheet(QString filename);
    void init_market_area();
    bool check_input_valid();
    int StringToHex(char *str, unsigned char *out, unsigned int *outlen);
    void Pack32(unsigned char* dst, unsigned int val);
    void Pack16(unsigned char* dst, unsigned int val);
    unsigned int Unpack32(unsigned char* src);
    unsigned int Unpack16(unsigned char* src);
    void hex_dump(const unsigned char *src, size_t length);
    void Sleep(int msec);

    bool check_modelid_valid(QString modelid);
    bool check_serialno_valid(QString serialno);
    bool checkIpValid(int version, QString ip);
    int checkIPversion(QString IP);
    bool check_server_status(const QString serverIP, const int port);

    void fill_supplyinfo_data();
    bool sendData(QString serverIP, int port, int cmd, void* data, int data_len);

    void print_chip_info(struct cgprintech_supply_info_readback* supply_info);
};

#endif // SUPPLYWRITER_H
