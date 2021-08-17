#include "bqgraphicsitem.h"
#include <QVector>
#include <QDebug>
#include <QMenu>
#include <QSpinBox>
#include <QWidgetAction>
#include <QtMath>

BGraphicsItem::BGraphicsItem(QPointF center, QPointF edge, ItemType type)
    : m_center(center), m_edge(edge), m_type(type)
{
    m_pen_noSelected.setColor(QColor(0, 160, 230));
    m_pen_noSelected.setWidth(2);
    m_pen_isSelected.setColor(QColor(255, 0, 255));
    m_pen_isSelected.setWidth(2);

    this->setPen(m_pen_noSelected);
    this->setFlags(QGraphicsItem::ItemIsSelectable |
                   QGraphicsItem::ItemIsMovable |
                   QGraphicsItem::ItemIsFocusable);
}

void BGraphicsItem::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    this->setPen(m_pen_isSelected);
}

void BGraphicsItem::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    this->setPen(m_pen_noSelected);
}

//------------------------------------------------------------------------------

BEllipse::BEllipse(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BGraphicsItem(QPointF(x,y), QPointF(x+width/2,y+height/2), type)
{
    BPointItem *point = new BPointItem(this, m_edge, BPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
    m_pointList.setRandColor();
}

QRectF BEllipse::boundingRect() const
{
    return QRectF(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
}

void BEllipse::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
    painter->drawEllipse(ret);
}

void BEllipse::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    QSpinBox* width_spinBox = new QSpinBox(menu);
    width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    width_spinBox->setRange(0, 1000);
    width_spinBox->setPrefix("w: ");
    width_spinBox->setSuffix(" mm");
    width_spinBox->setSingleStep(1);
    width_spinBox->setValue(2 * abs(m_edge.x()));
    connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.x() < 0) {
            m_edge.setX((-1) * v/2);
        } else {
            m_edge.setX(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QSpinBox* height__spinBox = new QSpinBox(menu);
    height__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    height__spinBox->setRange(0, 1000);
    height__spinBox->setPrefix("h: ");
    height__spinBox->setSuffix(" mm");
    height__spinBox->setSingleStep(1);
    height__spinBox->setValue(2 * abs(m_edge.y()));
    connect(height__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.y() < 0) {
            m_edge.setY((-1) * v/2);
        } else {
            m_edge.setY(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* width_widgetAction = new QWidgetAction(menu);
    width_widgetAction->setDefaultWidget(width_spinBox);
    menu->addAction(width_widgetAction);

    QWidgetAction* height_widgetAction = new QWidgetAction(menu);
    height_widgetAction->setDefaultWidget(height__spinBox);
    menu->addAction(height_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------

BCircle::BCircle(qreal x, qreal y, qreal radius, ItemType type)
    : BEllipse(x, y, radius*sqrt(2), radius*sqrt(2), type)
{
    updateRadius();
}

void BCircle::updateRadius()
{
    m_radius = sqrt(pow(m_center.x() - m_edge.x(), 2) + pow(m_center.y() - m_edge.y(), 2));
}

QRectF BCircle::boundingRect() const
{
    return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
    painter->drawEllipse(ret);
}

void BCircle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    QSpinBox* radius_spinBox = new QSpinBox(menu);
    radius_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    radius_spinBox->setRange(0, 1000);
    radius_spinBox->setPrefix("r: ");
    radius_spinBox->setSuffix(" mm");
    radius_spinBox->setSingleStep(1);
    radius_spinBox->setValue(m_radius);
    connect(radius_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        m_radius = v;

        if (m_edge.x() < 0) {
            m_edge.setX(m_center.x() - m_radius * sqrt(2)/2);
        } else {
            m_edge.setX(m_center.x() + m_radius * sqrt(2)/2);
        }

        if (m_edge.y() < 0) {
            m_edge.setY(m_center.y() - m_radius * sqrt(2)/2);
        } else {
            m_edge.setY(m_center.y() + m_radius * sqrt(2)/2);
        }

        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* radius_widgetAction = new QWidgetAction(menu);
    radius_widgetAction->setDefaultWidget(radius_spinBox);
    menu->addAction(radius_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------

BConcentricCircle::BConcentricCircle(qreal x, qreal y, qreal radius1, qreal radius2, ItemType type)
    : BCircle(x, y, radius1, type), m_another_edge(x+radius2*sqrt(2)/2, y+radius2*sqrt(2)/2)
{
    BPointItem *point = new BPointItem(this, m_another_edge, BPointItem::Special);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.setRandColor();

    updateOtherRadius();
}

void BConcentricCircle::updateOtherRadius()
{
    m_another_radius = sqrt(pow(m_center.x() - m_another_edge.x(), 2) +
                            pow(m_center.y() - m_another_edge.y(), 2));
}

void BConcentricCircle::setAnotherEdge(QPointF p)
{
    m_another_edge = p;
}

QRectF BConcentricCircle::boundingRect() const
{
    qreal temp = m_radius > m_another_radius ? m_radius : m_another_radius;
    return QRectF(m_center.x() - temp, m_center.y() - temp, temp * 2, temp * 2);
}

void BConcentricCircle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_another_radius, m_center.y() - m_another_radius, m_another_radius * 2, m_another_radius * 2);
    painter->drawEllipse(ret);

    BCircle::paint(painter, option, widget);
}

void BConcentricCircle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    QSpinBox* radius_spinBox = new QSpinBox(menu);
    radius_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    radius_spinBox->setRange(0, 1000);
    radius_spinBox->setPrefix("r1: ");
    radius_spinBox->setSuffix(" mm");
    radius_spinBox->setSingleStep(1);
    radius_spinBox->setValue(m_radius);
    connect(radius_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        m_radius = v;

        if (m_edge.x() < 0) {
            m_edge.setX(m_center.x() - m_radius * sqrt(2)/2);
        } else {
            m_edge.setX(m_center.x() + m_radius * sqrt(2)/2);
        }

        if (m_edge.y() < 0) {
            m_edge.setY(m_center.y() - m_radius * sqrt(2)/2);
        } else {
            m_edge.setY(m_center.y() + m_radius * sqrt(2)/2);
        }

        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QSpinBox* another_radius_spinBox = new QSpinBox(menu);
    another_radius_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    another_radius_spinBox->setRange(0, 1000);
    another_radius_spinBox->setPrefix("r2: ");
    another_radius_spinBox->setSuffix(" mm");
    another_radius_spinBox->setSingleStep(1);
    another_radius_spinBox->setValue(m_another_radius);
    connect(another_radius_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        m_another_radius = v;

        if (m_another_edge.x() < 0) {
            m_another_edge.setX(m_center.x() - m_another_radius * sqrt(2)/2);
        } else {
            m_another_edge.setX(m_center.x() + m_another_radius * sqrt(2)/2);
        }

        if (m_another_edge.y() < 0) {
            m_another_edge.setY(m_center.y() - m_another_radius * sqrt(2)/2);
        } else {
            m_another_edge.setY(m_center.y() + m_another_radius * sqrt(2)/2);
        }

        m_pointList.at(2)->setPoint(m_another_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* radius_widgetAction = new QWidgetAction(menu);
    radius_widgetAction->setDefaultWidget(radius_spinBox);
    menu->addAction(radius_widgetAction);

    QWidgetAction* another_radius_widgetAction = new QWidgetAction(menu);
    another_radius_widgetAction->setDefaultWidget(another_radius_spinBox);
    menu->addAction(another_radius_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------

BPie::BPie(qreal x, qreal y, qreal radius, qreal angle, ItemType type)
    : BCircle(x, y, radius, type), m_angle(angle)
{
    if ( (angle >= 0 && angle < 90) || (angle >= 270 && angle < 360) )
    {
        m_edge.setX( m_center.x() + radius * cos(angle/180*PI) );
        m_edge.setY( m_center.y() + radius * sin(angle/180*PI) * (-1) );
    }
    else if ( (angle >= 90 && angle < 270) )
    {
        m_edge.setY( m_center.y() + radius * sin(angle/180*PI) * (-1) );
        m_edge.setX( m_center.x() + radius * cos(angle/180*PI) );
    }

    m_pointList.at(0)->setPoint(m_edge);
    m_radius = radius;
}

void BPie::updateAngle()
{
    qreal dx = m_edge.x() - m_center.x();
    qreal dy = m_edge.y() - m_center.y();

    if ( dx >= 0 && dy < 0 )
    {
        m_angle = atan2( (-1)*(dy), dx ) *180/PI;
    }
    else if ( dx < 0 && dy < 0 )
    {
        m_angle = atan2( (-1)*dy, dx ) *180/PI;
    }
    else if ( dx < 0 && dy >= 0 )
    {
        m_angle = 360 + atan2( (-1)*dy, dx ) *180/PI;
    }
    else if ( dx >= 0 && dy >= 0 )
    {
        m_angle = 360 - atan2( dy, dx ) *180/PI;
    }
}

void BPie::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
    painter->drawPie(ret, 16*0, 16*(m_angle));
}

//------------------------------------------------------------------------------

BChord::BChord(qreal x, qreal y, qreal radius, qreal angle, ItemType type)
    : BPie(x, y, radius, angle, type)
{
    updateEndAngle();
}

void BChord::updateEndAngle()
{
    qreal dx = m_edge.x() - m_center.x();
    qreal dy = m_edge.y() - m_center.y();

    if ( dx >= 0 && dy < 0 )
    {
        m_angle = atan2( (-1)*(dy), dx ) *180/PI;
    }
    else if ( dx < 0 && dy < 0 )
    {
        m_angle = 180 - atan2( (-1)*dy, dx ) *180/PI;
    }
    else if ( dx < 0 && dy >= 0 )
    {
        m_angle = 360 + atan2( (-1)*dy, dx ) *180/PI;
    }
    else if ( dx >= 0 && dy >= 0 )
    {
        m_angle = atan2( (-1)*dy, dx ) *180/PI;
    }

    m_end_angle = 180 - m_angle*2;
}

void BChord::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
    painter->drawChord(ret, 16*(m_angle), 16*(m_end_angle));
}

//------------------------------------------------------------------------------

BRectangle::BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BGraphicsItem(QPointF(x,y), QPointF(width/2,height/2), type)
{
    BPointItem *point = new BPointItem(this, m_edge, BPointItem::Edge);
    point->setParentItem(this);
    m_pointList.append(point);
    m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
    m_pointList.setRandColor();

}
//获取矩形的长边和短边尺寸
qreal BRectangle::GetRectLength(QString type)
{
    qreal Rect_Length1,Rect_Length2;
    Rect_Length1 = abs(m_edge.x()-m_center.x()) * 2;
    Rect_Length2 = abs(m_edge.y()-m_center.y()) * 2;

    if(type == "Length1") return Rect_Length1;
    else if(type == "Length2") return Rect_Length2;
}

void BRectangle::SetRect(qreal x, qreal y, qreal width, qreal height)
{

    m_center.setX(x);
    m_center.setY(y);
    m_edge.setX(width/2);
    m_edge.setY(height/2);
    m_pointList.at(0)->setPoint(m_edge);
    this->hide();
    this->update();
    this->show();
    qDebug()<<m_center<<m_edge<<endl;
}
QRectF BRectangle::boundingRect() const
{
    return QRectF(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
}

void BRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
//    painter->setPen(this->pen());
//    painter->setBrush(this->brush());
    QPen mPen = QPen(Qt::blue,4);
    painter->setPen(mPen);

    QRectF ret(m_center.x() - abs(m_edge.x()), m_center.y() - abs(m_edge.y()), abs(m_edge.x()) * 2, abs(m_edge.y()) * 2);
    painter->drawRect(ret);
}

void BRectangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    QSpinBox* width_spinBox = new QSpinBox(menu);
    width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    width_spinBox->setRange(0, 1000);
    width_spinBox->setPrefix("w: ");
    width_spinBox->setSuffix(" mm");
    width_spinBox->setSingleStep(1);
    width_spinBox->setValue(2 * abs(m_edge.x()));
    connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.x() < 0) {
            m_edge.setX((-1) * v/2);
        } else {
            m_edge.setX(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QSpinBox* height__spinBox = new QSpinBox(menu);
    height__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    height__spinBox->setRange(0, 1000);
    height__spinBox->setPrefix("h: ");
    height__spinBox->setSuffix(" mm");
    height__spinBox->setSingleStep(1);
    height__spinBox->setValue(2 * abs(m_edge.y()));
    connect(height__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.y() < 0) {
            m_edge.setY((-1) * v/2);
        } else {
            m_edge.setY(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* width_widgetAction = new QWidgetAction(menu);
    width_widgetAction->setDefaultWidget(width_spinBox);
    menu->addAction(width_widgetAction);

    QWidgetAction* height_widgetAction = new QWidgetAction(menu);
    height_widgetAction->setDefaultWidget(height__spinBox);
    menu->addAction(height_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------

BRotateRectangle::BRotateRectangle(qreal x, qreal y, qreal width, qreal height)
{
    m_oldRect = QRectF(x,y,width,height);
    m_RotateAngle = 0;
    setRectSize(m_oldRect);
    m_StateFlag=DEFAULT_FLAG;
    setCursor(Qt::ArrowCursor);
    isAddArrow = false;
//    BPointItem *point = new BPointItem(this, m_edge, BPointItem::Edge);
//    point->setParentItem(this);
//    m_pointList.append(point);
//    m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
//    m_pointList.setRandColor();

}

BRotateRectangle::BRotateRectangle(qreal x, qreal y, qreal width, qreal height,QColor color)
{
    isAddArrow = true;
    m_oldRect = QRectF(x,y,width,height);
    m_RotateAngle = 0;
    setRectSize(m_oldRect);
    m_StateFlag=DEFAULT_FLAG;
    setCursor(Qt::ArrowCursor);
    ArrowColor = color;
}

QRectF BRotateRectangle::boundingRect() const
{
    //    return QRectF(m_center.x() - abs(m_edge.x()), m_center.y() - 40, abs(m_edge.x()) * 2+80, abs(m_edge.y()) * 2+80);
    //    return QRectF(m_center.x() - abs(m_edge.x()-20), m_center.y() - 10, abs(m_edge.x()) * 2+40, abs(m_edge.y()) * 2+40);
    //    return QRectF(m_center.x() - abs(m_edge.x()), m_center.y() - 10, abs(m_edge.x()) * 2+80, abs(m_edge.y())+80);
    QRectF boundingRectF = m_oldRectPolygon.boundingRect();
    return QRectF(boundingRectF.x()-40,boundingRectF.y()-40,boundingRectF.width()+80,boundingRectF.height()+80);
}

void BRotateRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen mPen = QPen(Qt::red,4);
    painter->setPen(mPen);

    //绘制旋转后的矩形
    //    QRectF rect(m_center.x() - abs(m_edge.x()), m_center.y() +10, abs(m_edge.x()) * 2, abs(m_edge.y())-10);
    //    painter->drawRect(rect);
    painter->drawPolygon(m_oldRectPolygon);

    if(isAddArrow)
    {
        QPen mPen = QPen(ArrowColor,2);
        painter->setPen(mPen);
        painter->drawLine(startP,endP);
    }

    //绘制旋转圆形
    mPen.setWidth(2);
    mPen.setColor(Qt::green);
    painter->setPen(mPen);
    QPointF pf = getSmallRotateRectCenter(m_oldRectPolygon[0],m_oldRectPolygon[1]);
    QRectF rect = QRectF(pf.x()-10,pf.y()-10,20,20);
    painter->drawEllipse(rect);//绘制圆形
    painter->drawPoint(pf);//绘制点


}

void BRotateRectangle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标，
        if(m_SmallRotatePolygon.containsPoint(m_startPos,Qt::WindingFill))//旋转矩形
        {
            setCursor(Qt::PointingHandCursor);
            m_StateFlag = ROTATE;
        }
        else if(m_insicedPolygon.containsPoint(m_startPos,Qt::WindingFill))//在矩形内框区域时按下鼠标，则可拖动图片
        {
            setCursor(Qt::ClosedHandCursor);   //改变光标形状,手的形状
            m_StateFlag = MOV_RECT;//标记当前为鼠标拖动图片移动状态
        }
        else if(m_leftPolygon.containsPoint(m_startPos,Qt::WindingFill))
        {
            setCursor(Qt::SizeHorCursor);
            m_StateFlag = MOV_LEFT_LINE;//标记当前为用户按下矩形的左边界区域
        }
        else if(m_rightPolygon.containsPoint(m_startPos,Qt::WindingFill))
        {
            setCursor(Qt::SizeHorCursor);
            m_StateFlag = MOV_RIGHT_LINE;//标记当前为用户按下矩形的右边界区域
        }
        else if(m_topPolygon.containsPoint(m_startPos,Qt::WindingFill))
        {
            setCursor(Qt::SizeVerCursor);
            m_StateFlag = MOV_TOP_LINE;//标记当前为用户按下矩形的上边界区域
        }
        else if(m_bottomPolygon.containsPoint(m_startPos,Qt::WindingFill))
        {
            setCursor(Qt::SizeVerCursor);
            m_StateFlag = MOV_BOTTOM_LINE;//标记当前为用户按下矩形的下边界区域
        }
//        else if(m_rbPolygon.containsPoint(m_startPos,Qt::WindingFill))
//        {
//            setCursor(Qt::SizeFDiagCursor);
//            m_StateFlag = MOV_RIGHTBOTTOM_RECT;//标记当前为用户按下矩形的右下角
//        }
        else
        {
            m_StateFlag = DEFAULT_FLAG;
        }
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}

void BRotateRectangle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_StateFlag == ROTATE)
    {
       int nRotateAngle = atan2((event->pos().x()-m_RotateCenter.x()),(event->pos().y()-m_RotateCenter.y()))*180/M_PI;
       SetRotate(180-nRotateAngle);
       setRectSize(m_oldRect);
       //qDebug()<<nRotateAngle;
    }
    else if(m_StateFlag == MOV_RECT)
    {
        QPointF point = (event->pos() - m_startPos);
        moveBy(point.x(), point.y());
        setRectSize(m_oldRect);
        scene()->update();
    }
    else if(m_StateFlag == MOV_LEFT_LINE)
    {
        QPointF pf = QPointF((m_oldRectPolygon.at(1).x()+m_oldRectPolygon.at(2).x())/2,((m_oldRectPolygon.at(1).y()+m_oldRectPolygon.at(2).y())/2));
        //计算到右侧边中点的距离
        qreal dis = sqrt((event->pos().x()-pf.x())*(event->pos().x()-pf.x()) +(event->pos().y()-pf.y())*(event->pos().y()-pf.y()));
        qreal dis2LT = sqrt((event->pos().x()-m_oldRectPolygon.at(0).x())*(event->pos().x()-m_oldRectPolygon.at(0).x()) +(event->pos().y()-m_oldRectPolygon.at(0).y())*(event->pos().y()-m_oldRectPolygon.at(0).y()));
        qreal dis2RT = sqrt((event->pos().x()-m_oldRectPolygon.at(1).x())*(event->pos().x()-m_oldRectPolygon.at(1).x()) +(event->pos().y()-m_oldRectPolygon.at(1).y())*(event->pos().y()-m_oldRectPolygon.at(1).y()));
        if(dis<16||dis2LT>dis2RT)
        {
            return;
        }
        else
        {
            QRectF newRect(m_oldRect);
            newRect.setLeft(m_oldRect.right()-dis);
            newRect.setRight(m_oldRect.right());
            setRectSize(newRect,false);
            m_RotateCenter=QPointF((m_oldRectPolygon.at(0).x()+m_oldRectPolygon.at(2).x())/2,(m_oldRectPolygon.at(0).y()+m_oldRectPolygon.at(2).y())/2);
            m_oldRect.moveCenter(m_RotateCenter);
            setRectSize(m_oldRect);
            scene()->update();//必须要用scene()->update()，不能用update();否则会出现重影
        }
    }
    else if(m_StateFlag == MOV_TOP_LINE)
    {
        //底边中点
        QPointF pf = QPointF((m_oldRectPolygon.at(2).x()+m_oldRectPolygon.at(3).x())/2,((m_oldRectPolygon.at(2).y()+m_oldRectPolygon.at(3).y())/2));
        //计算到底边中点的距离
        qreal dis = sqrt((event->pos().x()-pf.x())*(event->pos().x()-pf.x()) +(event->pos().y()-pf.y())*(event->pos().y()-pf.y()));
        qreal dis2LT = sqrt((event->pos().x()-m_oldRectPolygon.at(0).x())*(event->pos().x()-m_oldRectPolygon.at(0).x()) +(event->pos().y()-m_oldRectPolygon.at(0).y())*(event->pos().y()-m_oldRectPolygon.at(0).y()));
        qreal dis2LB = sqrt((event->pos().x()-m_oldRectPolygon.at(3).x())*(event->pos().x()-m_oldRectPolygon.at(3).x()) +(event->pos().y()-m_oldRectPolygon.at(3).y())*(event->pos().y()-m_oldRectPolygon.at(3).y()));
        if(dis<16||dis2LT>dis2LB)
        {
            return;
        }
        else
        {
            QRectF newRect(m_oldRect);
            newRect.setTop(m_oldRect.bottom()-dis);
            newRect.setBottom(m_oldRect.bottom());
            setRectSize(newRect,false);
            m_RotateCenter=QPointF((m_oldRectPolygon.at(0).x()+m_oldRectPolygon.at(2).x())/2,(m_oldRectPolygon.at(0).y()+m_oldRectPolygon.at(2).y())/2);
            m_oldRect.moveCenter(m_RotateCenter);
            setRectSize(m_oldRect);
            scene()->update();//必须要用scene()->update()，不能用update();否则会出现重影
        }
    }
    else if(m_StateFlag == MOV_RIGHT_LINE)
    {
        QPointF pf = QPointF((m_oldRectPolygon.at(0).x()+m_oldRectPolygon.at(3).x())/2,((m_oldRectPolygon.at(0).y()+m_oldRectPolygon.at(3).y())/2));
        //计算到左侧边中点的距离
        qreal dis = sqrt((event->pos().x()-pf.x())*(event->pos().x()-pf.x()) +(event->pos().y()-pf.y())*(event->pos().y()-pf.y()));
        qreal dis2LT = sqrt((event->pos().x()-m_oldRectPolygon.at(0).x())*(event->pos().x()-m_oldRectPolygon.at(0).x()) +(event->pos().y()-m_oldRectPolygon.at(0).y())*(event->pos().y()-m_oldRectPolygon.at(0).y()));
        qreal dis2RT = sqrt((event->pos().x()-m_oldRectPolygon.at(1).x())*(event->pos().x()-m_oldRectPolygon.at(1).x()) +(event->pos().y()-m_oldRectPolygon.at(1).y())*(event->pos().y()-m_oldRectPolygon.at(1).y()));
        if(dis<16||dis2LT<dis2RT)
        {
            return;
        }
        else
        {
            QRectF newRect(m_oldRect);
            newRect.setLeft(m_oldRect.left());
            newRect.setRight(m_oldRect.left()+dis);
            setRectSize(newRect,false);
            m_RotateCenter=QPointF((m_oldRectPolygon.at(0).x()+m_oldRectPolygon.at(2).x())/2,(m_oldRectPolygon.at(0).y()+m_oldRectPolygon.at(2).y())/2);
            m_oldRect.moveCenter(m_RotateCenter);
            setRectSize(m_oldRect);
            scene()->update();//必须要用scene()->update()，不能用update();否则会出现重影
        }
    }
    else if(m_StateFlag == MOV_BOTTOM_LINE)
    {
        //顶边中点
        QPointF pf = QPointF((m_oldRectPolygon.at(0).x()+m_oldRectPolygon.at(1).x())/2,((m_oldRectPolygon.at(0).y()+m_oldRectPolygon.at(1).y())/2));
        //计算到底边中点的距离
        qreal dis = sqrt((event->pos().x()-pf.x())*(event->pos().x()-pf.x()) +(event->pos().y()-pf.y())*(event->pos().y()-pf.y()));
        qreal dis2LT = sqrt((event->pos().x()-m_oldRectPolygon.at(0).x())*(event->pos().x()-m_oldRectPolygon.at(0).x()) +(event->pos().y()-m_oldRectPolygon.at(0).y())*(event->pos().y()-m_oldRectPolygon.at(0).y()));
        qreal dis2LB = sqrt((event->pos().x()-m_oldRectPolygon.at(3).x())*(event->pos().x()-m_oldRectPolygon.at(3).x()) +(event->pos().y()-m_oldRectPolygon.at(3).y())*(event->pos().y()-m_oldRectPolygon.at(3).y()));
        if(dis<16||dis2LT<dis2LB)
        {
            return;
        }
        else
        {
            QRectF newRect(m_oldRect);
            newRect.setTop(m_oldRect.top());
            newRect.setBottom(m_oldRect.top()+dis);
            setRectSize(newRect,false);
            m_RotateCenter=QPointF((m_oldRectPolygon.at(0).x()+m_oldRectPolygon.at(2).x())/2,(m_oldRectPolygon.at(0).y()+m_oldRectPolygon.at(2).y())/2);
            m_oldRect.moveCenter(m_RotateCenter);
            setRectSize(m_oldRect);
            scene()->update();//必须要用scene()->update()，不能用update();否则会出现重影
        }
    }
}

void BRotateRectangle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qreal length1 = GetPolygonLength("Length1");
    qreal length2 = GetPolygonLength("Length2");
    qDebug()<<"Length1"<<length1<<"Length2"<<length2<<endl;
    qDebug()<<"Angle"<<m_RotateAngle<<endl;
    QPointF CenterScene = mapToScene(m_RotateCenter);
            qDebug()<<"RotateCenter"<<m_RotateCenter.x()<<m_RotateCenter.y()<<endl;
            qDebug()<<"CenterScene"<<CenterScene.x()<<CenterScene.y()<<endl;
    setCursor(Qt::ArrowCursor);
    if(m_StateFlag == MOV_RECT)
    {
        m_StateFlag = DEFAULT_FLAG;
    }
    else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

//获取旋转矩形的长边和短边尺寸
qreal BRotateRectangle::GetPolygonLength(QString type)
{
    qreal Ploy_Length1,Ploy_Length2;
    qreal dis_x0 = m_oldRectPolygon.at(1).x() - m_oldRectPolygon.at(0).x();
    qreal dis_y0 = m_oldRectPolygon.at(1).y() - m_oldRectPolygon.at(0).y();
    Ploy_Length1 = sqrt(dis_x0 * dis_x0 + dis_y0*dis_y0);
    qreal dis_x1 = m_oldRectPolygon.at(2).x() - m_oldRectPolygon.at(1).x();
    qreal dis_y1= m_oldRectPolygon.at(2).y() - m_oldRectPolygon.at(1).y();
    Ploy_Length2 = sqrt(dis_x1*dis_x1 + dis_y1*dis_y1);

    if(type == "Length1") return Ploy_Length1;
    else if(type == "Length2") return Ploy_Length2;

}
void BRotateRectangle::setRectSize(QRectF mrect, bool bResetRotateCenter)
{
    m_oldRect = mrect;
    if(bResetRotateCenter)
    {
        m_RotateCenter.setX(m_oldRect.x()+m_oldRect.width()/2);
        m_RotateCenter.setY(m_oldRect.y()+m_oldRect.height()/2);
    }
    m_oldRectPolygon = getRotatePolygonFromRect(m_RotateCenter,m_oldRect,m_RotateAngle);


    m_insicedRectf = QRectF(m_oldRect.x()+8,m_oldRect.y()+8,m_oldRect.width()-16,m_oldRect.height()-16);
    m_insicedPolygon =getRotatePolygonFromRect(m_RotateCenter,m_insicedRectf,m_RotateAngle);

    m_leftRectf = QRectF(m_oldRect.x(),m_oldRect.y(),8,m_oldRect.height()-8);
    m_leftPolygon = getRotatePolygonFromRect(m_RotateCenter,m_leftRectf,m_RotateAngle);

    m_topRectf = QRectF(m_oldRect.x()+8,m_oldRect.y(),m_oldRect.width()-8,8);
    m_topPolygon = getRotatePolygonFromRect(m_RotateCenter,m_topRectf,m_RotateAngle);

    m_rightRectf = QRectF(m_oldRect.right()-8,m_oldRect.y()+8,8,m_oldRect.height()-16);
    m_rightPolygon = getRotatePolygonFromRect(m_RotateCenter,m_rightRectf,m_RotateAngle);

    m_bottomRectf = QRectF(m_oldRect.x(),m_oldRect.bottom()-8,m_oldRect.width()-8,8);
    m_bottomPolygon = getRotatePolygonFromRect(m_RotateCenter,m_bottomRectf,m_RotateAngle);

    m_SmallRotateRect = getSmallRotateRect(mrect.topLeft(),mrect.topRight());//矩形正上方的旋转标记矩形
    m_SmallRotatePolygon = getRotatePolygonFromRect(m_RotateCenter,m_SmallRotateRect,m_RotateAngle);
}

QPointF BRotateRectangle::UpdateRotateRectAngle()
{
    setRectSize(m_oldRect);
    QPointF pf = getSmallRotateRectCenter(m_oldRectPolygon[0],m_oldRectPolygon[1]);
    return pf;
}

QPointF BRotateRectangle::getRectEdgePoint()
{
    QPolygonF polygon = getRotatePolygonFromRect(m_RotateCenter,m_oldRect,m_RotateAngle);
    QPointF lastEdgePoint = polygon[2];
    return lastEdgePoint;
}
void BRotateRectangle::SetRotate(qreal RotateAngle, QPointF ptCenter)
{
    //    m_bRotate = true;
    if(ptCenter.x()==-999 && ptCenter.y()==-999)
    {
        m_RotateCenter = QPointF(m_oldRect.x()+m_oldRect.width()/2,m_oldRect.y()+m_oldRect.height()/2);
    }
    else
    {
        m_RotateCenter = ptCenter;
    }
    m_RotateAngle = RotateAngle;
    this->update();
}

QPolygonF BRotateRectangle::getRotatePolygonFromRect(QPointF ptCenter, QRectF rectIn, qreal angle)
{
    QVector<QPointF> vpt;
    QPointF pf = getRotatePoint(ptCenter,rectIn.topLeft(),angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter,rectIn.topRight(),angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter,rectIn.bottomRight(),angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter,rectIn.bottomLeft(),angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter,rectIn.topLeft(),angle);
    vpt.append(pf);
    return QPolygonF(vpt);

}

QPointF BRotateRectangle::getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle)
{
    double dx = ptCenter.x();
    double dy = ptCenter.y();
    double x = ptIn.x();
    double y = ptIn.y();
    double xx,yy;
    xx = (x-dx)*cos(angle*M_PI/180)-(y-dy)*sin(angle*M_PI/180)+dx;
    yy = (x-dx)*sin(angle*M_PI/180)+(y-dy)*cos(angle*M_PI/180)+dy;

    return QPointF(xx,yy);
}

QPointF BRotateRectangle::getSmallRotateRectCenter(QPointF ptA,QPointF ptB)
{
    QPointF ptCenter = QPointF((ptA.x()+ptB.x())/2,(ptA.y()+ptB.y())/2);//A,B点的中点C
    //中垂线方程式为 y=x*k + b;
    qreal x,y;//旋转图标矩形的中心
    if(abs(ptB.y()-ptA.y())<0.1)
    {
        if(ptA.x()<ptB.x())//矩形左上角在上方
        {
            x = ptCenter.x();
            y = ptCenter.y()-20;
        }
        else//矩形左上角在下方
        {
            x = ptCenter.x();
            y = ptCenter.y()+20;
        }
    }
    else if(ptB.y()>ptA.y())//顺时针旋转0-180
    {
        qreal k = (ptA.x()-ptB.x())/(ptB.y()-ptA.y());//中垂线斜率
        qreal b = (ptA.y()+ptB.y())/2-k*(ptA.x()+ptB.x())/2;
        //求AB线中垂线上离AB中点20个像素的点C的坐标
        x = 20*cos(atan(k))+ptCenter.x();
        y = k*x+b;
    }
    else if(ptB.y()<ptA.y())//顺时针旋转180-360
    {
        qreal k = (ptA.x()-ptB.x())/(ptB.y()-ptA.y());//中垂线斜率
        qreal b = (ptA.y()+ptB.y())/2-k*(ptA.x()+ptB.x())/2;
        //求AB线中垂线上离AB中点20个像素的点C的坐标
        x = -20*cos(atan(k))+ptCenter.x();
        y = k*x+b;
    }
    return QPointF(x,y);
}


QRectF BRotateRectangle::getSmallRotateRect(QPointF ptA,QPointF ptB)
{
    QPointF pt = getSmallRotateRectCenter(ptA,ptB);
    return QRectF(pt.x()-10,pt.y()-10,20,20);
}



void BRotateRectangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
//    if ( !this->isSelected() )
//        return;

//    QMenu* menu = new QMenu();
//    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

//    QSpinBox* width_spinBox = new QSpinBox(menu);
//    width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
//    width_spinBox->setRange(0, 1000);
//    width_spinBox->setPrefix("w: ");
//    width_spinBox->setSuffix(" mm");
//    width_spinBox->setSingleStep(1);
//    width_spinBox->setValue(2 * abs(m_edge.x()));
//    connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
//        if (m_edge.x() < 0) {
//            m_edge.setX((-1) * v/2);
//        } else {
//            m_edge.setX(v/2);
//        }
//        m_pointList.at(0)->setPoint(m_edge);
//        this->hide();
//        this->update();
//        this->show();
//    });

//    QSpinBox* height__spinBox = new QSpinBox(menu);
//    height__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
//    height__spinBox->setRange(0, 1000);
//    height__spinBox->setPrefix("h: ");
//    height__spinBox->setSuffix(" mm");
//    height__spinBox->setSingleStep(1);
//    height__spinBox->setValue(2 * abs(m_edge.y()));
//    connect(height__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
//        if (m_edge.y() < 0) {
//            m_edge.setY((-1) * v/2);
//        } else {
//            m_edge.setY(v/2);
//        }
//        m_pointList.at(0)->setPoint(m_edge);
//        this->hide();
//        this->update();
//        this->show();
//    });

//    //    QSpinBox* angle__spinBox = new QSpinBox(menu);
//    //    angle__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
//    //    angle__spinBox->setRange(0, 360);
//    //    angle__spinBox->setPrefix("A: ");
//    //    angle__spinBox->setSuffix(" c");
//    //    angle__spinBox->setSingleStep(1);
//    //    angle__spinBox->setValue(2 * abs(m_edge.y()));
//    //    connect(height__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
//    //        if (m_edge.y() < 0) {
//    //            m_edge.setY((-1) * v/2);
//    //        } else {
//    //            m_edge.setY(v/2);
//    //        }
//    //        m_pointList.at(0)->setPoint(m_edge);
//    //        this->hide();
//    //        this->update();
//    //        this->show();
//    //    });

//    QWidgetAction* width_widgetAction = new QWidgetAction(menu);
//    width_widgetAction->setDefaultWidget(width_spinBox);
//    menu->addAction(width_widgetAction);

//    QWidgetAction* height_widgetAction = new QWidgetAction(menu);
//    height_widgetAction->setDefaultWidget(height__spinBox);
//    menu->addAction(height_widgetAction);

//    menu->exec(QCursor::pos());
//    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------

BSquare::BSquare(qreal x, qreal y, qreal width, ItemType type)
    : BRectangle(x, y, width, width, type) {}

void BSquare::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    QSpinBox* distance_spinBox = new QSpinBox(menu);
    distance_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    distance_spinBox->setRange(0, 1000);
    distance_spinBox->setPrefix("d: ");
    distance_spinBox->setSuffix(" mm");
    distance_spinBox->setSingleStep(1);
    distance_spinBox->setValue(2 * abs(m_edge.x()));
    connect(distance_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_edge.x() < 0) {
            m_edge.setX((-1) * v/2);
            m_edge.setY((-1) * v/2);
        } else {
            m_edge.setX(v/2);
            m_edge.setY(v/2);
        }
        m_pointList.at(0)->setPoint(m_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* distance_widgetAction = new QWidgetAction(menu);
    distance_widgetAction->setDefaultWidget(distance_spinBox);
    menu->addAction(distance_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------

BPolygon::BPolygon(ItemType type)
    : BGraphicsItem(QPointF(0,0), QPointF(0,0), type)
{
    is_create_finished = false;
}

void BPolygon::pushPoint(QPointF p, QList<QPointF> list, bool isCenter)
{
    if (!is_create_finished) {
        m_center = getCentroid(list);
        getMaxLength();

        if (isCenter) {
            m_pointList.append(new BPointItem(this, m_center, BPointItem::Center));
            m_pointList.setRandColor();
            is_create_finished = true;
        } else {
            BPointItem *point = new BPointItem(this, p, BPointItem::Edge);
            point->setParentItem(this);
            m_pointList.append(point);
            m_pointList.setColor(QColor(0, 255, 0));
        }

        this->update();
    }
}

QPointF BPolygon::getCentroid(QList<QPointF> list)
{
    qreal x = 0;
    qreal y = 0;
    for (auto &temp : list)
    {
        x += temp.x();
        y += temp.y();
    }
    x = x/list.size();
    y = y/list.size();
    return QPointF(x,y);
}

void BPolygon::getMaxLength()
{
    QVector<qreal> vec;
    for (auto &temp : m_pointList)
    {
        qreal dis = sqrt(pow(m_center.x() - temp->x(), 2) + pow(m_center.y() - temp->y(), 2));
        vec.append(dis);
    }

    qreal ret = 0;
    for (auto &temp : vec)
    {
        if (temp > ret) {
            ret = temp;
        }
    }
    m_radius = ret;
}

void BPolygon::updatePolygon(QPointF origin, QPointF end)
{
    QList<QPointF> list;

    for (auto &temp : m_pointList) {
        if (temp->getPoint() == origin) {
            temp->setPoint(end);
        }
        list.append(temp->getPoint());
    }

    m_center = getCentroid(list);
    m_pointList.at(m_pointList.size()-1)->setPoint(m_center);

    getMaxLength();
}

QRectF BPolygon::boundingRect() const
{
    return QRectF(m_center.x() - m_radius, m_center.y() - m_radius, m_radius * 2, m_radius * 2);
}

void BPolygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    if (is_create_finished) {
        for (int i = 1; i < m_pointList.size() - 1; i++)
        {
            painter->drawLine(m_pointList.at(i-1)->getPoint(), m_pointList.at(i)->getPoint());
        }

        painter->drawLine(m_pointList.at(m_pointList.size()-2)->getPoint(), m_pointList.at(0)->getPoint());
    } else {
        for (int i = 1; i < m_pointList.size(); i++)
        {
            painter->drawLine(m_pointList.at(i-1)->getPoint(), m_pointList.at(i)->getPoint());
        }
    }
}

//------------------------------------------------------------------------------

BRound_End_Rectangle::BRound_End_Rectangle(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BRectangle(x, y, width, height, type) {}

QRectF BRound_End_Rectangle::boundingRect() const
{
    QRectF ret = QRectF(m_center.x() - m_edge.x() - m_edge.y(),
                        m_center.y() - m_edge.y(),
                        abs(m_edge.x()) * 2 + abs(m_edge.y()) * 2,
                        abs(m_edge.y()) * 2);

    if ( m_edge.x() >= 0 && m_edge.y() < 0 )
    {
        ret.moveTo(m_center.x() - m_edge.x() + m_edge.y(), m_center.y() + m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() < 0 )
    {
        ret.moveTo(m_center.x() + m_edge.x() + m_edge.y(), m_center.y() + m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() >= 0 )
    {
        ret.moveTo(m_center.x() + m_edge.x() - m_edge.y(), m_center.y() - m_edge.y());
    }
    else if ( m_edge.x() >= 0 && m_edge.y() >=0 )
    {
        ret.moveTo(m_center.x() - m_edge.x() - m_edge.y(), m_center.y() - m_edge.y());
    }

    return ret;
}

void BRound_End_Rectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QPointF left_top, left_bottom, right_top, right_bottom;

    if ( m_edge.x() >= 0 && m_edge.y() < 0 )
    {
        left_top = QPointF(m_center.x() - m_edge.x() + m_edge.y(), m_edge.y());
        left_bottom = QPointF(m_center.x() - m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_top = QPointF(m_center.x() + m_edge.x() - m_edge.y(), m_edge.y());
        right_bottom = QPointF(m_center.x() + m_edge.x() - m_edge.y(), (-1) * m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() < 0 )
    {
        left_top = QPointF(m_center.x() + m_edge.x() + m_edge.y(), m_edge.y());
        left_bottom = QPointF(m_center.x() + m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_top = QPointF(m_center.x() - m_edge.x() - m_edge.y(), m_edge.y());
        right_bottom = QPointF(m_center.x() - m_edge.x() - m_edge.y(), (-1) * m_edge.y());
    }
    else if ( m_edge.x() < 0 && m_edge.y() >= 0 )
    {
        left_top = QPointF(m_center.x() + m_edge.x() - m_edge.y(), (-1) * m_edge.y());
        left_bottom = QPointF(m_center.x() + m_edge.x() - m_edge.y(), m_edge.y());
        right_top = QPointF(m_center.x() - m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_bottom = QPointF(m_center.x() - m_edge.x() + m_edge.y(), m_edge.y());
    }
    else if ( m_edge.x() >= 0 && m_edge.y() >=0 )
    {
        left_top = QPointF(m_center.x() - m_edge.x() - m_edge.y(), (-1) * m_edge.y());
        left_bottom = QPointF(m_center.x() - m_edge.x() - m_edge.y(), m_edge.y());
        right_top = QPointF(m_center.x() + m_edge.x() + m_edge.y(), (-1) * m_edge.y());
        right_bottom = QPointF(m_center.x() + m_edge.x() + m_edge.y(), m_edge.y());
    }

    int radius = abs(m_edge.y());
    QPointF deltax(radius, 0);
    QPointF deltay(0, radius);

    painter->drawLine(left_top + deltax, right_top - deltax);
    painter->drawLine(left_bottom + deltax, right_bottom - deltax);
    painter->drawLine(left_top + deltay, left_bottom - deltay);
    painter->drawLine(right_top + deltay, right_bottom - deltay);

    painter->drawArc(QRectF(left_top, QSizeF(radius*2, radius*2)), -180 * 16, -90 * 16);
    painter->drawArc(QRectF(left_bottom-deltay*2, QSizeF(radius*2, radius*2)), -90 * 16, -90 * 16);
    painter->drawArc(QRectF(right_top-deltax*2, QSizeF(radius*2, radius*2)), 0 * 16, 90 * 16);
    painter->drawArc(QRectF(right_bottom-deltax*2-deltay*2, QSizeF(radius*2, radius*2)), 0 * 16, -90 * 16);
}

//------------------------------------------------------------------------------

BRounded_Rectangle::BRounded_Rectangle(qreal x, qreal y, qreal width, qreal height, ItemType type)
    : BRectangle(x, y, width, height, type)
{
    m_another_edge.setX( m_edge.x() );
    m_another_edge.setY( (-1) * m_edge.y() );

    BPointItem *point = new BPointItem(this, m_another_edge, BPointItem::Special);
    point->setParentItem(this);
    point->setCursor(Qt::SizeAllCursor);
    m_pointList.append(point);
    m_pointList.setRandColor();

    updateRadius();
}

void BRounded_Rectangle::updateRadius()
{
    qreal dx = 0;
    qreal dy = 0;
    qreal absX = abs(m_edge.x());
    qreal absY = abs(m_edge.y());

    if ( m_another_edge.x() >= 0 && m_another_edge.y() < 0 )
    {
        dx = absX - m_another_edge.x();
        dy = absY + m_another_edge.y();
    }
    else if ( m_another_edge.x() < 0 && m_another_edge.y() < 0 )
    {
        dx = absX + m_another_edge.x();
        dy = absY + m_another_edge.y();
    }
    else if ( m_another_edge.x() < 0 && m_another_edge.y() >= 0 )
    {
        dx = absX + m_another_edge.x();
        dy = absY - m_another_edge.y();
    }
    else if ( m_another_edge.x() >= 0 && m_another_edge.y() >= 0 )
    {
        dx = absX - m_another_edge.x();
        dy = absY - m_another_edge.y();
    }

    m_radius = dx >= dy ? dx : dy;
}

void BRounded_Rectangle::updateAnotherEdge(QPointF p)
{
    qreal retX = 0;
    qreal retY = 0;

    if ( p.x() == m_another_edge.x() ) {
        retX = m_edge.x();
        retY = (-1) * m_edge.y() + m_radius;
    } else {
        retX = m_edge.x() - m_radius;
        retY = (-1) * m_edge.y();
    }

    m_another_edge.setX(retX);
    m_another_edge.setY(retY);
    m_pointList.at(2)->setPoint(m_another_edge);
}

qreal BRounded_Rectangle::getRadius()
{
    return m_radius;
}

QPointF BRounded_Rectangle::getAnotherEdge()
{
    return m_another_edge;
}

void BRounded_Rectangle::setAnotherEdge(QPointF p)
{
    m_another_edge = p;
}

void BRounded_Rectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(this->pen());
    painter->setBrush(this->brush());

    QRectF ret(m_center.x() - m_edge.x(), m_center.y() - m_edge.y(), m_edge.x() * 2, m_edge.y() * 2);
    painter->drawRoundedRect(ret, m_radius, m_radius);
}

void BRounded_Rectangle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QMenu* menu = new QMenu();
    menu->setStyleSheet("QMenu { background-color:rgb(89,87,87); border: 5px solid rgb(235,110,36); }");

    QSpinBox* width_spinBox = new QSpinBox(menu);
    width_spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    width_spinBox->setRange(0, 1000);
    width_spinBox->setPrefix("w: ");
    width_spinBox->setSuffix(" mm");
    width_spinBox->setSingleStep(1);
    width_spinBox->setValue(2 * abs(m_edge.x()));
    connect(width_spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        m_another_edge.setX(m_another_edge.x() + v/2 - m_edge.x());
        m_edge.setX(v/2);

        m_pointList.at(0)->setPoint(m_edge);
        m_pointList.at(2)->setPoint(m_another_edge);
        this->hide();
        this->update();
        this->show();
    });

    QSpinBox* height__spinBox = new QSpinBox(menu);
    height__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    height__spinBox->setRange(0, 1000);
    height__spinBox->setPrefix("h: ");
    height__spinBox->setSuffix(" mm");
    height__spinBox->setSingleStep(1);
    height__spinBox->setValue(2 * abs(m_edge.y()));
    connect(height__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        m_another_edge.setY(m_another_edge.y() - v/2 + m_edge.y());
        m_edge.setY(v/2);

        m_pointList.at(0)->setPoint(m_edge);
        m_pointList.at(2)->setPoint(m_another_edge);
        this->hide();
        this->update();
        this->show();
    });

    QSpinBox* radius__spinBox = new QSpinBox(menu);
    radius__spinBox->setStyleSheet("QSpinBox{ width:120px; height:30px; font-size:16px; font-weight:bold; }");
    radius__spinBox->setRange(0, 1000);
    radius__spinBox->setPrefix("r: ");
    radius__spinBox->setSuffix(" mm");
    radius__spinBox->setSingleStep(1);
    radius__spinBox->setValue(m_radius);
    connect(radius__spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [=](int v){
        if (m_another_edge.x() >= abs(m_another_edge.y())) {
            m_radius = v <= m_edge.y() ? v : m_edge.y();
            m_another_edge.setY((m_edge.y() - m_radius) * (-1));
        } else {
            m_radius = v <= m_edge.x() ? v : m_edge.x();
            m_another_edge.setX(m_edge.x() - m_radius);
        }

        m_pointList.at(2)->setPoint(m_another_edge);
        this->hide();
        this->update();
        this->show();
    });

    QWidgetAction* width_widgetAction = new QWidgetAction(menu);
    width_widgetAction->setDefaultWidget(width_spinBox);
    menu->addAction(width_widgetAction);

    QWidgetAction* height_widgetAction = new QWidgetAction(menu);
    height_widgetAction->setDefaultWidget(height__spinBox);
    menu->addAction(height_widgetAction);

    QWidgetAction* radius_widgetAction = new QWidgetAction(menu);
    radius_widgetAction->setDefaultWidget(radius__spinBox);
    menu->addAction(radius_widgetAction);

    menu->exec(QCursor::pos());
    delete menu;

    QGraphicsItem::contextMenuEvent(event);
}

//------------------------------------------------------------------------------


BTargetPoint::BTargetPoint(qreal x, qreal y, qreal radius)
{
    m_oldRect = QRectF(x-radius,y-radius,radius*2,radius*2);
    m_radius = radius/2;
    HorizonLine_Start = QPointF((x - m_radius*3),y);
    HorizonLine_End = QPointF((x + m_radius*3),y);
    VerticalLine_Start = QPointF(x ,(y - m_radius*3));
    VerticalLine_End = QPointF(x ,(y + m_radius*3));
}

QRectF BTargetPoint::boundingRect() const
{
    return QRectF(m_oldRect.x() - m_radius, m_oldRect.y() - m_radius, m_oldRect.width()+m_radius*2,m_oldRect.height()+ m_radius * 2);
}

void BTargetPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen mPen = QPen(Qt::green,4);
    painter->setPen(mPen);

    //绘制圆形
    painter->drawEllipse(m_oldRect);

    //绘制靶心十字线
    mPen = QPen(Qt::red,2);
    painter->setPen(mPen);
    painter->drawLine(HorizonLine_Start,HorizonLine_End);
    painter->drawLine(VerticalLine_Start,VerticalLine_End);

}

void BTargetPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        m_startPos = event->pos();//鼠标左击时，获取当前鼠标在图片中的坐标，
        if(m_oldRect.contains(m_startPos))//旋转矩形
        {
            setCursor(Qt::PointingHandCursor);

        }
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}

void BTargetPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

        QPointF point = (event->pos() - m_startPos);
        moveBy(point.x(), point.y());
        scene()->update();
}

void BTargetPoint::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if ( !this->isSelected() )
        return;

    QGraphicsItem::contextMenuEvent(event);
}

//---------------------------------------------------------------------------------


BCrossPoint::BCrossPoint(qreal x, qreal y, qreal angle)
{
    center.setX(x);
    center.setY(y);
    QPointF p_oldLU = QPointF(x - 3*cos(1.0472),y-3*sin(1.0472));
    QPointF p_oldRU = QPointF(x + 3*cos(1.0472),y-3*sin(1.0472));
    QPointF p_oldRD = QPointF(x + 3*cos(1.0472),y+3*sin(1.0472));
    QPointF p_oldLD = QPointF(x - 3*cos(1.0472),y+3*sin(1.0472));
    QVector<QPointF> vpt;
    Point_LU = getRotatePoint(center,p_oldLU,angle);
    vpt.append(Point_LU);
    Point_RU = getRotatePoint(center,p_oldRU,angle);
    vpt.append(Point_RU);
    Point_RD = getRotatePoint(center,p_oldRD,angle);
    vpt.append(Point_RD);
    Point_LD = getRotatePoint(center,p_oldLD,angle);
    vpt.append(Point_LD);
    vpt.append(Point_LU);
    m_polyRect = QPolygonF(vpt);
}

QRectF BCrossPoint::boundingRect() const
{
    QRectF boundingRectF = m_polyRect.boundingRect();
    return QRectF(boundingRectF.x(), boundingRectF.y(), boundingRectF.width(),boundingRectF.height());
}

void BCrossPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen mPen = QPen(Qt::green,1);
    painter->setPen(mPen);

    //绘制十字线
    painter->drawLine(Point_LU,Point_RD);
    painter->drawLine(Point_RU,Point_LD);
}

QPointF BCrossPoint::getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle)
{
    double dx = ptCenter.x();
    double dy = ptCenter.y();
    double x = ptIn.x();
    double y = ptIn.y();
    double xx,yy;
    xx = (x-dx)*cos(angle*M_PI/180)-(y-dy)*sin(angle*M_PI/180)+dx;
    yy = (x-dx)*sin(angle*M_PI/180)+(y-dy)*cos(angle*M_PI/180)+dy;

    return QPointF(xx,yy);
}

//---------------------------------------------------------------------------------

BArrow::BArrow()
{

}

BArrow::BArrow(QPointF startPoint, QPointF endPoint)
{

    setFlag(ItemIsSelectable);
    setAcceptHoverEvents(true);

    m_Color = Qt::green;
    setLineItem(startPoint,endPoint);

}

void BArrow::setLineItem(QPointF startP, QPointF endP)
{
    m_EndP = endP - startP;

    CreatePointNodes();
}

void BArrow::setColor(QColor color)
{
   m_Color = color;
}

void BArrow::CreatePointNodes()
{
    //箭头直线与水平方向的夹角再加pi
    float angle = atan2(m_EndP.y(), m_EndP.x()) + 3.1415926;
    //这两个值需要根据实际场景的坐标大小进行调整，
    float ExtRefArrowLenght = 4;//箭头末端大小的长度，
    float ExtRefArrowDegrees = 1.047;//箭头末端顶角的一半

    m_points[0] = m_EndP;
     //求得箭头点1坐标
    m_points[1].setX(m_EndP.x() + ExtRefArrowLenght * cos(angle - ExtRefArrowDegrees));
    m_points[1].setY(m_EndP.y() + ExtRefArrowLenght * sin(angle - ExtRefArrowDegrees));
    //求得箭头点2坐标
    m_points[2].setX(m_EndP.x() + ExtRefArrowLenght * cos(angle + ExtRefArrowDegrees));
    m_points[2].setY(m_EndP.y() + ExtRefArrowLenght * sin(angle + ExtRefArrowDegrees));


}

QRectF BArrow::boundingRect() const
{
    return QRectF(0, 0, m_EndP.x(), m_EndP.y());
}


void BArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);                   //设置反走样，防锯齿
    QPen pen(m_Color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    QBrush brush(m_Color, Qt::SolidPattern);

    painter->setPen(pen);
    painter->setBrush(brush);

    QLineF line(0, 0, m_EndP.x(), m_EndP.y());
    painter->drawLine(line);

    painter->drawPolygon(m_points, 3);
}
