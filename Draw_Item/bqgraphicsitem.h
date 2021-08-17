#ifndef BQGRAPHICSITEM_H
#define BQGRAPHICSITEM_H

#include <QObject>
#include "bpointitem.h"

#define PI 3.1415926

// 自定义图元 - 基础类
class BGraphicsItem : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT

public:
    enum ItemType {
        Circle = 0,         // 圆
        Ellipse,            // 椭圆
        Concentric_Circle,  // 同心圆
        Pie,                // 饼
        Chord,              // 和弦
        Rectangle,          // 矩形
        Square,             // 正方形
        Polygon,            // 多边形
        Round_End_Rectangle,// 圆端矩形
        Rounded_Rectangle   // 圆角矩形
    };

    QPointF getCenter() { return m_center; }
    void setCenter(QPointF p) { m_center = p; }

    QPointF getEdge() { return m_edge; }
    void setEdge(QPointF p) { m_edge = p; }

    ItemType getType() { return m_type; }

protected:
    BGraphicsItem(QPointF center, QPointF edge, ItemType type);

    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

protected:
    QPointF m_center;
    QPointF m_edge;
    ItemType m_type;
    BPointItemList m_pointList;

    QPen m_pen_isSelected;
    QPen m_pen_noSelected;
};

//------------------------------------------------------------------------------

// 椭圆
class BEllipse : public BGraphicsItem
{
    Q_OBJECT

public:
    BEllipse(qreal x, qreal y, qreal width, qreal height, ItemType type);

    enum { Type = 20};
    int type() const
    {
        return Type;
    }

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

//------------------------------------------------------------------------------

// 圆
class BCircle : public BEllipse
{
public:
    BCircle(qreal x, qreal y, qreal radius, ItemType type);
    enum { Type = 21};
    int type() const
    {
        return Type;
    }
    void updateRadius();

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

public:
    qreal m_radius;
};

//------------------------------------------------------------------------------

// 同心圆
class BConcentricCircle : public BCircle
{
public:
    BConcentricCircle(qreal x, qreal y, qreal radius1, qreal radius2, ItemType type);
    enum { Type = 22};
    int type() const
    {
        return Type;
    }
    void updateOtherRadius();
    void setAnotherEdge(QPointF p);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

public:
    QPointF m_another_edge;
    qreal m_another_radius;
};

//------------------------------------------------------------------------------

// 饼
class BPie : public BCircle
{
public:
    BPie(qreal x, qreal y, qreal radius, qreal angle, ItemType type);
    enum { Type = 23};
    int type() const
    {
        return Type;
    }
    void updateAngle();

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

protected:
    qreal m_angle;
};

//------------------------------------------------------------------------------

// 和弦
class BChord : public BPie
{
public:
    BChord(qreal x, qreal y, qreal radius, qreal angle, ItemType type);
    enum { Type = 24};
    int type() const
    {
        return Type;
    }
    void updateEndAngle();

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

protected:
    qreal m_end_angle;
};

//------------------------------------------------------------------------------

// 矩形
class BRectangle : public BGraphicsItem
{
public:
    BRectangle(qreal x, qreal y, qreal width, qreal height, ItemType type);
    qreal GetRectLength(QString type);
    void SetRect(qreal x, qreal y, qreal width, qreal height);
    enum { Type = 25};
    int type() const
    {
        return Type;
    }
protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

//------------------------------------------------------------------------------

enum STATE_FLAG{
    DEFAULT_FLAG=0,
    MOV_LEFT_LINE,//标记当前为用户按下矩形的左边界区域
    MOV_TOP_LINE,//标记当前为用户按下矩形的上边界区域
    MOV_RIGHT_LINE,//标记当前为用户按下矩形的右边界区域
    MOV_BOTTOM_LINE,//标记当前为用户按下矩形的下边界区域
    MOV_RIGHTBOTTOM_RECT,//标记当前为用户按下矩形的右下角
    MOV_RECT,//标记当前为鼠标拖动图片移动状态
    ROTATE//标记当前为旋转状态
};

// 旋转矩形
class BRotateRectangle : public QGraphicsItem
{
public:
    BRotateRectangle(qreal x, qreal y, qreal width, qreal height);
    BRotateRectangle(qreal x, qreal y, qreal width, qreal height,QColor color);
    enum { Type = 26};
    int type() const
    {
        return Type;
    }

    qreal   m_RotateAngle;
    QPointF m_RotateCenter;
    QRectF  m_oldRect;
    QPolygonF m_oldRectPolygon;
    qreal GetPolygonLength(QString type);

    QRectF m_SmallRotateRect;//矩形顶部用来表示旋转的标记的矩形
    QPolygonF m_SmallRotatePolygon;//矩形顶部用来表示旋转的标记的矩形旋转后形成的多边形

