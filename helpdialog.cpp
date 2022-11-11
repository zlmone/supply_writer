#include <QUrl>
#include <QDir>

#include "helpdialog.h"
#include "ui_helpdialog.h"
#include "supplywriter.h"

extern SupplyWriter* writer;

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    setFixedSize(this->width(), this->height());
    setWindowFlags(Qt::WindowCloseButtonHint);
//    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setWindowTitle("耗材写入工具 帮助");
    ui->pushButton->setDefault(true);

    connect(writer, SIGNAL(sendThemeMode(int)), this, SLOT(get_theme_id(int)));

    ui->textBrowser->setOpenLinks(true);
    ui->textBrowser->setOpenExternalLinks(true);
    this->packing_the_help();
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::set_style_sheet(QString filename)
{
    QString qssfile = QString(":/qss/%1").arg(filename);
    QFile skinfile(qssfile);
    skinfile.open(QIODevice::ReadOnly);
    this->setStyleSheet(skinfile.readAll());
    skinfile.close();
}

void HelpDialog::get_theme_id(int state)
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

void HelpDialog::packing_the_help()
{
    QString current_path = QDir::currentPath();
    QString filename = QString("%1\\%2").arg(current_path).arg("help.md");

    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    ui->textBrowser->setMarkdown(file.readAll());
    file.close();
}

void HelpDialog::mousePressEvent(QMouseEvent *event)
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

void HelpDialog::mouseMoveEvent(QMouseEvent *event)
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

void HelpDialog::mouseReleaseEvent(QMouseEvent *event)
{
    //放下左键即停止移动
    if (event->button() == Qt::LeftButton)
    {
        is_drag = false;
    }
}

void HelpDialog::on_pushButton_clicked()
{
    this->accept();
}
