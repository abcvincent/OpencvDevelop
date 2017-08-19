#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPixmap>
#include <QList>
#include "imagelook.h"

#include <math.h>
#include <QWheelEvent>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
using namespace cv;

//class ImageLook;
class MyImageList;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);



    MyImageList *myimagelists;



//    QString lineEditStr1;
//    QString lineEditStr2;
//    int lineInt1;
//    int lineInt2;




signals:
    void sendDataMW(Mat,QString,QString,int,int);
    void sendAction();
    void sendStr(int,QString);


private slots: 
    void mesgbox();
    void receiveDataList(Mat imageData,QString textData,QString controlName);

//    void receiveDlg(QString,QString,int,int);

    void on_ac_openfile_triggered();

    void on_ac_savefile_triggered();

//    void on_ac_horizontal_triggered();

//    void on_ac_vertial_triggered();

    void on_ac_imageOriginal_triggered();

    void on_ac_codeFlow_triggered();

    void on_ac_order_triggered();

    void on_ac_stacking_triggered();

    void on_ac_closeAll_triggered();

    void on_ac_nextOne_triggered();

    void on_ac_preOne_triggered();

    void on_ac_gray_triggered();

    void on_ac_h_v_triggered();

    void on_ac_erode_triggered();

    void on_ac_dilate_triggered();

    void on_ac_programme_triggered();

private:
    Ui::MainWindow *ui;

    Mat pImgMat;
    QList<QString> listVariableMainWid;//对应图像变量名字
    QList<Mat> listMatMainWid;//图像数据
    QList<QString> listNameMainWid;//图像操作名
    QList<QString> listCodeMainWid;//code流程
    bool saveOffOn;
    QString fileExist;
    QString filePathExist;

};

#endif // MAINWINDOW_H
