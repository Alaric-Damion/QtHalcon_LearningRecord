#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tchar.h"
#include "JHCap.h"
#include "aboutdialog.h"
#include "capturethread.h"
#include <QMessageBox>
#include <QDebug>
#include <QtGui>
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    #include <QDateTime>
    #include <QTranslator>
    #include <QFileDialog>
#endif

const UINT WM_USERDEFMSG = ::RegisterWindowMessage(QString("JH CAMERA").toStdWString().c_str());

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon/app"));

    m_setdlg = new SettingDialog(this);
    m_setdlg->hide();

    m_aboutdlg=new AboutDialog(this);
    m_aboutdlg->hide();

    m_histogram=new Histogram(this);
    m_histogram->hide();

    m_active_view = 0;
    for(int i=0; i<CAMN; i++)
    {
        m_view[i].m_image_item = 0;
        m_view[i].m_resoIndex = 0;
        m_view[i].m_fps = 0;
        m_view[i].m_totalFrame = 0;
        m_view[i].m_droppedFrame = 0;
        m_view[i].m_camera_id = -1;

        m_view[i].m_scene = new QGraphicsScene(this);
        m_view[i].m_thread = new CaptureThread(this);
        m_view[i].m_thread->set_viewId(i);
        //start task thread
        connect(m_view[i].m_thread, SIGNAL(captured(int, QImage, unsigned char *, qint64)),
                this, SLOT(process(int, QImage, unsigned char *, qint64)));
    }

    ui->gvMain->setScene(m_view[0].m_scene);
    ui->gvSecond->setScene(m_view[1].m_scene);

    ui->splitter->setOrientation(Qt::Horizontal); //Qt::Vertical
    m_language = LANG_EN;
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateFps()));
    m_timer->start(1000);
    msgLabel = new QLabel(this);
    msgLabel->setAlignment(Qt::AlignHCenter);
    ui->statusBar->addWidget(msgLabel);

    fpsLabel = new QLabel(tr("fps:0.00"));
    fpsLabel->setIndent(3);
    fpsLabel->setAlignment(Qt::AlignRight);
    fpsLabel->setMinimumSize(fpsLabel->sizeHint());
    ui->statusBar->addWidget(fpsLabel, 1);


    m_deviceCombo[0] = new QComboBox(this);    
    ui->toolBar->addWidget(m_deviceCombo[0]);
    m_resoCombo[0] = new QComboBox(this);    
    ui->toolBar->addWidget(m_resoCombo[0]);
    m_deviceCombo[1] = new QComboBox(this);    
    ui->toolBar->addWidget(m_deviceCombo[1]);
    m_resoCombo[1] = new QComboBox(this);    
    ui->toolBar->addWidget(m_resoCombo[1]);

    int count=0;
    CameraGetCount(&count);
    m_deviceCombo[0]->addItem("Select...");
    m_deviceCombo[1]->addItem("Select...");
    for(int i=0; i<count; i++)
    {
        char name[255], model[255];
        CameraGetName(i, name, model);
        m_deviceCombo[0]->addItem(QString("%1").arg(model));
        m_deviceCombo[1]->addItem(QString("%1").arg(model));
    }   

    connect(m_deviceCombo[0], SIGNAL(currentIndexChanged(int)), this, SLOT(deviceChange1(int)));
    connect(m_resoCombo[0], SIGNAL(currentIndexChanged(int)), this, SLOT(resolutionChange1(int)));
    connect(m_deviceCombo[1], SIGNAL(currentIndexChanged(int)), this, SLOT(deviceChange2(int)));
    connect(m_resoCombo[1], SIGNAL(currentIndexChanged(int)), this, SLOT(resolutionChange2(int)));

    connect(ui->action_Play,SIGNAL(triggered()),this,SLOT(streamVideo()));
    connect(ui->action_Stop,SIGNAL(triggered()),this,SLOT(pauseVideo()));
    connect(ui->action_Save,SIGNAL(triggered()),this,SLOT(saveImage()));
    connect(ui->action_Setting,SIGNAL(triggered()),this,SLOT(openSetting()));
    connect(ui->actionZoom_In,SIGNAL(triggered()),this,SLOT(zoomIn()));
    connect(ui->actionZoom_Out,SIGNAL(triggered()),this,SLOT(zoomOut()));
    connect(ui->actionReset,SIGNAL(triggered()),this,SLOT(resetView()));
    connect(ui->actionEnglish_Chinese,SIGNAL(triggered()),this,SLOT(switchlanguage()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(aboutMe()));
    connect(ui->actionHistogram,SIGNAL(triggered()),this,SLOT(showHistogram()));    
}



