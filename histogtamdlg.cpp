#include "histogtamdlg.h"
#include "ui_histogtamdlg.h"
#include <QMessageBox>
#include <QPainter>

#pragma execution_character_set("utf-8")


HistogtamDlg::HistogtamDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogtamDlg)
{
    ui->setupUi(this);    

    Mat matImage;
    for (int i=0;i<10;++i){
        matImage = Mat(Size(2200, 2200), CV_8UC3);//大小2200*2200像素图片大了会报错
        m_listMat_glob<<matImage;
    }

    left_Threshold=1;
    right_Threshold=255;

    QObject::connect(ui->left_spinBox,SIGNAL(valueChanged(int)),
                     ui->left_horizontalSlider,SLOT(setValue(int)));//spinBox值与滑条对应

    QObject::connect(ui->left_horizontalSlider,SIGNAL(valueChanged(int)),
                     ui->left_spinBox,SLOT(setValue(int)));//滑条与spinBox值对应


    QObject::connect(ui->right_spinBox,SIGNAL(valueChanged(int)),
                     ui->right_horizontalSlider,SLOT(setValue(int)));//spinBox值与滑条对应

    QObject::connect(ui->right_horizontalSlider,SIGNAL(valueChanged(int)),
                     ui->right_spinBox,SLOT(setValue(int)));//滑条与spinBox值对应

//    QObject::connect(ui->left_spinBox,SIGNAL(valueChanged(int)),
//                     this,SLOT(Threshold_SetValue(int)));//spinBox值对应对应刷新

//    QObject::connect(ui->right_spinBox,SIGNAL(valueChanged(int)),
//                     this,SLOT(Threshold_SetValue2(int)));//spinBox值对应刷新

//    ui->histogtam_label->setScaledContents(true);//自动调整为Qlabel大小。

    connect(ui->left_horizontalSlider,SIGNAL(sliderReleased()),this,SLOT(setLeftValue()));
    connect(ui->right_horizontalSlider,SIGNAL(sliderReleased()),this,SLOT(setRightValue()));
    connect(ui->left_spinBox,SIGNAL(valueChanged(int)),this,SLOT(setHistogtamRe(int)));
    connect(ui->right_spinBox,SIGNAL(valueChanged(int)),this,SLOT(setHistogtamRe(int)));

}


HistogtamDlg::~HistogtamDlg()
{
    delete ui;
}




void HistogtamDlg::receiveDataForHistogtamdlg(QList<QString> m_listVatiate,QList<Mat> m_listMat)
{   
    for(int i=0;i<m_listVatiate.size();++i)
    {
        QString stri=m_listVatiate.at(i);//QString::number(i,10,0);
        ui->comboBox->addItem(stri);
    }

    if(m_listVatiate.size()>9)
    {
        Mat matImage = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
        m_listMat_glob.append(matImage);
    }
    m_listMat_glob=m_listMat;//list全局，对应的mat组
    m_listVatiate_glob=m_listVatiate; //lisr全局存储,对应的变量名字
//    createHistogtam(m_listMat_glob.at(0));
//    setPicture(result_Histogtam_Mat);


    //读取combobox对应的图
    for(int i=0;i<m_listVatiate.size();++i)//遍历变量名
    {
        //变量名如果和对话框中的值相等输出对象的Mat
        if(m_listVatiate.at(i)==ui->comboBox->currentText())
        {
            globalMat=m_listMat_glob.at(i).clone();
            break;
        }
    }

//    globalMat=m_listMat_glob.at(0).clone();//新添加全局mat变量

    createHistogramGray();//创建直方图
    setHistogtam();//添加直方图

}

