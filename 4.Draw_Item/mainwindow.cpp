#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDir>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ImageItem = new QGraphicsPixmapItem();
    m_scene = new BQGraphicsScene();

    m_scene->setBackgroundBrush(Qt::black);
    ui->graphicsView->setScene(m_scene);
    m_scene->addItem(ImageItem);
    ui->graphicsView->setContextMenuPolicy(Qt::CustomContextMenu);
    //    ItemGroup = new QGraphicsItemGroup();
    //    ItemGroup->hide();
    //    m_scene->addItem(ItemGroup);
    //    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //    ui->graphicsView->setCursor(Qt::PointingHandCursor);
    //    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    //    ui->graphicsView->setSceneRect(INT_MIN/2, INT_MIN/2, INT_MAX, INT_MAX);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_circleBtn_clicked()
{
    //    BCircle *m_circle = new BCircle(0, 0, 50, BGraphicsItem::ItemType::Circle);
    //    ItemGroup->addToGroup(m_circle);
    //    m_scene->addItem(m_circle);
}

//椭圆
void MainWindow::on_ellipseBtn_clicked()
{
    BEllipse *m_ellipse = new BEllipse(0, 0, 120, 80, BGraphicsItem::ItemType::Ellipse);
    m_scene->addItem(m_ellipse);
}

//同心圆
void MainWindow::on_conCircleBtn_clicked()
{
    BConcentricCircle *m_conCircle = new BConcentricCircle(0, 0, 50, 80, BGraphicsItem::ItemType::Concentric_Circle);
//        m_conCircle->setPos(500,500);
    m_scene->addItem(m_conCircle);
}

//饼图
void MainWindow::on_pieBtn_clicked()
{
    BPie *m_pie = new BPie(0, 0, 80, 30, BGraphicsItem::ItemType::Pie);
    m_scene->addItem(m_pie);
}

//合弦
void MainWindow::on_chordBtn_clicked()
{
    BChord *m_chord = new BChord(0, 0, 80, 30, BGraphicsItem::ItemType::Chord);
    m_scene->addItem(m_chord);
}

//正方形
void MainWindow::on_squareBtn_clicked()
{
    BSquare *m_square = new BSquare(0, 0, 60, BGraphicsItem::ItemType::Square);
    m_scene->addItem(m_square);
}

//矩形
void MainWindow::on_rectangleBtn_clicked()
{
    m_rectangle = new BRectangle(0, 0, 300, 300, BGraphicsItem::ItemType::Rectangle);
    QPen mPen = QPen(Qt::red,4);
    m_rectangle->setPen(mPen);
    m_rectangle->setPos(50,50);
    m_scene->addItem(m_rectangle);
    qDebug()<<m_rectangle->GetRectLength("Length1");
}

//多边形
void MainWindow::on_polygonBtn_clicked()
{
    m_scene->startCreate();
    setBtnEnabled(false);
    BPolygon *m_polygon = new BPolygon(BGraphicsItem::ItemType::Polygon);
    m_scene->addItem(m_polygon);

    connect(m_scene, SIGNAL(updatePoint(QPointF, QList<QPointF>, bool)), m_polygon, SLOT(pushPoint(QPointF, QList<QPointF>, bool)));
    connect(m_scene, &BQGraphicsScene::createFinished, [=](){
        setBtnEnabled(true);
    });
}

//圆端矩形
void MainWindow::on_rnRecBtn_clicked()
{
    BRound_End_Rectangle *m_round_end_Rectangle = new BRound_End_Rectangle(0, 0, 80, 60, BGraphicsItem::ItemType::Round_End_Rectangle);
    m_scene->addItem(m_round_end_Rectangle);
}

//圆角矩形
void MainWindow::on_roundRecBtn_clicked()
{
    BRounded_Rectangle *m_rounded_Rectangle = new BRounded_Rectangle(0, 0, 80, 60, BGraphicsItem::ItemType::Rounded_Rectangle);
    m_scene->addItem(m_rounded_Rectangle);

}

//清空所有Item
void MainWindow::on_clearBtn_clicked()
{
    m_scene->clear();
}

