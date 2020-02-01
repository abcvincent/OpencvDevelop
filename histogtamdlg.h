#ifndef HISTOGTAMDLG_H
#define HISTOGTAMDLG_H

#include <QWidget>
#include <QList>
#include "math.h"
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include <QComboBox>
//#include "showhistogtamresult.h"



using namespace cv;

namespace Ui {
class HistogtamDlg;
}

class HistogtamDlg : public QWidget
{
    Q_OBJECT

public:
    explicit HistogtamDlg(QWidget *parent = 0);
    ~HistogtamDlg();            
    //    void setPicture(Mat);
    //    void createHistogtam(Mat);

    void  refreshSenddata();
    //修复消耗内存越来越卡的问题


    void setHistogtam();//滑条函数，刷新直方图
    void createHistogramGray();//创建直方图
    QImage  MatToQImage(const cv::Mat& mat);//mat转qimage
    Mat globalMat;
    Mat globalHistMat;

private:
    Ui::HistogtamDlg *ui;
    void closeEvent(QCloseEvent *event);
    //    void paintEvent(QPaintEvent *);

    QList<QString> m_listVatiate_glob;//对应图像变量名字
    QList<Mat> m_listMat_glob;//图像数据
    Mat resultMat;
    Mat result_Histogtam_Mat;
    Mat source_Histogtam_Mat;
    Mat source_gray_Mat;

    int left_Threshold;
    int right_Threshold;

signals:
    void sendToShowhistogtam(Mat,int,int);
    void hdSendDataMW(Mat,QString,QString,int,int);  //给图元窗口发送信号，将Mat，图像变量名称，操作名称，行号，标记号发送给图元窗口
    void hdSendStr(int,QString); //程序流程窗口，将操作函数文本发送给程序流程窗口



private slots:
    void receiveDataForHistogtamdlg(QList<QString>,QList<Mat>);

    //    void Threshold_SetValue(int);
    //    void Threshold_SetValue2(int);


    //////////////////////新定义
    void setLeftValue();
    void setRightValue();
    void setHistogtamRe(int);//刷新直方图
    void on_pushButton_clicked();
    void on_show_pushButton_clicked();
    void on_out_pushButton_clicked();



};


#endif //HISTOGTAMDLG_H