void HistogtamDlg::closeEvent(QCloseEvent *event)
{


    QMessageBox mesg3;
    mesg3.about(NULL,"信息","关闭窗口了");

    this->setAttribute(Qt::WA_DeleteOnClose);

    //在这里添加你希望执行关闭事件需要处理的事情
    //弹出消息框，关闭其他窗口
}


 void  HistogtamDlg::refreshSenddata()
 {
     emit sendToShowhistogtam(resultMat,
                              ui->left_horizontalSlider->value(),
                              ui->right_horizontalSlider->value());

 }


 ////////////////////2017.10.22////////修复实验

 QImage HistogtamDlg::MatToQImage(const cv::Mat& mat)
 {
     // 8-bits unsigned, NO. OF CHANNELS = 1
     if (mat.type() == CV_8UC1)
     {
         QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
         // Set the color table (used to translate colour indexes to qRgb values)
         image.setColorCount(256);
         for (int i = 0; i < 256; i++)
         {
             image.setColor(i, qRgb(i, i, i));
         }
         // Copy input Mat
         uchar *pSrc = mat.data;
         for (int row = 0; row < mat.rows; row++)
         {
             uchar *pDest = image.scanLine(row);
             memcpy(pDest, pSrc, mat.cols);
             pSrc += mat.step;
         }
         return image;
     }
     // 8-bits unsigned, NO. OF CHANNELS = 3
     else if (mat.type() == CV_8UC3)
     {
         // Copy input Mat
         const uchar *pSrc = (const uchar*)mat.data;
         // Create QImage with same dimensions as input Mat
         QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
         return image.rgbSwapped();
     }
     else if (mat.type() == CV_8UC4)
     {
         // Copy input Mat
         const uchar *pSrc = (const uchar*)mat.data;
         // Create QImage with same dimensions as input Mat
         QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
         return image.copy();
     }
     else
     {
         return QImage();
     }
 }


 void HistogtamDlg::createHistogramGray()//创建直方图
 {

#define cvQueryHistValue_1D( hist, idx0 )\
    ((float)cvGetReal1D( (hist)->bins, (idx0)))

     //进行，灰度处理
     //matOri输入图像 由对话框来选
     //dstImage输出图像，最后希望显示的图像必须添加
     //例如：cv::cvtColor(matOri,dstImage,COLOR_RGB2GRAY);
     Mat intoMat=globalMat.clone();
     IplImage *src= &IplImage(intoMat);
     IplImage* gray_plane = cvCreateImage(cvGetSize(src),8,1);
     cvCvtColor(src,gray_plane,CV_RGB2GRAY);
     //  cv::cvtColor(src,gray_plane,COLOR_RGB2GRAY);

     int hist_size = 256;    //直方图尺寸
     int hist_height = 256;
     float range[] = {0,255};  //灰度级的范围
     float* ranges[]={range};
     //创建一维直方图，统计图像在[0 255]像素的均匀分布
     CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);
     //计算灰度图像的一维直方图
     cvCalcHist(&gray_plane,gray_hist,0,0);
     //归一化直方图
     cvNormalizeHist(gray_hist,1.0);

     int scale = 2;
     //创建一张一维直方图的“图”，横坐标为灰度级，纵坐标为像素个数（*scale）
     IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale,hist_height),8,3);
     cvZero(hist_image);
     //统计直方图中的最大直方块
     float max_value = 0;
     cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);

     //分别将每个直方块的值绘制到图中
     for(int i=0;i<hist_size;i++)
     {
         float bin_val = cvQueryHistValue_1D(gray_hist,i); //像素i的概率
         int intensity = cvRound(bin_val*hist_height/max_value);  //要绘制的高度
         cvRectangle(hist_image,
                     cvPoint(i*scale,hist_height-1),
                     cvPoint((i+1)*scale - 1, hist_height - intensity),
                     CV_RGB(255,255,255));
     }


     //2019.12.8 刷新占内存原因可能是每次移动滑条都要计算一遍直方图导致的,现在看不是直方图导致的
     Mat outMat=cvarrToMat(hist_image);
     globalHistMat=outMat;//2019.12.8 获取生成的直方图

//     IplImage* transIplimage = cvCloneImage(&(IplImage) outMat);
//     cvLine(transIplimage,cvPoint(1*2*(ui->left_horizontalSlider->value()),0),cvPoint(1*2*(ui->left_horizontalSlider->value()),255),CV_RGB(0,255,0),2);
//     cvLine(transIplimage,cvPoint(1*2*(ui->right_horizontalSlider->value()),0),cvPoint(1*2*(ui->right_horizontalSlider->value()),255),CV_RGB(255,0,0),2);

