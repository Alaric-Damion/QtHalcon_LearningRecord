#include "histogram.h"
#include "ui_histogram.h"
#include<Windows.h>
#include<QPainter>
#include "JHCap.h"
#include<QEvent>
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    #include <QScrollBar>
#endif

int Pixel[256];
int RImg[256];
int img_X=0;
int img_Y=0;
unsigned int dead_Pixel[128];
int index=1;

#include <QtGui>

Histogram::Histogram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Histogram)
{
    ui->setupUi(this);
}

Histogram::~Histogram()
{
    delete ui;
}

void Histogram::switchLanguage()
{
    ui->retranslateUi(this);
}

 void Histogram::setImage(QImage image)
 {
    img=image;

    QPixmap pix;
    pix=QPixmap::fromImage(img);
//    if(img.height()>761)
//        ui->scrollArea->setFixedHeight(761);
//    else
//        ui->scrollArea->setFixedHeight(img.height());

    ui->lbImage->setPixmap(pix);
    ui->lbImage->adjustSize();

    index=1;
    unsigned int buffer[128]={0};
    if(buffer[0]<64&&buffer[0]>0)
        index=2*buffer[0]+1;
    if(index>1&&index<128)
    {
        for(int i=0;i<2*buffer[0]+1;i++)
           dead_Pixel[i]=buffer[i];

        for(int i=0;i<buffer[0];i++)
        {
            QString temp=QString("(%1,%2)").arg(buffer[2*i+1]).arg(buffer[2*i+2]);
        }
    }
    Show();
 }

void Histogram::Show()
{
    int  width=0,height=0,max=10,min=250;
    int sum=0;
    width=img.width();
    height=img.height();
    for(int i=0;i<256;i++)
    {
        Pixel[i]=i;
        RImg[i]=0;
    }

    for(int i=0;i<width;i++)
         for(int j=0;j<height;j++)
         {
             QRgb rgb = img.pixel(i, j);
             int g = qGray(qRed(rgb),qGreen(rgb),qBlue(rgb));
             if(g>max)
                 max=g;
             else if(g<min)
                 min=g;
             RImg[Pixel[g]]=RImg[Pixel[g]]+1;
             sum+=g;
         }
    ui->lbMax->setText(QString("%1").arg(max));
    ui->lbMin->setText(QString("%1").arg(min));
    ui->lbWidth->setText(QString("%1").arg(width));
    ui->lbHeight->setText(QString("%1").arg(height));
    ui->lbDepth->setText(QString("%1").arg(img.depth()));
    ui->lbAverage->setText(QString("%1").arg(sum/width/height));
    QPicture picture;

    QPainter painter;
    painter.begin(&picture);
    picture.setBoundingRect(QRect(0,0,280,221)); //!!!!
    painter.drawLine(0,210,255,210 );
    painter.drawText(0,218,"0");
    painter.drawText(255,218,"255");

    for(int i=0;i<256;i++)
    {
         painter.drawLine(QPointF(i,210),QPointF(i,210-RImg[i]/300));
    }
    painter.end();
    ui->lbHistogram->setPicture(picture);
}
void Histogram::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos() - ui->lbImage->pos();
    img_Y=ui->saImage->verticalScrollBar()->value()+ pos.y();
    img_X=ui->saImage->horizontalScrollBar()->value()+pos.x();

    if(img_X<0 || img_Y<0) return;
    // qDebug()<<ui->scrollArea->verticalScrollBar()->value()<<event->y();
    // qDebug()<<ui->scrollArea->horizontalScrollBar()->value()<<event->x();

    QRgb rgb = img.pixel(img_X, img_Y);
    int g=qGray(qRed(rgb),qGreen(rgb),qBlue(rgb));
    ui->lbPosition->setText(QString("(%1,%2)").arg(img_X-1).arg(img_Y-1));
    ui->lbRGB->setText(QString("(R:%1,G:%2,B:%3)").arg(qRed(rgb)).arg(qGreen(rgb)).arg(qBlue(rgb)));
    QImage tempImg=QImage(10,10,QImage::Format_RGB32);

    for(int i=0;i<10;i++)
     {
       for(int j=0;j<10;j++)
          tempImg.setPixel(i,j,img.pixel(img_X-5+i,img_Y-5+j));
     }
    QPixmap pix;
    pix=QPixmap::fromImage(tempImg);
    QPixmap pix1=pix.scaledToHeight(100,Qt::FastTransformation);
    QPixmap pix2=pix1.scaledToWidth(100,Qt::FastTransformation);


    QPicture picture;
    QPainter painter;
    painter.begin(&picture);
    if(g<50)
       painter.setPen(QColor::fromRgb(255,255,255));
    else if(g>100)
       painter.setPen(QColor::fromRgb(0,0,0));

    painter.drawPixmap(0,0,100,100,pix2);
    painter.drawRect(40,40,10,10);
    painter.end();
    ui->lbPick->setPicture(picture);
}

void Histogram::mouseMoveEvent(QMouseEvent  *event)
{
    mousePressEvent(event);
}


