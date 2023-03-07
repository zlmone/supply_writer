#ifndef READBACK_H
#define READBACK_H

#include <QDialog>
#include <QMouseEvent>
#include <QPalette>

#include "common.h"

namespace Ui {
class ReadBack;
}

static QString _trademark[] =
{
    "Cgprintech",
    "Lanxum"
};

class ReadBack : public QDialog
{
    Q_OBJECT

public:
    explicit ReadBack(QWidget *parent = nullptr);
    ~ReadBack();
    void set_dialog_style();

private slots:
    void show_ChipInfo(struct cgprintech_supply_info_readback* ChipInfo);
    void on_pushButton_7_clicked();
    void get_theme_id(int state);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::ReadBack *ui;
    QPalette palette;

    unsigned int Unpack32(unsigned char* src);
    unsigned int Unpack16(unsigned char* src);
    void set_style_sheet(QString filename);
    void print_chipinfo(struct cgprintech_supply_info_readback* ChipInfo);

    bool is_drag = false;
    QPoint mouse_start_point;
    QPoint window_start_point;
};

#endif // READBACK_H
