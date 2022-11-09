#include <QDebug>
#include <QMessageBox>
#include <QHostInfo>
#include <QProcess>
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QRegExpValidator>
#include <QCryptographicHash>

#include "sqlchipinfo.h"
#include "supplywriter.h"
#include "helpdialog.h"

#include "ui_supplywriter.h"

void SupplyWriter::init_market_area()
{
    int kk;

    for (kk = 0; kk < 6; kk++)
        ui->comboBox->addItem(Market_Area[kk]);

    for (kk = 0; kk < 2; kk++)
    {
        ui->comboBox_2->addItem(_type[kk]);
        ui->comboBox_3->addItem(_trademark[kk]);
        ui->comboBox_4->addItem(_trademark[kk]);
    }
}

void SupplyWriter::clear_login_page()
{
    ui->username->clear();
    ui->password->clear();
    ui->label_21->clear();

    ui->username->setFocus();
    ui->LoginButton->setDefault(true);
}

void SupplyWriter::clear_main_page()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();
    ui->lineEdit_11->clear();
    ui->lineEdit_12->clear();
    ui->lineEdit_13->clear();
    ui->lineEdit_14->clear();
    ui->lineEdit_1->clear();

    ui->total_page_radio->setChecked(1);
    ui->lineEdit_6->setEnabled(1);
    ui->lineEdit_7->setEnabled(false);

    ui->beyond_printpage_radio->setChecked(1);
    ui->lineEdit_11->setEnabled(1);
    ui->lineEdit_12->setEnabled(false);

    ui->label_2->clear();
    ui->label_13->clear();
    ui->label_44->setText(ui->username->text());

    ui->lineEdit_2->setFocus();
}

void SupplyWriter::clear_newuser_page()
{
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->lineEdit_10->clear();

    ui->label_26->clear();

    ui->lineEdit_8->setFocus();
    ui->Confirm->setEnabled(true);
    ui->Return->setEnabled(true);
    ui->Confirm->setDefault(true);
}

void SupplyWriter::clear_renewpwd_page()
{
    ui->lineEdit_15->clear();
    ui->lineEdit_16->clear();
    ui->lineEdit_17->clear();
    ui->lineEdit_18->clear();

    ui->label_31->clear();

    ui->lineEdit_15->setFocus();
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_3->setDefault(true);
}

void SupplyWriter::clear_resetpwd_page0()
{
    ui->lineEdit_19->clear();

    ui->label_41->clear();

    ui->dateEdit_2->setDate(QDate::currentDate());
    ui->dateEdit_3->setDate(QDate::currentDate());

    ui->lineEdit_19->setFocus();
    ui->pushButton_8->setDefault(true);
}

void SupplyWriter::clear_resetpwd_page1()
{
    ui->lineEdit_20->clear();
    ui->lineEdit_21->clear();

    ui->label_40->clear();

    ui->lineEdit_20->setFocus();
    ui->pushButton_9->setEnabled(true);
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_10->setDefault(true);
}

