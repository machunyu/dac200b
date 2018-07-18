#ifndef SHOWPICSDLG_H
#define SHOWPICSDLG_H

#include <QDialog>
#include <QPainter>
#include <QDesktopWidget>
#include <QApplication>

namespace Ui {
class ShowPicsDlg;
}

class ShowPicsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ShowPicsDlg(QWidget *parent = 0);
    ~ShowPicsDlg();

private slots:
    void on_m_btnPrev_clicked();

    void on_m_btnNext_clicked();

    void timerEvent(QTimerEvent *event);

private:
    void paintEvent(QPaintEvent*); // 绘制事件处

private:
    Ui::ShowPicsDlg *ui;

    int m_idxImage; // 当前显示图片索引
    int m_nTimerId;	//定时器ID
};

#endif // SHOWPICSDLG_H
