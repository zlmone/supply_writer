#include <QDebug>

#include "sqlchipinfo.h"
#include "ui_sqlchipinfo.h"
#include "supplywriter.h"

extern SupplyWriter* writer;

SqlChipInfo::SqlChipInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqlChipInfo)
{
    ui->setupUi(this);

    setFixedSize(this->width(), this->height());
    setWindowFlags(Qt::WindowCloseButtonHint);
//    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->lineEdit->setReadOnly(true);
    ui->pushButton->setDefault(true);

    connect(writer, SIGNAL(sendThemeMode(int)), this, SLOT(get_theme_id(int)));
    connect(writer, SIGNAL(sendSqlInfo(struct cgprintech_supply_sqlinfo*)),
            this, SLOT(recvSqlInfo(struct cgprintech_supply_sqlinfo*)));
}

SqlChipInfo::~SqlChipInfo()
{
    delete ui;
}

void SqlChipInfo::set_style_sheet(QString filename)
{
    QString qssfile = QString(":/qss/%1").arg(filename);
    QFile skinfile(qssfile);
    skinfile.open(QIODevice::ReadOnly);
    this->setStyleSheet(skinfile.readAll());
    skinfile.close();
}

void SqlChipInfo::get_theme_id(int state)
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

void SqlChipInfo::recvSqlInfo(struct cgprintech_supply_sqlinfo* ChipInfo)
{
    ui->label_2->setText(ChipInfo->model_id);
    ui->lineEdit->setText(ChipInfo->serial_no);
    ui->label_15->setText(ChipInfo->marketing_area);
    ui->label_16->setText(ChipInfo->product_date);
    ui->label_17->setText(ChipInfo->pages);
    ui->label_18->setText(ChipInfo->dots);
    ui->label_19->setText(ChipInfo->manufacturer);
    ui->label_20->setText(ChipInfo->trade_mark);
    ui->label_21->setText(ChipInfo->type);
    ui->label_22->setText(ChipInfo->overflow_pages);
    ui->label_23->setText(ChipInfo->overflow_percent);
    ui->label_24->setText(ChipInfo->free_pages);
    ui->label_25->setText(ChipInfo->operator_id);

    this->setWindowTitle(ChipInfo->serial_no);
}

void SqlChipInfo::on_pushButton_clicked()
{
    this->accept();
}

void SqlChipInfo::mousePressEvent(QMouseEvent *event)
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

void SqlChipInfo::mouseMoveEvent(QMouseEvent *event)
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

void SqlChipInfo::mouseReleaseEvent(QMouseEvent *event)
{
    //放下左键即停止移动
    if (event->button() == Qt::LeftButton)
    {
        is_drag = false;
    }
}