//登录页面初始化
void SupplyWriter::login_page_init()
{
    icon.addFile(QString::fromUtf8(":/images/cgprint.png"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    setFixedSize(this->width(), this->height());
    this->setWindowFlags(Qt::FramelessWindowHint);
//    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    pixmap.load(":/images/title.png");
    pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->title_label->setScaledContents(false);
    ui->title_label->setPixmap(pixmap);

    pixmap.load(":/images/logo.png");
    pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->logo_label->setScaledContents(false);
    ui->logo_label->setPixmap(pixmap);
    ui->label_25->setPixmap(pixmap);

    QFont font1("Microsoft YaHei", 13, 80);
    ui->label_18->setFont(font1);
    QFont font2("Microsoft YaHei", 15, 70);
    ui->label_17->setFont(font2);
    QFont font3("Microsoft YaHei", 11, 65);
    ui->label_20->setFont(font3);

    ui->password->setEchoMode(QLineEdit::Password);
    ui->LoginButton->setDefault(true);
}

//耗材信息页面初始化
void SupplyWriter::main_page_init()
{
    this->status = false;
    tcpSocket = new QTcpSocket(this);

    QRegExp rx("^[1-9][0-9]{1,}$");
    validator = new QRegExpValidator(rx, this);
    ui->lineEdit_6->setValidator(validator);
    ui->lineEdit_7->setValidator(validator);
    ui->lineEdit_11->setValidator(validator);
    ui->lineEdit_12->setValidator(validator);
    ui->lineEdit_13->setValidator(validator);

    BtnGroup[0] = new QButtonGroup;
    BtnGroup[1] = new QButtonGroup;

    BtnGroup[0]->addButton(ui->total_page_radio, 0);
    BtnGroup[0]->addButton(ui->total_dot_radio, 1);
    BtnGroup[0]->setExclusive(true);
    ui->total_page_radio->setChecked(true);
    ui->lineEdit_7->setEnabled(false);

    BtnGroup[1]->addButton(ui->beyond_printpage_radio, 0);
    BtnGroup[1]->addButton(ui->beyond_printdot_radio, 1);
    BtnGroup[1]->setExclusive(true);
    ui->beyond_printpage_radio->setChecked(true);
    ui->lineEdit_12->setEnabled(false);

    ui->lineEdit_2->setFocus();
    this->init_market_area();

    ui->dateEdit->setDate(QDate::currentDate());

    this->setTabOrder(ui->lineEdit_2, ui->lineEdit_3);
    this->setTabOrder(ui->lineEdit_3, ui->lineEdit_6);
    this->setTabOrder(ui->lineEdit_6, ui->lineEdit_7);
    this->setTabOrder(ui->lineEdit_7, ui->lineEdit_11);
    this->setTabOrder(ui->lineEdit_11, ui->lineEdit_12);
    this->setTabOrder(ui->lineEdit_12, ui->lineEdit_13);
    this->setTabOrder(ui->lineEdit_13, ui->lineEdit_14);
    this->setTabOrder(ui->lineEdit_14, ui->lineEdit);
    this->setTabOrder(ui->lineEdit, ui->lineEdit_4);
    this->setTabOrder(ui->lineEdit_4, ui->lineEdit_5);
    this->setTabOrder(ui->lineEdit_5, ui->lineEdit_1);

    ui->lineEdit_4->setEchoMode(QLineEdit::Password);
    db = QSqlDatabase::addDatabase("QODBC");
}

//创建用户页面初始化
void SupplyWriter::newuser_page_init()
{
    pixmap.load(":/images/title2.png");
    pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_24->setPixmap(pixmap);

    QFont font1("Microsoft YaHei", 15, 70);
    ui->label_22->setFont(font1);
    QFont font2("Microsoft YaHei", 11, 60);
    ui->label_23->setFont(font2);

    ui->lineEdit_9->setEchoMode(QLineEdit::Password);
    ui->lineEdit_10->setEchoMode(QLineEdit::Password);
}

//修改密码页面初始化
void SupplyWriter::renewpwd_page_init()
{
    pixmap.load(":/images/title4.png");
    pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_27->setPixmap(pixmap);

    pixmap.load(":/images/logo.png");
    ui->label_28->setPixmap(pixmap);

    QFont font1("Microsoft YaHei", 15, 70);
    ui->label_30->setFont(font1);
    ui->label_39->setFont(font1);
    QFont font2("Microsoft YaHei", 11, 60);
    ui->label_29->setFont(font2);
    ui->label_38->setFont(font2);

    ui->lineEdit_16->setEchoMode(QLineEdit::Password);
    ui->lineEdit_17->setEchoMode(QLineEdit::Password);
    ui->lineEdit_18->setEchoMode(QLineEdit::Password);
}

//重置密码页面初始化
void SupplyWriter::resetpwd_page_init()
{
    pixmap.load(":/images/title3.png");
    pixmap.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->label_32->setPixmap(pixmap);

    pixmap.load(":/images/logo.png");
    ui->label_33->setPixmap(pixmap);
    ui->label_43->setPixmap(pixmap);

    QFont font1("Microsoft YaHei", 15, 70);
    ui->label_35->setFont(font1);
    ui->label_39->setFont(font1);
    QFont font2("Microsoft YaHei", 11, 60);
    ui->label_34->setFont(font2);
    ui->label_38->setFont(font2);

    ui->lineEdit_20->setEchoMode(QLineEdit::Password);
    ui->lineEdit_21->setEchoMode(QLineEdit::Password);
}

void SupplyWriter::set_style_sheet(QString filename)
{
    QString qssfile = QString(":/qss/%1").arg(filename);
    QFile skinfile(qssfile);
    skinfile.open(QIODevice::ReadOnly);
    this->setStyleSheet(skinfile.readAll());
    skinfile.close();
}

SupplyWriter::SupplyWriter(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SupplyWriter)
{
    ui->setupUi(this);

    set_style_sheet("light.qss");

    this->login_page_init();
    this->main_page_init();
    this->newuser_page_init();
    this->renewpwd_page_init();
    this->resetpwd_page_init();
}

// NOT USED
void SupplyWriter::set_dialog_style()
{
    palette.setColor(QPalette::Window, QColor(218, 176, 154));
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, QColor(216, 191, 216));
    palette.setColor(QPalette::AlternateBase, Qt::green);
    palette.setColor(QPalette::ToolTipBase, Qt::darkGray);
    palette.setColor(QPalette::ToolTipText, Qt::black);
    palette.setColor(QPalette::Text, Qt::black);

    palette.setColor(QPalette::Button, Qt::white);
    palette.setColor(QPalette::ButtonText, Qt::black);

    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, Qt::black);
    palette.setColor(QPalette::HighlightedText, Qt::white);
    this->setPalette(palette);
}

SupplyWriter::~SupplyWriter()
{
    delete ui;
    delete BtnGroup[0];
    delete BtnGroup[1];
    delete validator;
    delete tcpSocket;
}

void SupplyWriter::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        is_drag = true;
        //获得鼠标的初始位置
        mouse_start_point = event->globalPos();
        //获得窗口的初始位置
        window_start_point = this->frameGeometry().topLeft();
    }
}

void SupplyWriter::mouseMoveEvent(QMouseEvent *event)
{
    //判断是否在拖拽移动
    if (is_drag)
    {
        //获得鼠标移动的距离
        QPoint move_distance = event->globalPos() - mouse_start_point;
        //改变窗口的位置
        this->move(window_start_point + move_distance);
    }
}

void SupplyWriter::mouseReleaseEvent(QMouseEvent *event)
{
    //放下左键即停止移动
    if (event->button() == Qt::LeftButton)
    {
        is_drag = false;
    }
}

