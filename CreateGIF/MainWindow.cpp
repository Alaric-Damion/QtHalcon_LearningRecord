#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDateTime>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QStyle>
#include <QApplication>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QScreen>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    mGifWriter = nullptr;
	mFps = 15;
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(slotSaveImage()));

    mFrameHelper = new FramelessHelper(this);
    mFrameHelper->activateOn(this);			//激活当前窗体
    mFrameHelper->setTitleHeight(30);		//设置窗体的标题栏高度
    mFrameHelper->setWidgetMovable(true);	//设置窗体可移动
    mFrameHelper->setWidgetResizable(true); //设置窗体可缩放

	//UpdateSizeText();
	mTimerID = startTimer(50);
}

MainWindow::~MainWindow()
{
    if (nullptr != mGifWriter)
    {
        delete mGifWriter;
        mGifWriter = nullptr;
    }
    delete ui;
}

void MainWindow::on_btnRecord_clicked()
{
    mFps = ui->lineEditFps->text().toInt();
    mFile = QDateTime::currentDateTime().toString("yyyyMMddhhmmss.gif");
    if (nullptr != mGifWriter)
    {
        delete mGifWriter;
        mGifWriter = nullptr;
    }

    int w = ui->midSpacer->geometry().width();
    int h = ui->midSpacer->geometry().height();

    mGifWriter = new Gif_H::GifWriter;
    if (!mGif.GifBegin(mGifWriter, mFile.toLocal8Bit().data(), w, h, mFps))
    {
        delete mGifWriter;
        mGifWriter = nullptr;
        return;
    }

    slotSaveImage();

    mTimer.start(1000 / mFps);
    ui->labelStatus->setText("开始录制...");
}

void MainWindow::on_btnStop_clicked()
{
    if (!mGifWriter)
        return;
    mTimer.stop();
    mGif.GifEnd(mGifWriter);

    delete mGifWriter;
    mGifWriter = nullptr;

    ui->labelStatus->setText("录制完成：" + QCoreApplication::applicationDirPath() + "/" + mFile);

    QDesktopServices::openUrl(QUrl(mFile));
}

void MainWindow::on_btnClose_clicked()
{
	qApp->exit();
}

void MainWindow::slotSaveImage()
{
//    qDebug() << QDateTime::currentDateTime().toString("yyyyMMddhhmmss.zzz");
    if (!mGifWriter)
        return;

    QRect rc = ui->midSpacer->geometry();
    int w = rc.width();
    int h = rc.height();
    QScreen *screen = QApplication::primaryScreen();
    QPixmap pix = screen->grabWindow(0, x() + rc.x(), y() + rc.y(), w, h);
    QImage image = pix.toImage().convertToFormat(QImage::Format_RGBA8888);
    mGif.GifWriteFrame(mGifWriter, image.bits(), w, h, mFps);
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	if (mTimerID == event->timerId())
	{
		killTimer(mTimerID);
		UpdateSizeText();
	}
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor("#67b5a2"));

    QRect rc = ui->midSpacer->geometry();
    painter.setCompositionMode( QPainter::CompositionMode_Clear );
    painter.fillRect(rc, Qt::SolidPattern );
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    UpdateSizeText();
}

void MainWindow::UpdateSizeText()
{
    int w = ui->midSpacer->geometry().width();
    int h = ui->midSpacer->geometry().height();
    ui->labelSize->setText(QString("尺寸: %1 * %2").arg(w).arg(h));
}
