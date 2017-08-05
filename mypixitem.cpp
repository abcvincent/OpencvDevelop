#include "mypixitem.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPointF>
#include <QGraphicsSceneDragDropEvent>
#include <QDrag>
#include <math.h>
#include <QMessageBox>
#include <QGraphicsObject>
#pragma execution_character_set("utf-8")

//构造函数初始化了变量pix
MyPixItem::MyPixItem(QPixmap *pixmap)
{
    pix = *pixmap;
    setAcceptDrops(true); //设置可拖拽
    m_scaleValue = 0;//放缩值初始化
    m_isMove = false;//不可移动
    widx=0;
    widy=0;
}

//实现自己的图元边界函数
QRectF MyPixItem::boundingRect() const
{
    return QRectF(-pix.width()/2, -pix.height()/2,
                  pix.width(), pix.height());//需要对应图元，不然出现残影
}

//只需QPainter的drawPixmap()函数将图元图片绘出即可
void MyPixItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                    QWidget *)
{
    painter->drawPixmap(-pix.width()/2, -pix.height()/2, pix);//需要对应边界函数
}

//鼠标点击事件
void MyPixItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_startPos = event->pos();
    m_isMove = true;//图元是否可以移动
}

void MyPixItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isMove)
    {
        QPointF point =mapToScene(event->pos())-mapToScene(m_startPos);
        moveBy(point.x(),point.y());
        // 鼠标点击后并移动则图元相应移动，进行了图元坐标映射，映射到窗口中
    }
}

void MyPixItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    m_isMove = false;
}


//双击复位
void MyPixItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setPos(0,0);
    m_scaleValue = 0;
    setScale(1);
    setTransformOriginPoint(0, 0);

    //做了坐标映射，将图片回复到窗口中心
    QPointF m_center;
    m_center=mapToScene(0,0);
    if(m_center.x()!=0||m_center.y()!=0)
    {
        if(m_center.x()>0&&m_center.y()>0) moveBy(-m_center.x(), -m_center.y());
        if(m_center.x()<0&&m_center.y()<0) moveBy(m_center.x(), m_center.y());
        if(m_center.x()>0&&m_center.y()<0) moveBy(-m_center.x(), m_center.y());
        if(m_center.x()<0&&m_center.y()>0) moveBy(m_center.x(), -m_center.y());
    }

}

//使用滚轮整体缩放
void MyPixItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    setZoomState(NO_STATE);
    int scaleValue = m_scaleValue;
    if(event->delta() > 0)  //delta（）为正，滚轮向上滚
    {
        scaleValue++;
    }
    else
    {
        scaleValue--;
    }

    if (scaleValue > ZOOM_IN_TIMES || scaleValue < ZOOM_OUT_TIMES)
        return;

    m_scaleValue = scaleValue;
    qreal s;
    if(m_scaleValue > 0)
    {
        s = pow(1.1, m_scaleValue);        //放大 计算x的y方次 参数都是double类型
    }
    else
    {
        s = pow(1 / 1.1, -m_scaleValue);      //缩小
    }
    setScale(s);//setScale设置比例放缩，内置的处理图像放缩的方法
    if(sceneBoundingRect().width()>=widx||sceneBoundingRect().height()>=widy)
    {
        //  m_isMove = false;
        setTransformOriginPoint(event->pos());//基于图元坐标内鼠标指针变换中心
    }
}

//从widget获取的缩放值，用于同步滚轮和按键
void MyPixItem::setScaleValue(const int &scaleValue)
{
    if (scaleValue > ZOOM_IN_TIMES || scaleValue < ZOOM_OUT_TIMES)
        return;

    m_scaleValue = scaleValue;

    qreal s;
    if(m_scaleValue > 0)
    {
        s = pow(1.1, m_scaleValue);     //放大 计算x的y方次 参数都是double类型，去除正负数
    }
    else
    {
        s = pow(1 / 1.1, -m_scaleValue); //缩小
    }
    setScale(s);
}

//复原图像
void MyPixItem::setZoomState(const int &zoomState)
{
    m_zoomState = zoomState;
    if (m_zoomState == RESET)
    {
        m_scaleValue = 0;
        setScale(1);
        setTransformOriginPoint(0, 0);
    }
}

//获取放缩值
int MyPixItem::getScaleValue() const
{
    return m_scaleValue;
}

//返回窗口值
void  MyPixItem::setValue(const QPointF &pointxy)
{
    QPointF p;
    p=pointxy;
    widx=p.x();
    widy=p.y();
}