int SupplyWriter::StringToHex(char *str, unsigned char *out, unsigned int *outlen)
{
    char *p = str;
    char high = 0, low = 0;
    int tmplen = strlen(p), cnt = 0;
    tmplen = strlen(p);

    while (cnt < (tmplen / 2))
    {
        high =  ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
        low = (*(++p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p) - 48 - 7 : *(p) - 48;
        out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
        p++;
        cnt++;
    }

    if (tmplen % 2 != 0)
        out[cnt] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;

    if (outlen != NULL)
        *outlen = tmplen / 2 + tmplen % 2;

    return tmplen / 2 + tmplen % 2;
}

void SupplyWriter::Pack16(unsigned char *dst, unsigned int val)
{
    dst[0] = (unsigned char)((val >> 8) & 0xff);
    dst[1] = (unsigned char)(val & 0xff);
}

void SupplyWriter::Pack32(unsigned char *dst, unsigned int val)
{
    dst[0] = (unsigned char)((val >> 24) & 0xff);
    dst[1] = (unsigned char)((val >> 16) & 0xff);
    dst[2] = (unsigned char)((val >> 8) & 0xff);
    dst[3] = (unsigned char)(val & 0xff);
}

unsigned int SupplyWriter::Unpack16(unsigned char *src)
{
    return (((unsigned int)src[0]) << 8
          | ((unsigned int)src[1]));
}

unsigned int SupplyWriter::Unpack32(unsigned char *src)
{
    return(((unsigned int)src[0]) << 24
         | ((unsigned int)src[1]) << 16
         | ((unsigned int)src[2]) << 8
         | (unsigned int)src[3]);
}

void SupplyWriter::hex_dump(const unsigned char *src, size_t length)
{
    int i = 0;
    const unsigned char *address = src;
    unsigned int num=0;
    size_t line_size=16;

    printf("%08x | ", num);
    num += 16;
    while (length-- > 0)
    {
        printf("%02x ", *address++);

        if ((i+1)%8==0 && (i+1)%16==8)
        {
            printf("  ");
        }

        if (!(++i % line_size) || (length == 0 && i % line_size))
        {
            printf("\n");

            if (length > 0)
            {
                printf("%08x | ", num);
                num += 16;
            }
        }
    }
    printf("\n");
}

//检测输入信息是否合法
bool SupplyWriter::check_input_valid()
{
    if (ui->lineEdit_2->text().length() == 0 ||
        ui->lineEdit_3->text().length() == 0 ||
        ui->lineEdit_13->text().length() == 0)
        return 0;

    if (ui->total_page_radio->isChecked())
    {
        if (ui->lineEdit_6->text().length() == 0)
            return 0;
    }
    else
    {
        if (ui->lineEdit_7->text().length() == 0)
            return 0;
    }

    if (ui->beyond_printpage_radio->isChecked())
    {
        if (ui->lineEdit_11->text().length() == 0)
            return 0;
    }
    else
    {
        if (ui->lineEdit_12->text().length() == 0)
            return 0;
    }

    if (check_modelid_valid(ui->lineEdit_2->text()) == 0)
        return false;

    if (check_serialno_valid(ui->lineEdit_3->text()) == 0)
        return false;

    return true;
}

void SupplyWriter::Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while(QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void SupplyWriter::slotConnected()
{
//    qDebug() << "connected";
    this->server_status = true;
}

void SupplyWriter::slotDisconnected()
{
//    qDebug() << "Disconnected";
}

void SupplyWriter::print_chip_info(struct cgprintech_supply_info_readback *supply_info)
{
    printf("model_id\t%s\n", supply_info->model_id);
    printf("serial_no\t%s\n", supply_info->serial_no);
    printf("marketing_area\t%s\n", supply_info->marketing_area);
    printf("manufacturer\t%s\n", supply_info->manufacturer);
    printf("trade_mark\t%s\n", supply_info->trade_mark);
    printf("type\t%s\n", supply_info->type);
    printf("pages\t%d\n", Unpack32(supply_info->pages));
    printf("dots\t%d\n", Unpack32(supply_info->dots));
    printf("product_date\t%02x%02x%02x%02x\n", supply_info->product_date[0],
        supply_info->product_date[1],
        supply_info->product_date[2],
        supply_info->product_date[3]);
    printf("beyond_pages\t%d\n", Unpack16(supply_info->beyond_pages));
    printf("beyond_percent\t%d\n", Unpack16(supply_info->beyond_percent));
    printf("free_pages\t%d\n", Unpack16(supply_info->free_pages));

    printf("install_date\t%02x%02x%02x%02x\n", supply_info->install_date[0],
        supply_info->install_date[1],
        supply_info->install_date[2],
        supply_info->install_date[3]);
    printf("recent_date\t%02x%02x%02x%02x\n", supply_info->recent_date[0],
        supply_info->recent_date[1],
        supply_info->recent_date[2],
        supply_info->recent_date[3]);
    printf("printed beyond pages\t%d\n", Unpack32(supply_info->print_beyond_pages));
    printf("printed_pages\t%d\n", Unpack32(supply_info->printed_pages));
    printf("printed_dots\t%d\n", Unpack16(supply_info->printed_dots));
}

void SupplyWriter::dataReceived()
{
    unsigned char resp[sizeof(MsgHdr) + SUPPLY_INFO_READBACK_LEN];
    qint64 length;

    while (tcpSocket->bytesAvailable() > 0)
    {
        QByteArray datagram;
        length = tcpSocket->bytesAvailable();
//        qDebug() << "bytesAvailable" << length;

        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(), datagram.size());
        memcpy(&resp, datagram.data(), length);
//        qDebug() << ((MsgHdr*)resp)->cmd << " " << ((MsgHdr*)resp)->len << " " << ((MsgHdr*)resp)->ret;
//        qDebug() << "cmd=" << ((RespInfo*)resp)->cmd << " ret=" << ((RespInfo*)resp)->ret;

        if (((RespInfo*)resp)->ret == RESP_OK)
        {
            if (((RespInfo*)resp)->cmd == OP_WRITE_INFO)
            {
                ui->label_2->setText("<font color=green>写入成功！</font>");
            }
            else if (((RespInfo*)resp)->cmd == OP_READ_INFO)
            {
                ui->label_2->setText("<font color=green>读取成功！</font>");
                //展示读取到的信息
//                this->print_chip_info((struct cgprintech_supply_info_readback*)(resp + sizeof(RespInfo)));

                ReadBack *readback = new ReadBack();

                connect(this, SIGNAL(sendThemeMode(int)), readback, SLOT(get_theme_id(int)));
                connect(this, SIGNAL(sendChipInfo(struct cgprintech_supply_info_readback*)),
                        readback, SLOT(show_ChipInfo(struct cgprintech_supply_info_readback*)));

                emit sendChipInfo((struct cgprintech_supply_info_readback*)(resp + sizeof(RespInfo)));
                emit sendThemeMode(this->theme_state);

                readback->show();
            }
            this->status = false;
        }
        else
        {
            if (((RespInfo*)resp)->cmd == OP_WRITE_INFO)
            {
                ui->label_2->setText("<font color=red>写入失败！</font>");
            }
            else if (((RespInfo*)resp)->cmd == OP_READ_INFO)
            {
                ui->label_2->setText("<font color=red>读取失败！</font>");
            }
            this->status = true;
        }

        tcpSocket->close();
        break;
    }
}

bool SupplyWriter::sendData(QString serverIP, int port, int cmd, void* data, int data_len)
{
    uint8_t writeinfo[sizeof(MsgHdr) + data_len];

    connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

    tcpSocket->connectToHost(serverIP, port);

    ((MsgHdr*)writeinfo)->cmd = cmd;
    ((MsgHdr*)writeinfo)->len = data_len;
    if (data)
    {
        if (strncasecmp(((struct cgprintech_supply_info*)data)->model_id, "TL", 2) == 0)
            ((MsgHdr*)writeinfo)->i2c_addr = 0x2c;
        else if (strncasecmp(((struct cgprintech_supply_info*)data)->model_id, "DL", 2) == 0)
            ((MsgHdr*)writeinfo)->i2c_addr = 0x28;

        memcpy(writeinfo + sizeof(MsgHdr), data, data_len);
    }
    else
    {
        if (strncasecmp(ui->lineEdit_2->text().toLocal8Bit().data(), "TL", 2) == 0)
            ((MsgHdr*)writeinfo)->i2c_addr = 0x2c;
        else if (strncasecmp(ui->lineEdit_2->text().toLocal8Bit().data(), "DL", 2) == 0)
            ((MsgHdr*)writeinfo)->i2c_addr = 0x28;
    }

    tcpSocket->write((const char*)&writeinfo, sizeof(writeinfo));
//    this->hex_dump((const uint8_t*)data, data_len);
//    qDebug() << sizeof(WriteInfo);
//    this->hex_dump((unsigned char*)&writeinfo, sizeof(WriteInfo));

    return true;
}

//写入耗材信息，同时写入数据库
void SupplyWriter::on_ConcurrentButton_clicked()
{
    //检查下位机状态
    this->on_pushButton_1_clicked();
    if (status)
    {
//        qDebug() << "下位机检查失败！";
        return;
    }

    //检查数据库状态
    this->on_pushButton_4_clicked();
    if (status)
    {
//        qDebug() << "数据库检查失败！";
        return;
    }

    //写入耗材
    this->on_pushButton_5_clicked();
    if (status)
    {
//        qDebug() << "写入耗材失败！";
        return;
    }

    //写入数据库
    this->on_InsertSqlButton_clicked();
    if (status)
    {
//        qDebug() << "写入数据库失败！";
        return;
    }
}

//数据库连接测试
void SupplyWriter::on_pushButton_4_clicked()
{
    databaseIP = ui->lineEdit_14->text();
    username = ui->lineEdit->text();
    password = ui->lineEdit_4->text();
    datasource = ui->lineEdit_5->text();

    this->databaseIPversion = checkIPversion(databaseIP);
    if (databaseIPversion == 0)
    {
        ui->label_13->setText("<font color=red>数据库 IP 地址错误！</font>");
        status = true;
        return;
    }

    if (!checkIpValid(databaseIPversion, databaseIP))
    {
//        QMessageBox::warning(this, tr("错误"), tr("数据库 IP 地址错误！"), QMessageBox::Ok);
        ui->label_13->setText("<font color=red>数据库 IP 地址错误！</font>");
        status = true;
        return;
    }

    if (db.isOpen())
    {
        //如果数据库句柄已打开，则先关闭
        db.close();
    }

    if (db.isValid() == false)
    {
        ui->label_13->setText("<font color=red>无效的数据库句柄！</font>");
        status = true;
        return;
    }
    db.setHostName(databaseIP);
    db.setDatabaseName(datasource);   //这里填写data source name
    db.setUserName(username);
    db.setPassword(password);

    if (db.open()) {
        ui->label_13->setText("<font color=green>数据库连接成功！</font>");
        status = false;
        return;
    }
    else {
        ui->label_13->setText("<font color=red>数据库连接失败！</font>");
//        qDebug()<<"error open database because"<<db.lastError().text();
        status = true;
        return;
    }
}

//下位机连接测试
void SupplyWriter::on_pushButton_1_clicked()
{
    serverIP = ui->lineEdit_1->text();

    serverIPversion = this->checkIPversion(serverIP);
    if (serverIPversion == 0)
    {
        ui->label_2->setText("<font color=red>下位机 IP 地址错误！</font>");
        status = true;
        return;
    }

    if (!checkIpValid(this->serverIPversion, serverIP))
    {
//        QMessageBox::warning(this, tr("错误"), tr("下位机 IP 地址错误！"), QMessageBox::Ok);
        ui->label_2->setText("<font color=red>下位机 IP 地址错误！</font>");
        status = true;
        return;
    }

    if (check_server_status(serverIP, TCP_PORT) == false)
    {
//        QMessageBox::warning(this, tr("错误"), tr("下位机设备离线！"), QMessageBox::Ok);
        ui->label_2->setText("<font color=red>下位机设备离线！</font>");
        status = true;
        return;
    }
    else
    {
//        qDebug() << serverIP << "在线" << endl;
        ui->label_2->setText("<font color=green>下位机连接正常！</font>");
    }
    status = false;
}

//写入数据库
void SupplyWriter::on_InsertSqlButton_clicked()
{
    bool ret = false;

    this->on_pushButton_4_clicked();
    if (status)
    {
        return;
    }

    this->fill_supplyinfo_data();
    if (status)
    {
        ui->label_13->setText("<font color=red>耗材信息不完整！</font>");
        return;
    }

    query = QSqlQuery(this->db);
    QString new_sql = QString("insert into %1.%2 (operator,model_id,serial_no,marketing_area,year,month,day,"
                              "manufacturer,trade_mark,type,pages,dots,overflow_pages,overflow_percent,free_pages)"
                              "values(:operator,:model_id,:serial_no,:marketing_area,:year,:month,:day,"
                              ":manufacturer,:trade_mark,:type,:pages,:dots,:overflow_pages,"
                              ":overflow_percent,:free_pages)").arg(DATABASE_NAME).arg(TABLE_NAME);

    query.prepare(new_sql);
    query.bindValue(":operator", login_user.toLocal8Bit().data());
    query.bindValue(":model_id", supply_info.model_id);
    query.bindValue(":serial_no", supply_info.serial_no);
    query.bindValue(":marketing_area", supply_info.marketing_area);
    query.bindValue(":year", this->year);
    query.bindValue(":month", this->month);
    query.bindValue(":day", this->day);
    query.bindValue(":manufacturer", supply_info.manufacturer);
    query.bindValue(":trade_mark", supply_info.trade_mark);
    query.bindValue(":type", supply_info.type);
    query.bindValue(":pages", Unpack32(supply_info.pages));
    query.bindValue(":dots", Unpack32(supply_info.dots));
    query.bindValue(":overflow_pages", Unpack16(supply_info.beyond_pages));
    query.bindValue(":overflow_percent", Unpack16(supply_info.beyond_percent));
    query.bindValue(":free_pages", Unpack16(supply_info.free_pages));
    ret = query.exec();
    if (ret)
    {
        ui->label_13->setText("<font color=green>写入数据正常！</font>");
        this->status = true;
    }
    else
    {
        ui->label_13->setText("<font color=red>写入数据失败！</font>");
        qDebug() << query.lastError().driverText() << QString(QObject::tr("插入失败"));
        this->status = false;
    }
}

//根据序列号查询数据库中相应记录信息
void SupplyWriter::on_QuerySqlButton_clicked()
{
    int num = 0;

    this->on_pushButton_4_clicked();
    if (status)
    {
        return;
    }

    struct cgprintech_supply_sqlinfo ChipInfo;
    QString sqlcmd = QString("select model_id,serial_no,marketing_area,year,month,day,manufacturer,trade_mark,"
                             "type,pages,dots,overflow_pages,overflow_percent,free_pages,operator from %1.%2 "
                             "where serial_no='%3'").arg(DATABASE_NAME).arg(TABLE_NAME).arg(ui->lineEdit_3->text());
//    qDebug() << sqlcmd;
    query = QSqlQuery(this->db);
    if (!query.exec(sqlcmd))
    {
        qDebug() << query.lastError().driverText() << QString(QObject::tr("读取失败"));
        return;
    }
    else
    {
        while (query.next())
        {
            num++;

            memcpy(ChipInfo.model_id, query.value(0).toString().toLatin1().data(), 16);
            memcpy(ChipInfo.serial_no, query.value(1).toString().toLatin1().data(), 32);
            memcpy(ChipInfo.marketing_area, query.value(2).toString().toLatin1().data(), 4);
            QString date = query.value(3).toString() + "." + query.value(4).toString() + "." + query.value(5).toString();
            memcpy(ChipInfo.product_date, date.toLatin1().data(), 12);
            memcpy(ChipInfo.manufacturer, query.value(6).toString().toLatin1().data(), 16);
            memcpy(ChipInfo.trade_mark, query.value(7).toString().toLatin1().data(), 16);
            memcpy(ChipInfo.type, query.value(8).toString().toLatin1().data(), 4);
            memcpy(ChipInfo.pages, query.value(9).toString().toLatin1().data(), 12);
            memcpy(ChipInfo.dots, query.value(10).toString().toLatin1().data(), 12);
            memcpy(ChipInfo.overflow_pages, query.value(11).toString().toLatin1().data(), 4);
            memcpy(ChipInfo.overflow_percent, query.value(12).toString().toLatin1().data(), 4);
            memcpy(ChipInfo.free_pages, query.value(13).toString().toLatin1().data(), 4);
            ChipInfo.operator_id = query.value(14).toString();
//            memcpy(ChipInfo.operator_id, query.value(14).toString().toLatin1().data(), 16);
//            qDebug() << query.value(14).toString();
        }
    }

    if (num == 0)
    {
        ui->label_13->setText("<font color=red>查询数据失败！</font>");
        return;
    }
    ui->label_13->setText("<font color=green>查询数据成功！</font>");

    SqlChipInfo *sqlinfo = new SqlChipInfo();

    connect(this, SIGNAL(sendThemeMode(int)), sqlinfo, SLOT(get_theme_id(int)));
    connect(this, SIGNAL(sendSqlInfo(struct cgprintech_supply_sqlinfo*)),
            sqlinfo, SLOT(recvSqlInfo(struct cgprintech_supply_sqlinfo*)));

    emit sendThemeMode(theme_state);
    emit sendSqlInfo(&ChipInfo);

    sqlinfo->show();
}

//写入耗材
void SupplyWriter::on_pushButton_5_clicked()
{
    this->fill_supplyinfo_data();
    if (this->status)
    {
        ui->label_2->setText("<font color=red>耗材信息不正确！</font>");
        return;
    }

    this->on_pushButton_1_clicked();
    if (this->status)
        return;

    this->sendData(serverIP, TCP_PORT, OP_WRITE_INFO, &supply_info, CGPRINTECH_SUPPLY_INFO_LEN);
}

void SupplyWriter::fill_supplyinfo_data()
{
    unsigned int len;
    char date[8] = {0};

    memset(&supply_info, 0, CGPRINTECH_SUPPLY_INFO_LEN);

    if (check_input_valid() == 0)
    {
        this->status = true;
        return;
    }

    memcpy(&supply_info.model_id, ui->lineEdit_2->text().toLatin1().data(), ui->lineEdit_2->text().length());
    memcpy(&supply_info.serial_no, ui->lineEdit_3->text().toLatin1().data(), ui->lineEdit_3->text().length());
//    memcpy(&supply_info.marketing_area, ui->comboBox->currentText().toLatin1().data(), ui->comboBox->currentText().length());
    memcpy(&supply_info.manufacturer, ui->comboBox_3->currentText().toLatin1().data(), ui->comboBox_3->currentText().length());
    memcpy(&supply_info.trade_mark, ui->comboBox_4->currentText().toLatin1().data(), ui->comboBox_4->currentText().length());
    memcpy(&supply_info.marketing_area, marketing_area[ui->comboBox->currentIndex()].toLatin1().data(), 2);
    memcpy(&supply_info.type, supply_type[ui->comboBox_2->currentIndex()].toLatin1().data(), 1);

    if (ui->total_page_radio->isChecked())
    {
        Pack32(supply_info.pages, ui->lineEdit_6->text().toUInt());
    }
    else
    {
        Pack32(supply_info.dots, ui->lineEdit_7->text().toUInt());
    }

    if (ui->beyond_printpage_radio->isChecked())
    {
        Pack16(supply_info.beyond_pages, ui->lineEdit_11->text().toUInt());
    }
    else
    {
        Pack16(supply_info.beyond_percent, ui->lineEdit_12->text().toUInt());
    }
    Pack16(supply_info.free_pages, ui->lineEdit_13->text().toUInt());

    year = ui->dateEdit->date().year();
    month = ui->dateEdit->date().month();
    day = ui->dateEdit->date().day();
    sprintf(date, "%04d%02d%02d", year, month, day);
    this->StringToHex(date, supply_info.product_date, &len);

//    this->hex_dump((unsigned char*)&supply_info, sizeof(struct cgprintech_supply_info));
    this->status = false;
}

//耗材信息回读
void SupplyWriter::on_pushButton_7_clicked()
{
    this->on_pushButton_1_clicked();
    if (this->status)
        return;

    if ((strncmp(ui->lineEdit_2->text().toLatin1().data(), "TL", 2) != 0) &&
        (strncmp(ui->lineEdit_2->text().toLatin1().data(), "DL", 2) != 0))
    {
        ui->label_2->setText("<font color=red>请输入耗材型号以便读取！</font>");
        return;
    }

    //1.发送读取耗材信息命令
    this->sendData(serverIP, TCP_PORT, OP_READ_INFO, NULL, 0);

    return;
}

bool SupplyWriter::check_modelid_valid(QString modelid)
{
    QRegExp rx3("^[TD]L-34[01][UHL]?$");
    if (!rx3.exactMatch(modelid))
    {
        return false;
    }
    return true;
}

bool SupplyWriter::check_serialno_valid(QString serialno)
{
    QRegExp rx4("^CGL?[0-9]{10}CGRX[ABCDFGHJKLMNPQRSTWXYZ][1-9A-C][0-9]{5}$");
    if (!rx4.exactMatch(serialno))
    {
        return false;
    }
    return true;
}

//检查IP地址是否合法
bool SupplyWriter::checkIpValid(int version, QString ip)
{
    if (version == 4)
    {
        QRegExp rx2("^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\."
                "(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\."
                "(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\."
                "(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$");
        if(!rx2.exactMatch(ip))
        {
            //ip地址非法
            return false;
        }
    }
    else if (version == 6)
    {
        QRegExp rx2("^([\\da-fA-F]{1,4}:){6}((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}"
                    "(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$|^::([\\da-fA-F]{1,4}:){0,4}"
                    "((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}"
                    "(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$|^([\\da-fA-F]{1,4}:):([\\da-fA-F]{1,4}:){0,3}"
                    "((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}"
                    "(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$|^([\\da-fA-F]{1,4}:){2}:([\\da-fA-F]{1,4}:){0,2}"
                    "((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}"
                    "(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$|^([\\da-fA-F]{1,4}:){3}:([\\da-fA-F]{1,4}:){0,1}"
                    "((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}"
                    "(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$|^([\\da-fA-F]{1,4}:){4}:"
                    "((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}"
                    "(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$|^([\\da-fA-F]{1,4}:){7}[\\da-fA-F]{1,4}$|^:((:[\\da-fA-F]{1,4}){1,6}|:)$|^[\\da-fA-F]{1,4}:"
                    "((:[\\da-fA-F]{1,4}){1,5}|:)$|^([\\da-fA-F]{1,4}:){2}"
                    "((:[\\da-fA-F]{1,4}){1,4}|:)$|^([\\da-fA-F]{1,4}:){3}"
                    "((:[\\da-fA-F]{1,4}){1,3}|:)$|^([\\da-fA-F]{1,4}:){4}"
                    "((:[\\da-fA-F]{1,4}){1,2}|:)$|^([\\da-fA-F]{1,4}:){5}:([\\da-fA-F]{1,4})?$|^([\\da-fA-F]{1,4}:){6}:$");
        if(!rx2.exactMatch(ip))
        {
            //ip地址非法
            return false;
        }
    }

    return true;
}

//检测IP地址的版本
int SupplyWriter::checkIPversion(QString IP)
{
    if (IP.contains(":", Qt::CaseInsensitive))
        return 6;
    else if (IP.contains(".", Qt::CaseInsensitive))
        return 4;

    return 0;
}

//返回false表示离线，返回true表示在线
bool SupplyWriter::check_server_status(const QString serverIP, const int port)
{
    tcpSocket = new QTcpSocket(this);
    this->server_status = false;

    connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnected()));
