#include "imagelook.h"
#include "ui_imagelook.h"
#include "mainwindow.h"

#pragma execution_character_set("utf-8")

ImageLook::ImageLook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageLook)
{
    ui->setupUi(this);
    showImage(pImgs,globNames,controlNameGlob);//显示图像

    QObject::connect(pixItem,SIGNAL(scaleChanged()),
                     this,SLOT(my_change()));//自定义信号和槽获取scale值反馈出来,scaleChanged()系统信号

    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),
                     ui->horizontalSlider,SLOT(setValue(int)));//spinBox值与滑条对应

    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),
                     ui->spinBox,SLOT(setValue(int)));//滑条与spinBox值对应

    ui->horizontalSlider->setValue(0);//滑条初始值，范围值在窗口设计器里设置了

    QObject::connect(ui->B_ReZoom,SIGNAL(clicked(bool)),
                     this,SLOT(resetzoom()));//复位按钮


}

ImageLook::~ImageLook()
{
    delete ui;
}


void ImageLook::showImage(Mat matL,QString nameL,QString controlName)
{  
    globNames=nameL;//接收加载名字
    pImgs=matL;//接收加载图元
//    controlNameGlob=controlName;

    QImage image;
    if(pImgs.channels()==1)//灰度图通道变为1
    {
        //灰度转换时候Format_Grayscale8
        QImage imageGray((const uchar*)pImgs.data, pImgs.cols,
                     pImgs.rows,pImgs.cols*pImgs.channels(),QImage::Format_Grayscale8);
        image=imageGray;
    }
    else
    {
       // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
        QImage imageOriginal((const uchar*)pImgs.data, pImgs.cols,
                     pImgs.rows,pImgs.cols*pImgs.channels(),QImage::Format_RGB888);
         image=imageOriginal;
    }

    QPixmap pixmapCV =QPixmap::fromImage(image);
//    QPixmap pixmapCV = QPixmap::fromImage(image.scaled(size(), Qt::KeepAspectRatio) );

    pixItem = new MyPixItem(new QPixmap(pixmapCV));
    //将该图元对象添加到场景中，并设置此图元在场景中的位置为中心（0，0）
    m_graphicsScene = new QGraphicsScene;  //new 一个新的场景对象
    ui->graphicsView->setSceneRect(-10/2,-10/2,10,10);
    //设置开启滚动条范围，不然即使隐藏了滚动条后还是有滚动效果
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏横向滚动条
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏纵向滚动条
    ui->graphicsView->setScene(m_graphicsScene);//添加场景
    m_graphicsScene->addItem(pixItem);//将图元添加到场景中
    pixItem->setPos(0,0);//中心位置
    setWindowTitle(globNames);

    setWindowIcon(QIcon(":/images/eye_32px.png"));


    QObject::connect(pixItem,SIGNAL(scaleChanged()),
                     this,SLOT(my_change()));//自定义信号和槽获取scale值反馈出来

}


void ImageLook::mouseDoubleClickEvent(QMouseEvent *e)
{
    ////////***********图片复原代码**************/////
    pixItem->setPos(0,0);
    pixItem->setScaleValue(1);
    pixItem->setZoomState(NO_STATE);
    ui->horizontalSlider->setValue(0);
}

//滚轮滚动图片缩小放大
void ImageLook::wheelEvent(QWheelEvent *event)
{
    pixItem->setZoomState(NO_STATE);//枚举，对应pixitem类中
    int scaleValue=pixItem->getScaleValue();//获取ScaleValue值（放大缩小值）
    if(event->delta() > 0)  //delta（）为正，滚轮向上滚
    {
        scaleValue++;
        ui->horizontalSlider->setValue(scaleValue);//同步滑条
    }
    else
    {
        scaleValue--;
        ui->horizontalSlider->setValue(scaleValue);//同步滑条
    }
    pixItem->setScaleValue(scaleValue);//重新设置ScaleValue值（放大缩小值）
}

void ImageLook::my_change()
{
    ui->horizontalSlider->setValue(pixItem->getScaleValue());//获取图元的放大缩小值
}



void ImageLook::mousePressEvent(QMouseEvent *e)
{
    last = e->globalPos(); //鼠标点击点位
    wt=width();//获取窗口大小
    ht=height();//同上
   // pixItem->setValue(p);//传到pixitem类
}


//系统内置spinBox槽函数,在设计窗口选中控件，右键添加valueChanged，当值改变时候自动启动
void ImageLook::on_spinBox_valueChanged(int arg1)
{
    pixItem->setZoomState(NO_STATE);//对应图元类中放大缩小原始枚举项
    pixItem->setScaleValue(arg1); //获取控件改变的值
    wt=width();
    ht=height();
    QPointF p(wt,ht);
    pixItem->setValue(p);//将窗口值传给pixitem类
}


//复原函数
void ImageLook::resetzoom()
{
    pixItem->setPos(0,0);//复原位置
    pixItem->setScaleValue(0);//复原比例
    pixItem->setZoomState(NO_STATE);
    ui->horizontalSlider->setValue(0);//滑条值
}


//放大按钮
void ImageLook::on_B_Boost_clicked()
{
    int a=ui->spinBox->value()+1;
    ui->horizontalSlider->setValue(a);
}

//缩小按钮
void ImageLook::on_B_Reduce_clicked()
{
    int a=ui->spinBox->value()-1;
    ui->horizontalSlider->setValue(a);
}

