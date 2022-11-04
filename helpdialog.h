#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = nullptr);
    ~HelpDialog();

private:
    Ui::HelpDialog *ui;
    bool is_drag = false;
    QPoint mouse_start_point;
    QPoint window_start_point;

    void packing_the_help();
    void set_style_sheet(QString filename);

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_pushButton_clicked();
    void get_theme_id(int state);
};

#endif // HELPDIALOG_H