//    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
//    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

    tcpSocket->connectToHost(serverIP, port);

    tcpSocket->close();
    this->Sleep(10);

    return this->server_status;
}

//关于按钮
void SupplyWriter::on_AboutButton_clicked()
{
    QDate date = QDate::fromString("20221024", "yyyyMMdd");

    QString info = QString("软件版本：1.8.0\n"
                           "编译时间：%1\n"
                           "基于 Qt 5.14.2 开发 by youshun\n\n"
                           "本软件 仅用于辰光融信写入耗材出厂信息，严禁外泄\n"
                           "辰光融信技术有限公司 版权所有\n").arg(date.toString("yyyy.MM.dd"));
    QMessageBox::about(this, tr("关于 耗材写入工具"), info);

    return;
}

void SupplyWriter::on_total_page_radio_clicked()
{
    ui->lineEdit_6->setEnabled(true);
    ui->lineEdit_7->setEnabled(false);
    ui->lineEdit_6->setFocus();
}

void SupplyWriter::on_total_dot_radio_clicked()
{
    ui->lineEdit_6->setEnabled(false);
    ui->lineEdit_7->setEnabled(true);
    ui->lineEdit_7->setFocus();
}

void SupplyWriter::on_beyond_printpage_radio_clicked()
{
    ui->lineEdit_11->setEnabled(true);
    ui->lineEdit_12->setEnabled(false);
    ui->lineEdit_11->setFocus();
}