//     outMat=cvarrToMat(transIplimage);
//     QImage imgshow=MatToQImage(outMat);
//     ui->histogtam_label->clear();
//     imgshow=imgshow.scaled(this->ui->histogtam_label->width(),this->ui->histogtam_label->height(),Qt::KeepAspectRatio);
//     this->ui->histogtam_label->setPixmap(QPixmap::fromImage(imgshow));
 }

 void HistogtamDlg::setHistogtam()//滑条函数，刷新直方图
 {
     Mat img2=globalMat.clone();
     Mat dst;
     int value_m=this->ui->left_horizontalSlider->value();
     int value_m2=this->ui->right_horizontalSlider->value();

     cv::cvtColor(img2,img2,COLOR_RGB2GRAY);//进行，灰度处理
     //阈值分割
     threshold( img2,                 //输入图像,原始数组 (单通道 , 8-bit of 32-bit 浮点数).
                img2,                          //输出图像,输出数组，必须与 src 的类型一致，或者为 8-bit.
                value_m,                 //分割值
                0,                         // 使用 CV_THRESH_BINARY 和 CV_THRESH_BINARY_INV 的最大值.
                cv::THRESH_TOZERO); //阈值类型

     threshold(img2,                 //输入图像,原始数组 (单通道 , 8-bit of 32-bit 浮点数).
               dst,                          //输出图像,输出数组，必须与 src 的类型一致，或者为 8-bit.
               value_m2,                 //分割值
               0,                         // 使用 CV_THRESH_BINARY 和 CV_THRESH_BINARY_INV 的最大值.
               cv::THRESH_TOZERO_INV  ); //阈值类型

     threshold( dst,                 //输入图像,原始数组 (单通道 , 8-bit of 32-bit 浮点数).
                dst,                          //输出图像,输出数组，必须与 src 的类型一致，或者为 8-bit.
                1,                         //分割值
                255,                         // 使用 CV_THRESH_BINARY 和 CV_THRESH_BINARY_INV 的最大值.
                cv::THRESH_BINARY); //阈值类型

//     QImage imgshow=MatToQImage(dst);
//     this-> ui->lb_OutToImg->clear();
//     imgshow=imgshow.scaled( this->ui->lb_OutToImg->width(), this->ui->lb_OutToImg->height(),Qt::KeepAspectRatio);
//     this-> ui->lb_OutToImg->setPixmap(QPixmap::fromImage(imgshow));
//     outGlobalMat=dst.clone();

     resultMat=dst.clone();//处理结果
     refreshSenddata();

     //2019.12.8 将刷新放在这里
     Mat outMat=globalHistMat.clone();
     IplImage* transIplimage = cvCloneImage(&(IplImage) outMat);
     cvLine(transIplimage,cvPoint(1*2*(ui->left_horizontalSlider->value()),0),cvPoint(1*2*(ui->left_horizontalSlider->value()),255),CV_RGB(0,255,0),2);
     cvLine(transIplimage,cvPoint(1*2*(ui->right_horizontalSlider->value()),0),cvPoint(1*2*(ui->right_horizontalSlider->value()),255),CV_RGB(255,0,0),2);

     outMat=cvarrToMat(transIplimage);
     QImage imgshow=MatToQImage(outMat);
     ui->histogtam_label->clear();
     imgshow=imgshow.scaled(this->ui->histogtam_label->width(),this->ui->histogtam_label->height(),Qt::KeepAspectRatio);
     this->ui->histogtam_label->setPixmap(QPixmap::fromImage(imgshow));

 }

 void HistogtamDlg::setHistogtamRe(int value)//刷新直方图
 {
//     createHistogramGray();
     setHistogtam();
 }


 void HistogtamDlg::setLeftValue()
 {

     if(ui->left_horizontalSlider->value()>=ui->right_spinBox->value())
     {
         ui->right_spinBox->setValue(ui->left_horizontalSlider->value()+1);
     }


 }
 void HistogtamDlg::setRightValue()
 {

     if(ui->right_horizontalSlider->value()<=ui->left_spinBox->value())
     {
         ui->left_spinBox->setValue(ui->right_horizontalSlider->value()-1);
     }

 }

