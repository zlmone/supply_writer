#ifndef SQLCHIPINFO_H
#define SQLCHIPINFO_H

#include <QDialog>
#include <QMouseEvent>

#include "common.h"

namespace Ui {
class SqlChipInfo;
}

class SqlChipInfo : public QDialog
{
    Q_OBJECT

public:
    explicit SqlChipInfo(QWidget *parent = nullptr);
    ~SqlChipInfo();

private slots:
    void on_pushButton_clicked();
    void recvSqlInfo(struct cgprintech_supply_sqlinfo* ChipInfo);
    void get_theme_id(int state);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::SqlChipInfo *ui;
    bool is_drag = false;
    QPoint mouse_start_point;
    QPoint window_start_point;

    void set_style_sheet(QString filename);
};

#endif // SQLCHIPINFO_H