void SupplyWriter::on_beyond_printdot_radio_clicked()
{
    ui->lineEdit_11->setEnabled(false);
    ui->lineEdit_12->setEnabled(true);
    ui->lineEdit_12->setFocus();
}

//关闭对话框
void SupplyWriter::on_CloseButton_clicked()
{
    this->accept();
}

//展示帮助信息
void SupplyWriter::on_HelpButton_clicked()
{
    HelpDialog* help = new HelpDialog();
    connect(this, SIGNAL(sendThemeMode(int)), help, SLOT(get_theme_id(int)));
    emit sendThemeMode(this->theme_state);
    help->show();
}

//删除记录，根据序列号
void SupplyWriter::on_DeleteSqlButton_clicked()
{
    this->on_pushButton_4_clicked();
    if (status)
    {
        return;
    }

    QString sqlcmd = QString("delete from %1.%2 where serial_no='%3'").arg(DATABASE_NAME).arg(TABLE_NAME).arg(ui->lineEdit_3->text());
    qDebug() << sqlcmd;
    query = QSqlQuery(this->db);
    query.exec(sqlcmd);

    ui->label_13->setText("<font color=green>删除数据库记录成功！</font>");
}

//登录进入耗材信息界面，更新最近使用日期
void SupplyWriter::on_LoginButton_clicked()
{
    if (ui->username->text().length() == 0 ||
        ui->password->text().length() == 0)
    {
        ui->label_21->setText("<font color=red>用户名或密码不能为空！</font>");
        return;
    }

    QString strmd5_username = QCryptographicHash::hash(ui->username->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString strmd5_password = QCryptographicHash::hash(ui->password->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
//    qDebug() << strmd5_username;
//    qDebug() << strmd5_password;

    setting.beginGroup(strmd5_username);
    if (setting.value("name").toString().compare(strmd5_username, Qt::CaseInsensitive) != 0)
    {
        ui->label_21->setText("<font color=red>该用户不存在！</font>");
        setting.endGroup();
        return;
    }
    if (setting.value("pwd").toString().compare(strmd5_password, Qt::CaseInsensitive) != 0)
    {
        ui->label_21->setText("<font color=red>密码不正确！</font>");
        setting.endGroup();
        return;
    }
    //更新最近使用日期
    QString strmd5_recently = QCryptographicHash::hash(QDate::currentDate().toString("yyyy/M/d").toLocal8Bit(), QCryptographicHash::Md5).toHex();
    setting.setValue("recent", strmd5_recently);
    login_user = ui->username->text();
//    qDebug() << login_user;
    setting.endGroup();

    ui->stackedWidget->setCurrentIndex(1);
    this->clear_main_page();
}

//退出登录
void SupplyWriter::on_ExitLogin_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->clear_login_page();
}

//退出界面
void SupplyWriter::on_ExitButton_clicked()
{
    this->accept();
}

//创建用户
void SupplyWriter::on_CreateUser_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    this->clear_newuser_page();
}