MainWindow::~MainWindow()
{
    //qDebug() << "~~";
    delete m_setdlg;
    delete m_aboutdlg;
    delete m_histogram;

    for(int i=0; i<CAMN; i++)
    {
        delete m_view[i].m_scene;
        delete m_view[i].m_thread;
    }

    delete m_timer;
    delete msgLabel;
    delete fpsLabel;

    delete m_deviceCombo[0];
    delete m_deviceCombo[1];
    delete m_resoCombo[0];
    delete m_resoCombo[1];

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * e)
{
    for(int i=0; i<CAMN; i++)
    {
        m_view[i].m_thread->stop();
        m_view[i].m_thread->wait();
        CameraFree(m_view[i].m_camera_id);
    }
    QMainWindow::closeEvent(e);
}

//qt5
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    MSG* msg = reinterpret_cast<MSG*>(message);
    return winEvent(msg, result);
}

bool MainWindow::winEvent(MSG *message, long *result)
{
    static int i=0;
    if(message->message == WM_USERDEFMSG)
    {
        this->statusBar()->showMessage(QString("%1 WM_USERDEFMSG received.").arg(i++));
    }
#if QT_VERSION < 0x050000
        return QMainWindow::winEvent(message, result);
#else
        return QMainWindow::nativeEvent("", message, result);
#endif
}

void MainWindow::zoomIn()
{
    if(m_active_view ==0)
        ui->gvMain->scale(1.2, 1.2);
    else
        ui->gvSecond->scale(1.2, 1.2);
}

void MainWindow::zoomOut()
{
    if(m_active_view ==0)
        ui->gvMain->scale(1/1.2, 1/1.2);
    else
        ui->gvSecond->scale(1/1.2, 1/1.2);
}

void MainWindow::resetView()
{
    if(m_active_view==0)
        ui->gvMain->resetTransform ();
    else
        ui->gvSecond->resetTransform();
}

void MainWindow::resolutionChange1(int index)
{
    resolutionChange(index, 0);
}
void MainWindow::deviceChange1(int index)
{
    m_active_view = 0;
    deviceChange(index-1, 0);
}

void MainWindow::resolutionChange2(int index)
{
    resolutionChange(index, 1);
}
void MainWindow::deviceChange2(int index)
{
    m_active_view = 1;
    deviceChange(index-1, 1);
}

void MainWindow::resolutionChange(int index, int viewid)
{
    m_view[viewid].m_thread->pause();
    CameraSetResolution(m_view[viewid].m_camera_id, index, 0, 0);
    m_view[viewid].m_thread->stream();
}

void MainWindow::deviceChange(int index, int viewid)
{    
    m_view[viewid].m_thread->pause();

    if(m_view[1-viewid].m_camera_id==index)
    {
        QMessageBox::information(this, "Warn", "Camera is in use.");
        return;
    }
    if(m_view[viewid].m_camera_id==index)//same
    {
        return;
    }

    CameraFree(m_view[viewid].m_camera_id);
    CameraInit(index);
    CameraLoadParameter(index,0);
    CameraLoadParameter(index,0);
    //set_trigger_message(index,this->winId(), WM_USERDEFMSG);

    m_view[viewid].m_camera_id = index;
    m_view[viewid].m_thread->set_deviceId(index);

    int c = m_resoCombo[viewid]->count();
    for(int i=0; i<c; i++)
    {
        m_resoCombo[viewid]->removeItem(0);
    }
    int count =0;
    CameraGetResolutionCount(index,&count);
    //int maxDepth, maxWidth, maxHeight;
    //get_resolution_max(&maxWidth, &maxHeight,&maxDepth);

    for(int i=0; i<count; i++)
    {
        int width, height;
        CameraGetResolution(index,i, &width, &height);
        m_resoCombo[viewid]->addItem(QString("%1x%2").arg(width).arg(height));

    }
}