    void SetRotate(qreal RotateAngle,QPointF ptCenter=QPointF(-999,-999));
    QPointF getSmallRotateRectCenter(QPointF ptA,QPointF ptB);//获取旋转时候矩形正上方的旋转标记矩形
    QRectF  getSmallRotateRect(QPointF ptA,QPointF ptB);
    QPointF getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle);//获取旋转后的点
    void setRectSize(QRectF mrect,bool bResetRotateCenter = true);
    QPolygonF getRotatePolygonFromRect(QPointF ptCenter,QRectF rectIn,qreal angle);//将矩形旋转之后返回多边形
    QPointF UpdateRotateRectAngle();
    QPointF getRectEdgePoint();
        virtual QRectF boundingRect() const override;
protected:
//    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
private:
    QPolygonF m_insicedPolygon;
    QRectF  m_insicedRectf;
    QPolygonF m_leftPolygon;
    QRectF  m_leftRectf;
    QPolygonF m_topPolygon;
    QRectF  m_topRectf;
    QPolygonF m_rightPolygon;
    QRectF  m_rightRectf;
    QPolygonF m_bottomPolygon;
    QRectF  m_bottomRectf;
        QPointF m_startPos;
        STATE_FLAG m_StateFlag;
        QPointF startP,endP;
        bool isAddArrow;
        QColor ArrowColor;
};

//------------------------------------------------------------------------------

// 正方形
class BSquare : public BRectangle
{
public:
    BSquare(qreal x, qreal y, qreal width, ItemType type);
    enum { Type = 27};
    int type() const
    {
        return Type;
    }
protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
};

//------------------------------------------------------------------------------

// 多边形
class BPolygon : public BGraphicsItem
{
    Q_OBJECT

public:
    BPolygon(ItemType type);
    enum { Type = 28};
    int type() const
    {
        return Type;
    }
    QPointF getCentroid(QList<QPointF> list);
    void getMaxLength();
    void updatePolygon(QPointF origin, QPointF end);

public slots:
    void pushPoint(QPointF p, QList<QPointF> list, bool isCenter);

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

protected:
    qreal m_radius;
    bool is_create_finished;
};

//------------------------------------------------------------------------------

// 圆端矩形
class BRound_End_Rectangle : public BRectangle
{
public:
    BRound_End_Rectangle(qreal x, qreal y, qreal width, qreal height, ItemType type);
    enum { Type = 29};
    int type() const
    {
        return Type;
    }
protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
};

//------------------------------------------------------------------------------

// 圆角矩形
class BRounded_Rectangle : public BRectangle
{
public:
    BRounded_Rectangle(qreal x, qreal y, qreal width, qreal height, ItemType type);
    enum { Type = 30};
    int type() const
    {
        return Type;
    }
    void updateRadius();
    void updateAnotherEdge(QPointF p);
    qreal getRadius();
    QPointF getAnotherEdge();
    void setAnotherEdge(QPointF p);

protected:
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

protected:
    QPointF m_another_edge;
    qreal m_radius;
};

//------------------------------------------------------------------------------

//靶点
class BTargetPoint : public QGraphicsItem
{
public:
    BTargetPoint();
    BTargetPoint(qreal x, qreal y, qreal radius);
    enum { Type = 31};
    int type() const
    {
        return Type;
    }

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
//    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
private:
    QRectF m_oldRect;
    qreal m_radius;
    QPointF HorizonLine_Start,HorizonLine_End;
    QPointF VerticalLine_Start,VerticalLine_End;
    QPointF m_startPos;
};

//---------------------------------------------------------------------------------

//交叉点
class BCrossPoint : public QGraphicsItem
{
public:
    BCrossPoint();
    BCrossPoint(qreal x, qreal y, qreal angle);
    enum { Type = 32};
    int type() const
    {
        return Type;
    }
        QPointF getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle);//获取旋转后的点

protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;

private:
QPointF center;
QPointF Point_LU;
QPointF Point_RU;
QPointF Point_RD;
QPointF Point_LD;
QPolygonF m_polyRect;
};


//------------------------------------------------------------------------------------------

//箭头
class BArrow : public QGraphicsItem
{
  public:
    BArrow();
    BArrow(QPointF startPoint,QPointF endPoint);
    void setLineItem(QPointF startP, QPointF endP);
    void setColor(QColor color);
        enum { Type = 33};
    int type() const
    {
        return Type;
    }
protected:
    virtual QRectF boundingRect() const override;

    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget) override;
private:
    void CreatePointNodes(void);
private:
    QPointF m_EndP;
    QPointF m_points[3];
    QColor  m_Color;
};
#endif // BQGRAPHICSITEM_H