//进入重置密码页面
void SupplyWriter::on_ResetPassword_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->stackedWidget_2->setCurrentIndex(0);
    this->clear_resetpwd_page0();
}

//修改密码
void SupplyWriter::on_ModifyPassword_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    this->clear_renewpwd_page();
}

//取消注册，返回登录页面
void SupplyWriter::on_Return_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->clear_login_page();
}

//确认注册新用户
//比对两次密码是否一致，将用户名、密码、注册日期及最近使用日期的哈希值写入注册表
//返回登录页面
void SupplyWriter::on_Confirm_clicked()
{
    if (ui->lineEdit_8->text().length() == 0 ||
        ui->lineEdit_9->text().length() == 0 ||
        ui->lineEdit_10->text().length() == 0)
    {
        ui->label_26->setText("<font color=red>用户名或密码不能为空！</font>");
        return;
    }
    if (ui->lineEdit_9->text().compare(ui->lineEdit_10->text(), Qt::CaseSensitive) != 0)
    {
        ui->label_26->setText("<font color=red>输入密码不一致！</font>");
        return;
    }
    QString strmd5_username = QCryptographicHash::hash(ui->lineEdit_8->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString strmd5_password = QCryptographicHash::hash(ui->lineEdit_9->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString strmd5_register = QCryptographicHash::hash(QDate::currentDate().toString("yyyy/M/d").toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString strmd5_recently = QCryptographicHash::hash(QDate::currentDate().toString("yyyy/M/d").toLocal8Bit(), QCryptographicHash::Md5).toHex();

//    qDebug() << ui->lineEdit_8->text();
//    qDebug() << ui->lineEdit_9->text();
//    qDebug() << QDate::currentDate().toString();
//    qDebug() << QCryptographicHash::hash(ui->lineEdit_8->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
//    qDebug() << QCryptographicHash::hash(ui->lineEdit_9->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
//    qDebug() << QCryptographicHash::hash(QDate::currentDate().toString().toLocal8Bit(), QCryptographicHash::Md5).toHex();
//    qDebug() << QCryptographicHash::hash(QDate::currentDate().toString().toLocal8Bit(), QCryptographicHash::Md5).toHex();

    setting.beginGroup(strmd5_username);
    if (setting.value("name").toString().compare(strmd5_username, Qt::CaseInsensitive) == 0)
    {
        ui->label_26->setText("<font color=red>该用户已存在，不能重复创建！</font>");
        setting.endGroup();
        return;
    }
    setting.setValue("name", strmd5_username);
    setting.setValue("pwd", strmd5_password);
    setting.setValue("origin", strmd5_register);
    setting.setValue("recent", strmd5_recently);
    setting.endGroup();

    ui->label_26->setText("<font color=green>注册成功，将在 3 秒后返回登录页面</font>");
    ui->Confirm->setDisabled(true);
    ui->Return->setDisabled(true);
    this->Sleep(3000);
    ui->stackedWidget->setCurrentIndex(0);
    this->clear_login_page();
}

//取消修改密码
void SupplyWriter::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->clear_login_page();
}

//确定修改密码
void SupplyWriter::on_pushButton_3_clicked()
{
    if (ui->lineEdit_15->text().length() == 0 ||
        ui->lineEdit_16->text().length() == 0 ||
        ui->lineEdit_17->text().length() == 0 ||
        ui->lineEdit_18->text().length() == 0)
    {
        ui->label_31->setText("<font color=red>输入信息不能为空！</font>");
        return;
    }

    if (ui->lineEdit_17->text().compare(ui->lineEdit_18->text(), Qt::CaseSensitive) != 0)
    {
        ui->label_31->setText("<font color=red>密码不一致</font>");
        return;
    }

    QString strmd5_username = QCryptographicHash::hash(ui->lineEdit_15->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString strmd5_password = QCryptographicHash::hash(ui->lineEdit_16->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString strmd5_newpwd = QCryptographicHash::hash(ui->lineEdit_17->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();

    setting.beginGroup(strmd5_username);
    if (setting.value("name").toString().compare(strmd5_username, Qt::CaseInsensitive) != 0)
    {
        ui->label_31->setText("<font color=red>该用户不存在！</font>");
        setting.endGroup();
        return;
    }

    if (setting.value("pwd").toString().compare(strmd5_password, Qt::CaseInsensitive) != 0)
    {
        ui->label_31->setText("<font color=red>原密码不正确！</font>");
        setting.endGroup();
        return;
    }

    setting.setValue("pwd", strmd5_newpwd);
    setting.endGroup();

    ui->label_31->setText("<font color=green>密码修改成功，将在 3 秒后返回登录页面</font>");
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    this->Sleep(3000);
    ui->stackedWidget->setCurrentIndex(0);
    this->clear_login_page();
}

//返回登录页面
void SupplyWriter::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->clear_login_page();
}

//用户名及注册时间、最近使用时间验证正确后，进入密码重置页面
void SupplyWriter::on_pushButton_8_clicked()
{
    if (ui->lineEdit_19->text().length() == 0)
    {
        ui->label_41->setText("<font color=red>用户名不能为空！</font>");
        return;
    }

    resetpwd_username = QCryptographicHash::hash(ui->lineEdit_19->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();

    setting.beginGroup(resetpwd_username);
    if (setting.value("name").toString().compare(resetpwd_username, Qt::CaseInsensitive) != 0)
    {
        ui->label_41->setText("<font color=red>该用户不存在！</font>");
        setting.endGroup();
        return;
    }

    QString strmd5_register = QCryptographicHash::hash(ui->dateEdit_2->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString strmd5_recently = QCryptographicHash::hash(ui->dateEdit_3->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
//    qDebug() << ui->dateEdit_2->text();
//    qDebug() << QDate::currentDate().toString("yyyy/MM/dd");
//    qDebug() << strmd5_register;
//    qDebug() << strmd5_recently;
    if (setting.value("origin").toString().compare(strmd5_register, Qt::CaseInsensitive) != 0)
    {
        ui->label_41->setText("<font color=red>账户注册日期不正确，验证失败！</font>");
        setting.endGroup();
        return;
    }

    if (setting.value("recent").toString().compare(strmd5_recently, Qt::CaseInsensitive) != 0)
    {
        ui->label_41->setText("<font color=red>账户最近使用日期不正确，验证失败！</font>");
        setting.endGroup();
        return;
    }
    setting.endGroup();

    ui->stackedWidget_2->setCurrentIndex(1);
    this->clear_resetpwd_page1();
//    ui->pushButton_10->setAutoDefault(1);
}

//取消重置密码
void SupplyWriter::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->clear_login_page();
}

//确定重置密码
void SupplyWriter::on_pushButton_10_clicked()
{
    if (ui->lineEdit_20->text().length() == 0 ||
        ui->lineEdit_21->text().length() == 0)
    {
        ui->label_40->setText("<font color=red>密码不能为空！</font>");
        return;
    }

    if (ui->lineEdit_20->text().compare(ui->lineEdit_21->text(), Qt::CaseSensitive) != 0)
    {
        ui->label_40->setText("<font color=red>输入密码不一致！</font>");
        return;
    }

    QString strmd5_password = QCryptographicHash::hash(ui->lineEdit_20->text().toLocal8Bit(), QCryptographicHash::Md5).toHex();
//    qDebug() << strmd5_password;
    setting.beginGroup(resetpwd_username);
    setting.setValue("pwd", strmd5_password);
    setting.endGroup();

    ui->label_40->setText("<font color=green>密码重置成功，将在 3 秒后返回登录页面</font>");
    ui->pushButton_9->setDisabled(true);
    ui->pushButton_10->setDisabled(true);
    this->Sleep(3000);
    ui->stackedWidget->setCurrentIndex(0);
    this->clear_login_page();
}

void SupplyWriter::on_checkBox_stateChanged(int state)
{
    this->theme_state = state;
    if (state == 2)
    {
        this->set_style_sheet("dark.qss");
    }
    else if (state == 0)
    {
        this->set_style_sheet("light.qss");
    }
}

void SupplyWriter::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (arg1.length() < 6)
        return;

    QRegExp rx2("^TL-34[01]{1}$");
    if (rx2.exactMatch(arg1))
    {
        ui->lineEdit_6->setText("3000");
        return;
    }

    QRegExp rx1("^TL-34[01]{1}L$");
    if (rx1.exactMatch(arg1))
    {
        ui->lineEdit_6->setText("1500");
        return;
    }

    ui->lineEdit_6->setText("");
}
