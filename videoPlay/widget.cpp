#pragma execution_character_set("utf-8")
#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    this->receiveCurrentFramecount=0;
    this->isend=false;
    ui->setupUi(this);
    this->setWindowTitle("视频播放器");
    this->setWindowIcon(QIcon(":/img/video_icon.png"));
    connect(ui->horizontalSlider,SIGNAL(costomSliderClicked()),this,SLOT(sliderClickedSlot()));//点击进度条信号槽
    ui->horizontalSlider->setSingleStep(1);//设置单步长为1
    qRegisterMetaType<Mat>("Mat");

    ui->btn_backward->setEnabled(false);
    ui->btn_forward->setEnabled(false);
    ui->btn_replay->setEnabled(false);
    ui->btn_stopPlay->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::receiveFrame(int currentFrame, Mat frame)
{
    this->receiveCurrentFramecount=currentFrame;
    this->img1 = QImage(frame.data,frame.cols,frame.rows,QImage::Format_RGB888);
    this->img1 = this->img1.scaled(ui->label->width(),ui->label->height());
    ui->label->setPixmap(QPixmap::fromImage(this->img1));

    ui->horizontalSlider->setValue(this->receiveCurrentFramecount);//设置当前进度条取值
    ui->label_2->setText(QString::number(this->receiveCurrentFramecount));//设置当前帧数
    update();
}

void Widget::sliderClickedSlot()//点击进度条 信号槽
{
    //自定义鼠标点击信号，可以实现点哪 跳转到哪
    qDebug()<<"点击了进度条";
    pthread->pauseThread();//播放暂停
    pthread->setCurrentFrame(ui->horizontalSlider->value());
    pthread->resumeThread();//播放继续 设置为true
}

void Widget::on_horizontalSlider_sliderReleased()//释放进度条
{
    //ui->horizontalSlider->value();获取当前进度条值
    pthread->setCurrentFrame(ui->horizontalSlider->value());
    pthread->resumeThread();//播放继续 设置为true
    //此处一定要先设置进度条再开启线程，否则线程开启时再设置会冲突
}

void Widget::on_horizontalSlider_sliderMoved(int position)//进度条拖动
{
    pthread->pauseThread();//播放暂停
}

void Widget::on_btn_startPlay_clicked()
{
    if(videoFilePath == "")
    {
        QMessageBox::warning(this,"警告","请选择要加载的视频文件！");
        return;
    }
    if(pthread == nullptr)
    {
        QByteArray ba = videoFilePath.toLocal8Bit();
        char* ch = ba.data();
        pthread=new videothread(ch);
        //第五个参数采用Qt::BlockingQueuedConnection   信号发出后发送线程阻塞，直到槽函数执行完毕，继续发送下一条信号
        connect(pthread,SIGNAL(sendFrame(int,Mat)),this,SLOT(receiveFrame(int,Mat)));//接收每一帧Mat
        //connect(pthread,SIGNAL(endPlayVideo()),this,SLOT(setPlayEndSlot()),Qt::BlockingQueuedConnection);
        ui->horizontalSlider->setRange(0,pthread->getVideoAllFramecount());//设置进度条取值范围
        ui->label_4->setText(QString::number(pthread->getVideoAllFramecount()));//设置总帧数
    }
    if(pthread->isRunning())
    {
        if(pthread->getIsrun()==false)
        {
            pthread->resumeThread();//修改为播放状态
            ui->btn_startPlay->setIcon(QIcon(":/img/24gf-pause.png"));
        }
        else
        {
            pthread->pauseThread();;//视频暂停
            ui->btn_startPlay->setIcon(QIcon(":/img/24gf-play.png"));
        }
    }
    else
    {
        pthread->start();
        pthread->setIsrun(true);//视频开始
        ui->btn_startPlay->setIcon(QIcon(":/img/24gf-pause.png"));

        ui->btn_backward->setEnabled(true);
        ui->btn_forward->setEnabled(true);
        ui->btn_replay->setEnabled(true);
        ui->btn_stopPlay->setEnabled(true);
    }
}