void MainWindow::streamVideo()
{
    m_view[m_active_view].m_fps = 0;
    m_view[m_active_view].m_totalFrame = 0;
    m_view[m_active_view].m_droppedFrame = 0;
    m_view[m_active_view].m_thread->start();
    m_view[m_active_view].m_thread->stream();
}

void MainWindow::pauseVideo()
{    
    m_view[m_active_view].m_thread->pause();
}


void MainWindow::openSetting()
{
    m_setdlg->show();
    m_setdlg->set_deviceid(m_view[m_active_view].m_camera_id);
    m_setdlg->InitParameter();
    m_setdlg->InitWBGain();
}

void MainWindow::saveImage()
{
    QDateTime d = QDateTime::currentDateTime();
    QString suggestFileName = QString("snapshot-%1.bmp").arg( d.toString("yyyyMMddhhmmss"));
    QString fileName = QFileDialog::getSaveFileName(this,
         tr("Save image to file"), suggestFileName, tr("*.bmp | *.jpg"));

    if(!fileName.isNull())
    {
        m_view[m_active_view].m_image_item->pixmap().save(fileName);
    }
}

void MainWindow::resetHardware()
{

}
void MainWindow::switchlanguage()
{

    QTranslator translator;
    if(LANG_EN == m_language){
        translator.load("lang/usbVideo_zh_CN");
        m_language=LANG_CN;

    }
    else
    {
        m_language = LANG_EN;
    }
    qApp->installTranslator(&translator);
    ui->retranslateUi(this);
    m_setdlg->switchLanguage();
    m_aboutdlg->switchLanguage();
    m_histogram->switchLanguage();
}

void MainWindow::aboutMe()
{
    m_aboutdlg->show();
}

void MainWindow::updateFps()
{
    for(int i=0; i<CAMN; i++)
    {
        if(m_view[i].m_fps == 0) m_view[i].m_fps = m_view[i].m_totalFrame;
        else m_view[i].m_fps = 0.6*m_view[i].m_fps + 0.4*m_view[i].m_totalFrame;
        m_view[i].m_totalFrame = 0;
        m_view[i].m_droppedFrame = 0;
    }
    fpsLabel->setText(QString(tr("Active Camera %5 | Camera 0 fps: %1 dropped: %2 | Camera 1 fps: %3 dropped: %4"))
                      .arg(QString::number(m_view[0].m_fps, 'f', 2))
                      .arg(m_view[0].m_droppedFrame)
                      .arg(QString::number(m_view[1].m_fps, 'f', 2))
                      .arg(m_view[1].m_droppedFrame)
                      .arg(m_active_view)
    );
}

void MainWindow::process(int id, QImage img, unsigned char * buffer, qint64 timestamp)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();

    if(now-timestamp>150) //skip old frame
    {
        m_view[id].m_droppedFrame++;
        delete [] buffer;
        return;
    }
    if(m_view[id].m_image_item)
    {
        m_view[id].m_scene->removeItem(m_view[id].m_image_item);
        delete m_view[id].m_image_item;
        m_view[id].m_image_item = 0;
    }

    m_view[id].m_image_item = m_view[id].m_scene->addPixmap(QPixmap::fromImage(img));
    m_view[id].m_scene->setSceneRect(0, 0, img.width(), img.height());
    delete [] buffer;
    m_view[id].m_totalFrame++;
}

void MainWindow::showHistogram()
{
    if(m_view[m_active_view].m_image_item)
     {
        m_histogram->setImage(m_view[m_active_view].m_image_item->pixmap().toImage());
        m_histogram->show();
     }
    else
        QMessageBox::about(NULL, "Notice", "Please play the video!");
}

void MainWindow::on_actionWindow_Switch_triggered()
{
    m_active_view = 1-m_active_view;
}
