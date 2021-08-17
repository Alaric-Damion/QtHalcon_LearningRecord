#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bqgraphicsitem.h"
#include "bqgraphicsscene.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef struct _BarCodeItem
{
    QGraphicsRectItem* Rect1;
    QGraphicsEllipseItem* RotateRect;

}BarCodeItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setBtnEnabled(bool enable);

private slots:
    void on_circleBtn_clicked();

    void on_ellipseBtn_clicked();

    void on_conCircleBtn_clicked();

    void on_pieBtn_clicked();

    void on_chordBtn_clicked();

    void on_squareBtn_clicked();

    void on_rectangleBtn_clicked();

    void on_polygonBtn_clicked();

    void on_rnRecBtn_clicked();

    void on_roundRecBtn_clicked();

    void on_clearBtn_clicked();

    void on_saveBtn_clicked();

    void on_loadBtn_clicked();

    void on_RotateRectBtn_clicked();



    void on_graphicsView_customContextMenuRequested(const QPoint &pos);

    void on_ArrowBtn_clicked();

    void on_TargetBtn_clicked();

    void on_CrossBtn_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsPixmapItem *ImageItem;
    BQGraphicsScene *m_scene;
    BRotateRectangle *m_RotateRect = NULL;
        BRectangle *m_rectangle;
//    QList<BRectangle*> RectList;
//    BarCodeItem h_Bar;
//    QMap<QString,BarCodeItem> toolBar;
//BConcentricCircle *m_conCircle;
//    QGraphicsItemGroup *ItemGroup;
};
#endif // MAINWINDOW_H