//保存Item
void MainWindow::on_saveBtn_clicked()
{



    //    qDebug()<<"rect"<<m_RotateRect->scenePos().x()<<m_RotateRect->scenePos().y()<<endl;
    //        qDebug()<<"rect"<<m_RotateRect->m_RotateCenter.x()<<m_RotateRect->m_RotateCenter.y()<<endl;
    //    QPointF rectcenter = m_RotateRect->mapToScene(m_RotateRect->scenePos().x(),m_RotateRect->scenePos().y());
    //        qDebug()<<"center"<<rectcenter.x()<<rectcenter.y()<<endl;


    //    BarCodeItem h_r = toolBar.value("Bar");
    //    if(h_r.Rect1->isVisible()){
    //        h_r.Rect1->hide();
    //    }
    //    else {
    //        h_r.Rect1->show();
    //    }
    ////    h_r.Rect1->setPos(100,200);
    //    h_r.RotateRect->setPos(200,100);

    //    qreal L1=h_r.Rect1->GetRectLength("Length1");
    //    qreal L2=h_r.Rect1->GetRectLength("Length2");
    //    qDebug()<<"Length1"<<L1<<"Length2"<<L2<<endl;

    //    qreal L3=h_r.RotateRect->GetPolygonLength("Length1");
    //    qreal L4=h_r.RotateRect->GetPolygonLength("Length2");
    //    qDebug()<<"PloyLength1"<<L3<<"Length2"<<L4<<endl;
    //    qreal L1=m_rectangle->GetRectLength("Length1");
    //    qreal L2=m_rectangle->GetRectLength("Length2");
    //    qDebug()<<"Length1"<<L1<<"Length2"<<L2<<endl;
    //    QList<QGraphicsItem *> listScene = m_scene->items();
    //    int num = listScene.count();
    //    qDebug()<<"ItemNum"<<num<<endl;
}

//加载图像
void MainWindow::on_loadBtn_clicked()
{
    QString lastPath="D:/image";
    QDir dir(lastPath);

    QStringList ImageList;

    ImageList << "*.bmp" << "*.jpg" << "*.png";//向字符串列表添加图片类型

    dir.setNameFilters(ImageList);//获得文件夹下图片的名字

    int ImageCount = dir.count();//获得dir里名字的个数，也表示文件夹下图片的个数

    /************获得图片名，对dir进行下标访问即可************/

    for (int i=0;i < ImageCount;i++)

    {
        QString ImageName  = dir[i];
        QString ImagePath = "D:/image/" + ImageName;

        qDebug() << ImagePath;//输出图片名
        QImage img(ImagePath);
        ImageItem->setPixmap(QPixmap::fromImage(img));

        ui->graphicsView->fitInView(ImageItem);
        QTime dieTime = QTime::currentTime().addMSecs(30);
        while(QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }

}

void MainWindow::setBtnEnabled(bool enable)
{
    ui->ArrowBtn->setEnabled(enable);
    ui->ellipseBtn->setEnabled(enable);
    ui->conCircleBtn->setEnabled(enable);
    ui->pieBtn->setEnabled(enable);
    ui->chordBtn->setEnabled(enable);
    ui->squareBtn->setEnabled(enable);
    ui->rectangleBtn->setEnabled(enable);
    ui->RotateRectBtn->setEnabled(enable);
    ui->roundRecBtn->setEnabled(enable);
    ui->rnRecBtn->setEnabled(enable);
    ui->clearBtn->setEnabled(enable);
    ui->saveBtn->setEnabled(enable);
    ui->loadBtn->setEnabled(enable);
}

//旋转矩形
void MainWindow::on_RotateRectBtn_clicked()
{
    m_RotateRect = new BRotateRectangle(0, 0, 400, 500,Qt::blue);
    m_RotateRect->setPos(50,50);
    m_scene->addItem(m_RotateRect);
}

void MainWindow::on_graphicsView_customContextMenuRequested(const QPoint &pos)
{
    QPointF point = ui->graphicsView->mapToScene(pos);
    QTransform transform;
    if(m_scene->itemAt(point,transform) != NULL){
        QGraphicsItem *item = m_scene->itemAt(point,transform);
        if(item->type() == BConcentricCircle::Type)
        {
            BConcentricCircle *rect = qgraphicsitem_cast<BConcentricCircle*>(item);
            qDebug()<<rect->scenePos().x()<<rect->scenePos().y()<<rect->m_radius<<rect->m_another_radius<<endl;
            qDebug()<<item->type()<<point;
            rect->hide();
        }
    }
}

//箭头
void MainWindow::on_ArrowBtn_clicked()
{
    BArrow *m_Arrow = new BArrow(QPointF(0,0),QPointF(100,100));
    m_Arrow->setPos(50,50);
    m_Arrow->setColor(Qt::red);
    m_scene->addItem(m_Arrow);
}

//靶点
void MainWindow::on_TargetBtn_clicked()
{
    BTargetPoint *m_Target = new BTargetPoint(0,0,6);
    m_Target->setPos(100,100);
    m_scene->addItem(m_Target);

}

//交叉点
void MainWindow::on_CrossBtn_clicked()
{
    BCrossPoint *m_cross = new BCrossPoint(0,0,45);
    m_scene->addItem(m_cross);
}