void Widget::on_btn_backward_clicked()
{
    pthread->pauseThread();//播放暂停
    int curFrameIndex = ui->horizontalSlider->value();
    if(curFrameIndex < 100)
    {
        ui->horizontalSlider->setValue(0);
    }
    else
    {
        ui->btn_forward->setEnabled(true);
        ui->horizontalSlider->setValue(curFrameIndex-100);
    }
    pthread->setCurrentFrame(ui->horizontalSlider->value());
    pthread->resumeThread();//播放继续 设置为true
    ui->btn_startPlay->setIcon(QIcon(":/img/24gf-pause.png"));
}


void Widget::on_btn_forward_clicked()
{
    pthread->pauseThread();//播放暂停
    int curFrameIndex = ui->horizontalSlider->value();
    if(curFrameIndex > (pthread->getVideoAllFramecount() -100))
    {
        ui->btn_forward->setEnabled(false);
        ui->horizontalSlider->setValue(pthread->getVideoAllFramecount()-1);
    }
    else
    {
        ui->btn_backward->setEnabled(true);
        ui->horizontalSlider->setValue(curFrameIndex+100);
    }
    pthread->setCurrentFrame(ui->horizontalSlider->value());
    pthread->resumeThread();//播放继续 设置为true
    ui->btn_startPlay->setIcon(QIcon(":/img/24gf-pause.png"));
}


void Widget::on_btn_stopPlay_clicked()
{
    pthread->stopThread();//结束线程
    qDebug()<<"结束播放，线程关闭";
    if(!pthread->isRunning())//线程不在运行
    {
        if(this->isend!=true)//此时线程结束 已释放，就不再释放
        {
            //断开连接
            disconnect(pthread,SIGNAL(sendFrame(int,Mat)),this,SLOT(receiveFrame(int,Mat)));//接收每一帧Mat
            delete pthread;
            this->pthread = nullptr;
        }
    }
    ui->btn_startPlay->setIcon(QIcon(":/img/24gf-play.png"));
    ui->btn_backward->setEnabled(false);
    ui->btn_forward->setEnabled(false);
    ui->btn_replay->setEnabled(false);
    ui->btn_stopPlay->setEnabled(false);
    ui->label_2->setText("0");
    ui->label_4->setText("0");
    ui->label->setText("视频播放器");
    ui->widget_FileOpen->show();
    videoFilePath = "";
    ui->horizontalSlider->setValue(0);
    ui->horizontalSlider->setEnabled(false);
}


void Widget::on_btn_replay_clicked()
{
    pthread->stopThread();//结束线程
    qDebug()<<"重新播放";
    if(!pthread->isRunning())//线程不在运行
    {
        if(this->isend!=true)//此时线程结束 已释放，就不再释放
        {
            //断开连接
            disconnect(pthread,SIGNAL(sendFrame(int,Mat)),this,SLOT(receiveFrame(int,Mat)));//接收每一帧Mat
            delete pthread;
            this->pthread = nullptr;
        }

        //创建新线程
        QByteArray ba = videoFilePath.toLocal8Bit();
        char* ch = ba.data();
        pthread=new videothread(ch);
        pthread->start();
        pthread->setIsrun(true);//视频开始
        connect(pthread,SIGNAL(sendFrame(int,Mat)),this,SLOT(receiveFrame(int,Mat)));//接收每一帧Mat

        this->isend=false;//表明此时线程还未结束
        ui->btn_startPlay->setIcon(QIcon(":/img/24gf-pause.png"));
        ui->btn_backward->setEnabled(true);
        ui->btn_forward->setEnabled(true);
    }

}


void Widget::on_btn_openFile_clicked()
{
    QSettings setting("./Setting.ini", QSettings::IniFormat);          //为了能记住上次打开的路径
    QString lastPath = setting.value("LastFilePath").toString();
    QString FilePath = QFileDialog::getOpenFileName(this,"加载视频文件",lastPath,QObject::tr("所有文件(*.mp4)"));
    if(FilePath.isEmpty())
    {
        QMessageBox::warning(this,"警告","未选择文件，请重新选择！");
        return;
    }
    videoFilePath = FilePath;
    ui->widget_FileOpen->hide();
    ui->horizontalSlider->setEnabled(true);
    on_btn_startPlay_clicked();
}

