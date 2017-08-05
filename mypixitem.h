#ifndef PIXITEM_H
#define PIXITEM_H
#include <QGraphicsItem>
#include <QPixmap>
#include <QPainter>
#include <QRectF>
#include <QMouseEvent>
#include <QPointF>
#include <QDragEnterEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsObject>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

//枚举方便传递
enum Enum_ZoomState{
    NO_STATE,
    RESET,
    ZOOM_IN,
    ZOOM_OUT
};

//放大缩小值范围
enum Enum_ZoomTimes{
    ZOOM_IN_TIMES = 50,
    ZOOM_OUT_TIMES = -50,
};

//直接继承QGraphicsObject，这样有信号和槽QGraphicsItem没有信号和槽功能
class MyPixItem :public QGraphicsObject//public QGraphicsItem
//继承自图元类,实现自定义的图元,qt预置的有直线，椭圆，文本图元，矩形图元等
{
public:
    MyPixItem(QPixmap *pixmap);     //构造函数初始化了变量pix
    QRectF boundingRect() const;
    //实现自己的boundingRect 图元边界方法，继承QGraphicsItem必须要初始化话的虚函数
    //完成以图元坐标系为基础增加两个像素点的冗余的工作

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget); //重画图形函数

    void wheelEvent(QGraphicsSceneWheelEvent *event);//滚轮放大缩小
    void setZoomState(const int &zoomState);//设置图像恢复原始
    void mousePressEvent(QGraphicsSceneMouseEvent *event);//用于移动图像
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);//用于移动图像
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);//用于移动图像
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);//双击恢复图像
    int getScaleValue() const;//获取当前图元放缩值
    void setScaleValue(const int &);//返回放放缩值
    void setValue(const QPointF &);//返回窗口大小值
    int widx;
    int widy;

signals:


private:
    qreal m_scaleValue;   //缩放值
    QPixmap pix;    //作为图元显示的图片
    int m_zoomState;
    bool m_isMove;
    QPointF m_startPos;
};

#endif // PIXITEM_H