void HistogtamDlg::on_pushButton_clicked()
{

    /*
    直方图的均值化。
    图像中像素个数多的灰度级进行展宽，
    而对图像中像素个数少的灰度进行压缩，
    从而扩展像原取值的动态范围，
    提高了对比度和灰度色调的变化，
    使图像更加清晰。
    */

}

void HistogtamDlg::on_show_pushButton_clicked()
{

    //读取combobox对应的图
    for(int i=0;i< m_listVatiate_glob.size();++i)//遍历变量名
    {
        //变量名如果和对话框中的值相等输出对象的Mat
        if( m_listVatiate_glob.at(i)==ui->comboBox->currentText())
        {
            globalMat=m_listMat_glob.at(i).clone();
            break;
        }
    }

     setHistogtam();//刷新视图
}


//确定按钮
void HistogtamDlg::on_out_pushButton_clicked()
{


    Mat dstImage;
    if( m_listMat_glob.isEmpty()){
        QMessageBox emptybox;
        emptybox.about(NULL,"信息","没有加载图片");
    }
    else
    {
        QString inImgDlgVar=ui->comboBox->currentText();//输入变量名字
        QString outImgDlgVar=ui->out_lineEdit->text();//输出变量名字

        ////////////////////////////////
        ///*******获取变量**********//
        ///////////////////////
        QString left_var1=QString::number(ui->left_spinBox->value(),10,0);//获取变量1
        QString right_var2=QString::number(ui->right_spinBox->value(),10,0);//获取变量2


        for(int i=0;i< m_listVatiate_glob.size();++i)//遍历变量名，如果输出重复设为默认
        {
            if(outImgDlgVar==m_listVatiate_glob.at(i))//如果没有输入变量
            {

                QMessageBox mesg;
                mesg.about(NULL,"提示","无输出对象或对象重复,设置为默认");

                QString strVar= QString::number(m_listVatiate_glob.size(),10,0);
                QString strVariable=QString("image%1").arg(strVar);
                outImgDlgVar=strVariable;
                break;
            }
        }

        if(outImgDlgVar.isEmpty())//如果没有输入变量，设为默认
        {
            QMessageBox mesg;
            mesg.about(NULL,"提示","无输出对象或对象重复,设置为默认");

            QString strVar= QString::number(m_listVatiate_glob.size(),10,0);
            QString strVariable=QString("image%1").arg(strVar);
            outImgDlgVar=strVariable;
        }

        m_listVatiate_glob.append(outImgDlgVar);//添加变量名list
        //            listNameMainWid.append(strg);//添加操作名list

        if(m_listMat_glob.size()>9)//mat list 长度超过9后要开辟空间
        {
            Mat matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
            m_listMat_glob.append(matImageMainWid);
        }
        m_listMat_glob.insert(m_listMat_glob.size()-1,resultMat);//添加图元list


        //code内容
        QString sendstrg=QString("threshold( %1,%1, %2, 0,cv::THRESH_TOZERO); //阈值类型;\r\n"
                                 "threshold( %1,dst,%3, 0,cv:: cv::THRESH_TOZERO_INV ); //阈值类型;//设置核\r\n"
                                 "threshold( dst,%4, 1, 255,cv:: cv::THRESH_BINARY); //阈值类型,\r\n")
                .arg(inImgDlgVar).arg(left_var1).arg(right_var2).arg(outImgDlgVar);

        QMessageBox mesg2;
        mesg2.about(NULL,"信息",sendstrg);

        QString strg="灰度阈值分割";


//        QMessageBox mesg3;
//        mesg3.about(NULL,"信息",outImgDlgVar);

        dstImage=resultMat.clone();

        emit hdSendDataMW(dstImage,strg,outImgDlgVar,0,-1);
        emit hdSendStr(-1,sendstrg);

    }

}




