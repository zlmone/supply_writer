#include <QDebug>

#include "readback.h"
#include "ui_readback.h"
#include "supplywriter.h"

extern SupplyWriter* writer;

ReadBack::ReadBack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReadBack)
{
    ui->setupUi(this);

    setFixedSize(this->width(), this->height());
    setWindowFlags(Qt::WindowCloseButtonHint);
//    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->lineEdit->setReadOnly(true);
    ui->pushButton_7->setDefault(true);

    connect(writer, SIGNAL(sendThemeMode(int)), this, SLOT(get_theme_id(int)));
    connect(writer, SIGNAL(sendChipInfo(struct cgprintech_supply_info_readback*)),
            this, SLOT(show_ChipInfo(struct cgprintech_supply_info_readback*)));
//    this->set_dialog_style();
}

void ReadBack::set_style_sheet(QString filename)
{
    QString qssfile = QString(":/qss/%1").arg(filename);
    QFile skinfile(qssfile);
    skinfile.open(QIODevice::ReadOnly);
    this->setStyleSheet(skinfile.readAll());
    skinfile.close();
}

void ReadBack::get_theme_id(int state)
{
    if (state == 2)
    {
        this->set_style_sheet("dark.qss");
    }
    else if (state == 0)
    {
        this->set_style_sheet("light.qss");
    }
}

void ReadBack::set_dialog_style()
{
    palette.setColor(QPalette::Window, QColor(216, 186, 174));
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

ReadBack::~ReadBack()
{
    delete ui;
}

unsigned int ReadBack::Unpack16(unsigned char *src)
{
    return (((unsigned int)src[0]) << 8
          | ((unsigned int)src[1]));
}

unsigned int ReadBack::Unpack32(unsigned char *src)
{
    return(((unsigned int)src[0]) << 24
         | ((unsigned int)src[1]) << 16
         | ((unsigned int)src[2]) << 8
         | (unsigned int)src[3]);
}

void ReadBack::print_chipinfo(struct cgprintech_supply_info_readback* supply_info)
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
    printf("printed free pages\t%d\n", Unpack16(supply_info->print_free_pages));
    printf("printed beyond pages\t%d\n", Unpack16(supply_info->print_beyond_pages));
    printf("printed_pages\t%d\n", Unpack32(supply_info->printed_pages));
    printf("printed_dots\t%d\n", Unpack32(supply_info->printed_dots));
}

void ReadBack::show_ChipInfo(struct cgprintech_supply_info_readback* ChipInfo)
{
    char buff[64];

//    print_chipinfo(ChipInfo);

    ui->label->setText(ChipInfo->model_id);
    ui->lineEdit->setText(ChipInfo->serial_no);
    ui->label_3->setText(ChipInfo->marketing_area);
    ui->label_6->setText(ChipInfo->manufacturer);
    ui->label_7->setText(ChipInfo->trade_mark);
    ui->label_8->setText(ChipInfo->type);

    sprintf(buff, "%02x%02x.%x.%x", ChipInfo->product_date[0], ChipInfo->product_date[1],
            ChipInfo->product_date[2], ChipInfo->product_date[3]);
    ui->label_4->setText(buff);

    sprintf(buff, "%02x%02x.%x.%x", ChipInfo->install_date[0], ChipInfo->install_date[1],
            ChipInfo->install_date[2], ChipInfo->install_date[3]);
    ui->label_11->setText(buff);

    sprintf(buff, "%02x%02x.%x.%x", ChipInfo->recent_date[0], ChipInfo->recent_date[1],
            ChipInfo->recent_date[2], ChipInfo->recent_date[3]);
    ui->label_9->setText(buff);

    sprintf(buff, "%u", Unpack32(ChipInfo->pages));
    ui->label_22->setText(buff);
//    sprintf(buff, "%u", Unpack32(ChipInfo->dots));
//    ui->label_18->setText(buff);
    sprintf(buff, "%u", Unpack16(ChipInfo->beyond_pages));
    ui->label_30->setText(buff);
//    sprintf(buff, "%u%%", Unpack16(ChipInfo->beyond_percent));
//    ui->label_29->setText(buff);
    sprintf(buff, "%u", Unpack16(ChipInfo->free_pages));
    ui->label_5->setText(buff);
    sprintf(buff, "%u", Unpack32(ChipInfo->printed_pages));
    ui->label_10->setText(buff);
//    sprintf(buff, "%u", Unpack32(ChipInfo->printed_dots));
//    ui->label_32->setText(buff);
    sprintf(buff, "%u", Unpack16(ChipInfo->print_beyond_pages));
    ui->label_34->setText(buff);
    sprintf(buff, "%u", Unpack16(ChipInfo->print_free_pages));
    ui->label_35->setText(buff);

//    sprintf(buff, "序列号：%s", ChipInfo->serial_no);
    this->setWindowTitle(ChipInfo->serial_no);
}

void ReadBack::mousePressEvent(QMouseEvent *event)
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

void ReadBack::mouseMoveEvent(QMouseEvent *event)
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

void ReadBack::mouseReleaseEvent(QMouseEvent *event)
{
    //放下左键即停止移动
    if (event->button() == Qt::LeftButton)
    {
        is_drag = false;
    }
}

void ReadBack::on_pushButton_7_clicked()
{
    this->accept();
}
