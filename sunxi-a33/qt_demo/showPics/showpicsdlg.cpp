#include "showpicsdlg.h"
#include "ui_showpicsdlg.h"

ShowPicsDlg::ShowPicsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowPicsDlg),
    m_idxImage(0)
{
    ui->setupUi(this);
	QDesktopWidget* desktopWidget = QApplication::desktop();
	QRect screenRect = desktopWidget->screenGeometry();
    resize(screenRect.width(), screenRect.height());
    m_nTimerId = startTimer(1000);
}

ShowPicsDlg::~ShowPicsDlg()
{
    if (m_nTimerId != 0)
        killTimer(m_nTimerId);
    delete ui;
}

void ShowPicsDlg::paintEvent(QPaintEvent* event) {
	(void*)event;
    // 创建绘制器对象
    QPainter painter (this);
    // 计算绘制区域
    QRect rcImage = ui->m_frmImage->frameRect();
    rcImage.translate(ui->m_frmImage->pos());
    // 从资源加载图像
    QImage image (":/Res/" +
    QString::number (m_idxImage) + ".jpg");
    // 绘制图像
    painter.drawImage(rcImage, image);
}

void ShowPicsDlg::on_m_btnPrev_clicked()
{
    if (--m_idxImage < 0)
        m_idxImage = 9;
    repaint (); // 触发重绘事件
}

void ShowPicsDlg::on_m_btnNext_clicked()
{
    if (++m_idxImage > 9)
            m_idxImage = 0;
        repaint (); // 触发重绘事件
}

void ShowPicsDlg::timerEvent(QTimerEvent *event)
{
	(void*)event;
    m_idxImage = (++m_idxImage) % 10;
    repaint();
}
