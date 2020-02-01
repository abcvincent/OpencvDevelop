#ifndef MYIMAGELIST_H
#define MYIMAGELIST_H

#include <QWidget>
#include <math.h>
#include <QWheelEvent>
#include <QListWidget>
#include <QGraphicsView>    //视图类
#include <QGraphicsScene>   //场景类
#include <QGraphicsItem>    //图元类
#include "mypixitem.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;


namespace Ui {
class MyImageList;
}

class MyImageList : public QWidget
{
    Q_OBJECT

public:
    explicit MyImageList(QWidget *parent = 0);
    ~MyImageList();

    void showListItem(Mat dstImage,QString strName,QString controlName);
    void intoList(QString listName,QString listV,Mat listMat,int listRow,int marks);
    void updataListShow(QString listNames,Mat listMats,int listRow);
    void updataListShows(int marks);
    void showTableView(Mat,QString,QString,int);



private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

//    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);    //实验双击
//    void  on_listWidget_itemActivated(QListWidgetItem *item); //实验

    void receiveDataWM(Mat,QString,QString,int,int);//接收mainwindow打开文件数据
    void receiveAction();



signals:
     //图元窗口图片被选中后弹出视图窗口，图元窗口间Mat和变量名返回给主窗口，主窗口再讲变量发给视图窗口
    void  sendDataList(Mat imageData,QString textData,QString controlName);//发送信号

private:
    Ui::MyImageList *ui;

    //声明IplImage指针
    IplImage *pImg;
    Mat pImgs;

    QList<QString> listText;//存储图像名称
    QList<QString> listData;//存储图像变量名称
    QList<QPixmap> pixmapList;//存储图像

    QList<Mat> matList;//存储mat图像

};

#endif // MYIMAGELIST_H
