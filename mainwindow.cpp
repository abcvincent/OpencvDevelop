#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagelook.h"
#include "myimagelist.h"
#include "variatedialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QMdiSubWindow>
#include "codewidget.h"
#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QFormLayout>
#include <QFrame>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QDebug>
#include <QTime>
#include <QDir>
#include <QFile>

#include <math.h>
#include <QWheelEvent>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#pragma execution_character_set("utf-8")

using namespace cv;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    saveOffOn=false;
    fileExist="";

    filePathExist="";
    //开辟控件存储图元
    Mat matImageMainWid;
    for (int i=0;i<10;++i){
        matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//大小2200*2200像素图片大了会报错
        listMatMainWid<<matImageMainWid;
    }


    myimagelists=new MyImageList;//添加list窗口对象，图元窗口
    ui->mdiArea->addSubWindow(myimagelists)->resize(300,300);//添加到mdi上
    myimagelists->show();//显示

    codeWidget *codew=new codeWidget;//创建code窗口，程序流程窗口
    ui->mdiArea->addSubWindow(codew)->resize(300,300);//添加到mdi上
    codew->show();//显示

    //给图元窗口发送信号，将Mat，图像变量名称，操作名称，行号，标记号发送给图元窗口
    QObject::connect(this,SIGNAL(sendDataMW(Mat,QString,QString,int,int)),myimagelists,SLOT(receiveDataWM(Mat,QString,QString,int,int)));
    //给图元窗口发送信号，让其刷新数据
    QObject::connect(this,SIGNAL(sendAction()),myimagelists,SLOT(receiveAction()));
    //图元窗口图片被选中后弹出视图窗口，图元窗口间Mat和变量名返回给主窗口，主窗口再讲变量发给视图窗口
    QObject::connect(myimagelists,SIGNAL(sendDataList(Mat,QString,QString)),this,SLOT(receiveDataList(Mat,QString,QString)));
    //程序流程窗口，将操作函数文本发送给程序流程窗口
    QObject::connect(this,SIGNAL(sendStr(int,QString)),codew,SLOT(receiveStr(int,QString)));//发送到code


    //灰度直方图信号槽
    his_dlg=new HistogtamDlg;
    connect(this,SIGNAL(sendDataToHistogtamdlg(QList<QString>,QList<Mat>)),
            his_dlg,SLOT(receiveDataForHistogtamdlg(QList<QString>,QList<Mat>)));
    connect(his_dlg,SIGNAL(sendToShowhistogtam(Mat,int,int)),
            this,SLOT(receiveHistogtamResult(Mat,int,int)));


    //灰度直方图分割结果信号槽
    show_his_image=new ShowHistogtamResult;
    connect(this,SIGNAL(sendHistogtam_Mat(Mat)),show_his_image,SLOT(receiveHistogtam_Mat(Mat)));

    ////2019.12.7更新
    //给图元窗口发送信号，将Mat，图像变量名称，操作名称，行号，标记号发送给图元窗口
//    connect(his_dlg,SIGNAL(hdSendDataMW(QString)),this,SLOT(receiveDataHd(QString)));
    //给图元窗口发送信号，让其刷新数据
//    connect(his_dlg,SIGNAL(hdSendAction()),myimagelists,SLOT(receiveAction()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


///////////////////////////////////////////
//实验程序开始
void MainWindow::on_ac_programme_triggered()
{
//    QMessageBox mesg;
//    mesg.about(NULL,"信息","开始定义程序");



    QString strg="自定义图像处理";
    QString textDlg="算子：无";
    QList<QString> listCV_CodeWid1;
    listCV_CodeWid1={"Not"};
    QList<QString> listCV_CodeWid2;
    listCV_CodeWid2={"Not"};
    QList<QString> listCV_CodeWid3;
    listCV_CodeWid3={"Not"};
    int VariaeCount=1;


    Mat dstImage;
    if(listVariableMainWid.isEmpty()){
        QMessageBox emptybox;
        emptybox.about(NULL,"信息","没有加载图片");
    }
    else
    {
        //打开操作对话框，创建时候需要将数据输入，操作名，变量list<T>,算子解释
        VariateDialog *varDlg=new VariateDialog(strg,
                                                listVariableMainWid,
                                                textDlg,
                                                VariaeCount,
                                                listCV_CodeWid1,
                                                listCV_CodeWid2,
                                                listCV_CodeWid3);
        varDlg->exec();//显示对话框

        if(varDlg->cancelDlg==1)//做判断如果对话框点击取消或叉则不添加，等于1时才赋值等
        {
            Mat matOri;//创建一个Mat
            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名
            {
                //变量名如果和对话框中的值相等输出对象的Mat
                if(listVariableMainWid.at(i)==varDlg->comboxStr)
                {
                    matOri=listMatMainWid.at(i);
                    break;
                }
            }
            QString inImgDlgVar=varDlg->comboxStr;//输入变量名字
            QString outImgDlgVar=varDlg->lineEditStr;//输出变量名字


            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名，如果输出重复设为默认
            {
                if(outImgDlgVar==listVariableMainWid.at(i))//如果没有输入变量
                {
                    mesgbox();
                    QString strVar= QString::number(listVariableMainWid.size(),10,0);
                    QString strVariable=QString("image%1").arg(strVar);
                    outImgDlgVar=strVariable;
                    break;
                }
            }

            if(outImgDlgVar.isEmpty())//如果没有输入变量，设为默认
            {
                mesgbox();
                QString strVar= QString::number(listVariableMainWid.size(),10,0);
                QString strVariable=QString("image%1").arg(strVar);
                outImgDlgVar=strVariable;
            }

            listVariableMainWid.append(outImgDlgVar);//添加变量名list
            listNameMainWid.append(strg);//添加操作名list


            ////////////////////////////////////////////////////
            ///////////////////////////////
            /////////////////////////////
            ////////////////////////

            //cvQueryHistValue_1D宏，必须加下面句，不然报错，没有
#define cvQueryHistValue_1D( hist, idx0 )\
    ((float)cvGetReal1D( (hist)->bins, (idx0)))

            //进行，灰度处理
            //matOri输入图像 由对话框来选
            //dstImage输出图像，最后希望显示的图像必须添加
            //例如：cv::cvtColor(matOri,dstImage,COLOR_RGB2GRAY);

            //灰度直方图处理测试

            IplImage *src= &IplImage(matOri);
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


            cvNamedWindow( "GraySource", 1 );
            cvShowImage("GraySource",gray_plane);
            cvNamedWindow( "H-S Histogram", 1 );
            cvShowImage( "H-S Histogram", hist_image );

            dstImage=cvarrToMat(hist_image);//IplImage转Mat




            /////////////////////////////////
            ////////////////////////////////
            ////////////////////////////////
            ////////////////////////////////
            ///////////////////////////////




            QString sendstrg=QString("code(%1,%2);//输入：%1，输出：%2//灰度直方图\r\n")
                    .arg(inImgDlgVar).arg(outImgDlgVar);

            if(listVariableMainWid.size()>9)
            {
                Mat matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
                listMatMainWid.append(matImageMainWid);
            }
            listMatMainWid.insert(listVariableMainWid.size()-1,dstImage);//添加图元list


            emit sendStr(-1,sendstrg);//发送到code
            emit sendDataMW(dstImage,strg,outImgDlgVar,0,-1);//发送到list
            emit sendAction();//刷新list
            listCodeMainWid.append(sendstrg);//添加到code存储表中



            /////////////手动添加/////////////////
            //////////////////
            /////////////////////////////
            ///

             Mat dstImage2;
             cv::cvtColor(matOri,dstImage2,COLOR_RGB2GRAY);
            //Mat dstImage2=cvarrToMat(gray_plane);//IplImage转Mat

            QString outImgDlgVar2;
            QString strg2="灰度操作";


            if(outImgDlgVar2.isEmpty())//如果没有输入变量，设为默认
            {

                QString strVar= QString::number(listVariableMainWid.size(),10,0);
                QString strVariable=QString("image%1").arg(strVar);
                outImgDlgVar2=strVariable;
            }


            listVariableMainWid.append(outImgDlgVar2);//添加变量名list
            listNameMainWid.append(strg2);//添加操作名list


            QString sendstrg2=QString("%1;//灰度转换\r\n").arg(outImgDlgVar2);



            if(listVariableMainWid.size()>9)
            {
                Mat matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
                listMatMainWid.append(matImageMainWid);
            }
            listMatMainWid.insert(listVariableMainWid.size()-1,dstImage2);//添加图元list


            emit sendStr(-1,sendstrg2);//发送到code
            emit sendDataMW(dstImage2,strg2,outImgDlgVar2,0,-1);//发送到list
            emit sendAction();//刷新list
            listCodeMainWid.append(sendstrg2);//添加到code存储表中



            //////////////////////////////////阈值操作////////////
            /// \brief src
            ///
            ///

            Mat src3=cvarrToMat(gray_plane);
            Mat dst;

            double thresh = 150;
            double thresh2 = 200;
            int maxVal = 255;
            // cv::threshold(src3, dst, thresh, maxVal, cv::THRESH_BINARY);
              cv::threshold(src3, dst, thresh, maxVal, cv::THRESH_TOZERO);
              cv::threshold(dst, dst, thresh2, maxVal, cv::THRESH_BINARY);



            cv::imshow("threshold", dst);

            QString Threshold=QString::number(thresh,10,2);
            QString setMaxShow=QString::number(maxVal,10,0);

            Mat dstImage3=dst;
            QString outImgDlgVar3;
            QString strg3="灰度阈值操作";
            QString sendstrg3=QString("Threshold=%1,setMaxShow=%2//灰度阈值分割\r\n")
                    .arg(Threshold).arg(setMaxShow);

            if(outImgDlgVar3.isEmpty())//如果没有输入变量，设为默认
            {
                QString strVar= QString::number(listVariableMainWid.size(),10,0);
                QString strVariable=QString("image%1").arg(strVar);
                outImgDlgVar3=strVariable;
            }

            QString sendstrg_g=QString("%1;%2").arg(outImgDlgVar3).arg(sendstrg3);


            listVariableMainWid.append(outImgDlgVar3);//添加变量名list
            listNameMainWid.append(strg3);//添加操作名list

            if(listVariableMainWid.size()>9)
            {
                Mat matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
                listMatMainWid.append(matImageMainWid);
            }
            listMatMainWid.insert(listVariableMainWid.size()-1,dstImage3);//添加图元list

            emit sendStr(-1,sendstrg_g);//发送到code
            emit sendDataMW(dstImage3,strg3,outImgDlgVar3,0,-1);//发送到list
            emit sendAction();//刷新list
            listCodeMainWid.append(sendstrg_g);//添加到code存储表中
        }
        else
        {
            QMessageBox mesg;
            mesg.about(NULL,"信息","已经取消输入");
        }
    }
}

//////////////////////////////////////////////
///////实验程序结束




//打开图片
void MainWindow::on_ac_openfile_triggered()
{


    if (saveOffOn)
    {
        QMessageBox::StandardButton mesgdir= QMessageBox::information (NULL,
                                                                       "信息", "文件未保存，是否保存？",
                                                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(mesgdir==QMessageBox::Yes)
        {
            on_ac_savefile_triggered();
        }

    }


    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open image file"),
                "./", tr("Image files(*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)"));



    if(fileName.isEmpty())
    {
        QMessageBox mesg;
        mesg.warning(this,"警告","打开图片失败!");//.about(NULL,"图片大小", strg);
        return;
    }

    //    QString  str=fileName;
    //    char*  ch;
    //    QByteArray ba = str.toLatin1(); // must
    //    ch=ba.data();
    //    Mat pImgs=imread(ch);

    QString  str=fileName;//文件名
    std::string path = str.toLocal8Bit().toStdString(); //opencv加载中文问题关键是这个
    Mat pImgs=imread(path,1 );//cv函数创建Mat

    cvtColor(pImgs,pImgs,CV_BGR2RGB);//opencv Mat一般是bgr格式需要转换成rgb

    QString  str1="原始图";//操作名

    emit sendDataMW(pImgs,str1,"image",0,0);//发送myimagelist,显示图像图元窗口
    emit sendAction();//刷新图元列表窗口

    //更新mainwindow的list列表
    listVariableMainWid.clear();//清空图元变量名称
    listVariableMainWid.append("image");//图元代号，list存储
    listMatMainWid.insert(0,pImgs);//添加Mat
    listNameMainWid.clear();//情况操作名称
    listNameMainWid.append(str1);//添加操作名称


    //发送到codewidget显示程序流程
    QString sendstrg=QString("Mat image=imread(%1,1);//加载图片\r\n").arg(str);//.arg(total).arg(fileName);
    emit sendStr(0,sendstrg);//发送给程序流程窗口
    pImgMat=pImgs;//传给其他操作
    listCodeMainWid.append(sendstrg);//讲流程文本数据保存
    saveOffOn=true;
    fileExist.clear();

}

void MainWindow::on_ac_savefile_triggered()
{

    if(listVariableMainWid.isEmpty())//如果没有变量侧提示并返回
    {
        QMessageBox mesgs;
        mesgs.about(NULL,"信息","无文件");
        saveOffOn=true;
        return;
    }
    else
    {

        QString dirPath;
        QString filePath;

        if(fileExist.isEmpty())//判断fileExist是否存储，如果存在则说明已经保存过
        {
            QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
            QString str = time.toString("yyyyMMdd_hhmmss"); //设置显示格式
            filePath = QFileDialog::getExistingDirectory(this,"");
            dirPath=QString("%1/Opencv develop temp%2").arg(filePath).arg(str);
            QDir *temp = new QDir;//声明文件对象
            temp->mkdir(dirPath);//创建文件夹
            filePathExist=filePath;//存储文件路径，为了再次保存时候找到
            fileExist=dirPath;//存储文件名，为了再次保存时候找到
        }
        else
        {
            dirPath=fileExist;
            filePath=filePathExist;
            QMessageBox mesgdir;
            mesgdir.about(NULL,"保存在",fileExist);
        }

        if(filePath.isEmpty())
        {
            QMessageBox::information(this,"信息","保存失败");
            return;
        }
        else
        {
            QString myTxt=QString("%1/程序流程.txt").arg(dirPath);//创建txt
            QFile myfile(myTxt);
            myfile.open(QIODevice::WriteOnly);

            for(int i=0;i<listCodeMainWid.size();i++)
            {
                QTextStream in(&myfile);
                in<<listCodeMainWid.at(i);//+"\r\n";
                in.flush();
            }
            myfile.close();


            for(int i=0;i<listVariableMainWid.size();i++)
            {
                QString saveName=listVariableMainWid.at(i);
                Mat dstImage=listMatMainWid.at(i);
                QString savePath=QString("%1//%2.jpg").arg(dirPath).arg(saveName);

                /*
                //mat保存方法,有bug,bgr会转rgb原因未知
                if(dstImage.channels()==1)//灰度图通道变为1
                {
                   dstImage=dstImage;
                }
                else
                {
                  cvtColor(dstImage,dstImage,CV_RGB2BGR);
                }
                QByteArray ba = savePath.toLatin1();
                char *savePathCV=ba.data();
                IplImage qImg;
                qImg = IplImage(dstImage); //
                cvSaveImage(savePathCV, &qImg);
                */

                //QImage保存方法
                QImage images;
                if(dstImage.channels()==1)//灰度图通道变为1
                {
                    //    灰度转换时候需要用Format_Grayscale8
                    QImage imagesGray((const uchar*)dstImage.data, dstImage.cols,
                                      dstImage.rows,dstImage.cols*dstImage.channels(),QImage::Format_Grayscale8);
                    images=imagesGray;
                }
                else
                {
                    // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
                    QImage imagesOriginal((const uchar*)dstImage.data, dstImage.cols,
                                          dstImage.rows,dstImage.cols*dstImage.channels(),QImage::Format_RGB888);
                    images=imagesOriginal;
                }

                images.save(savePath);

            }
            QMessageBox::information(this,"信息","保存成功");
            saveOffOn=true;

        }
    }
}


//收到lmyimagelist信号，显示imagelook窗口
void MainWindow::receiveDataList(Mat imageData,QString textData,QString controlName)
{
    bool flags=false;
    for(int i=0;i<ui->mdiArea->subWindowList().size();i++)
    {
        QString str2=ui->mdiArea->subWindowList().at(i)->windowTitle();
        if(str2==textData)
        {
            ui->mdiArea->setActiveSubWindow(ui->mdiArea->subWindowList().at(i));
            flags=true;
            break;
        }
    }

    if(flags==false)
    {
        ImageLook *imgL=new ImageLook;
        imgL->showImage(imageData,textData,controlName);
        QMdiSubWindow *subwindow1=new QMdiSubWindow;
        subwindow1->setWidget(imgL);
        subwindow1->setAttribute(Qt::WA_DeleteOnClose);
        ui->mdiArea->addSubWindow(subwindow1);
        subwindow1->show();
        ui->mdiArea->activeSubWindow()->resize(imageData.cols/2,imageData.rows/2);
    }
}

void MainWindow::mesgbox()
{
    QMessageBox mesg;
    mesg.about(NULL,"提示","无输出对象或对象重复,设置为默认");

}


void MainWindow::on_ac_imageOriginal_triggered()
{
    bool flags=false;
    for(int i=0;i<ui->mdiArea->subWindowList().size();i++)
    {
//        QString str=QString::number(ui->mdiArea->subWindowList().size(),10,0);
//        QMessageBox mesg;
//        mesg.about(NULL,"提示",str);

        QString str2=ui->mdiArea->subWindowList().at(i)->windowTitle();
        if(str2=="图元窗口")
        {
            ui->mdiArea->setActiveSubWindow(ui->mdiArea->subWindowList().at(i));
            flags=true;
            break;
        }
    }

    if(flags==false)
    {

        myimagelists=new MyImageList;

        for(int i=0;i<listVariableMainWid.size();i++)
        {
        myimagelists->intoList(listNameMainWid.at(i),listVariableMainWid.at(i),listMatMainWid.at(i),i,-1);

        }
        myimagelists->updataListShows(1);
        ui->mdiArea->addSubWindow(myimagelists)->resize(300,300);
        myimagelists->show();

        QObject::connect(this,SIGNAL(sendDataMW(Mat,QString,QString,int,int)),myimagelists,SLOT(receiveDataWM(Mat,QString,QString,int,int)));
        QObject::connect(this,SIGNAL(sendAction()),myimagelists,SLOT(receiveAction()));
        QObject::connect(myimagelists,SIGNAL(sendDataC(Mat,QString)),this,SLOT(receiveDataC(Mat,QString)));
    }

}

void MainWindow::on_ac_codeFlow_triggered()
{


    bool flags=false;
    for(int i=0;i<ui->mdiArea->subWindowList().size();i++)
    {
        QString str2=ui->mdiArea->subWindowList().at(i)->windowTitle();
        if(str2=="程序流程窗口")
        {
            ui->mdiArea->setActiveSubWindow(ui->mdiArea->subWindowList().at(i));
            flags=true;
            break;
        }
    }

    if(flags==false)
    {
        codeWidget *codew=new codeWidget;
        QObject::connect(this,SIGNAL(sendStr(int,QString)),codew,SLOT(receiveStr(int,QString)));//发送到code

        for(int i=0;i<listCodeMainWid.size();i++)
        {

            QString sendstrg=listCodeMainWid.at(i);
            emit sendStr(-1,sendstrg);
        }

        ui->mdiArea->addSubWindow(codew)->resize(300,300);
        codew->show();

    }
}



void MainWindow::on_ac_order_triggered()
{
    ui->mdiArea->tileSubWindows();
}

void MainWindow::on_ac_stacking_triggered()
{
    ui->mdiArea->cascadeSubWindows();
}

void MainWindow::on_ac_closeAll_triggered()
{
    ui->mdiArea->closeAllSubWindows();
}

void MainWindow::on_ac_nextOne_triggered()
{
    ui->mdiArea->activateNextSubWindow();
}

void MainWindow::on_ac_preOne_triggered()
{
    ui->mdiArea->activatePreviousSubWindow();
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    if (saveOffOn)
    {
        QMessageBox::StandardButton mesgdir= QMessageBox::information (NULL,
                                                                       "信息", "文件未保存，是否保存？",
                                                                      QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(mesgdir==QMessageBox::Yes)
        {
            on_ac_savefile_triggered();
        }

        else
        {
            event->ignore();
            event->accept();
        }

        //        QMessageBox mesgdir;
        //        mesgdir.about(NULL,"信息","未保存");

    }
    else
    {
        event->accept();
    }
}

void MainWindow::on_ac_gray_triggered()//灰度图
{


    QString strg="灰度处理";
    QString textDlg="算子：cv::cvtColor(Input src,Outpu dst,COLOR_BGR2GRAY)";
    QList<QString> listCV_CodeWid1;
    listCV_CodeWid1={"Yes_combox","COLOR_RGB2GRAY"};
    QList<QString> listCV_CodeWid2;
    listCV_CodeWid2={"Not"};
    QList<QString> listCV_CodeWid3;
    listCV_CodeWid3={"Not"};
    int VariaeCount=1;


    Mat dstImage;
    if(listVariableMainWid.isEmpty()){
        QMessageBox emptybox;
        emptybox.about(NULL,"信息","没有加载图片");
    }
    else
    {
        //打开操作对话框，创建时候需要将数据输入，操作名，变量list<T>,算子解释
        VariateDialog *varDlg=new VariateDialog(strg,
                                                listVariableMainWid,
                                                textDlg,
                                                VariaeCount,
                                                listCV_CodeWid1,
                                                listCV_CodeWid2,
                                                listCV_CodeWid3);
        varDlg->exec();//显示对话框

        if(varDlg->cancelDlg==1)//做判断如果对话框点击取消或叉则不添加，等于1时才赋值等
        {
            Mat matOri;//创建一个Mat
            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名
            {
                //变量名如果和对话框中的值相等输出对象的Mat
                if(listVariableMainWid.at(i)==varDlg->comboxStr)
                {
                    matOri=listMatMainWid.at(i);
                    break;
                }
            }
            QString inImgDlgVar=varDlg->comboxStr;//输入变量名字
            QString outImgDlgVar=varDlg->lineEditStr;//输出变量名字


            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名，如果输出重复设为默认
            {
                if(outImgDlgVar==listVariableMainWid.at(i))//如果没有输入变量
                {
                    mesgbox();
                    QString strVar= QString::number(listVariableMainWid.size(),10,0);
                    QString strVariable=QString("image%1").arg(strVar);
                    outImgDlgVar=strVariable;
                    break;
                }
            }

            if(outImgDlgVar.isEmpty())//如果没有输入变量，设为默认
            {
                mesgbox();
                QString strVar= QString::number(listVariableMainWid.size(),10,0);
                QString strVariable=QString("image%1").arg(strVar);
                outImgDlgVar=strVariable;
            }

            listVariableMainWid.append(outImgDlgVar);//添加变量名list
            listNameMainWid.append(strg);//添加操作名list

            //进行，灰度处理
            cv::cvtColor(matOri,dstImage,COLOR_RGB2GRAY);

            QString sendstrg=QString("cv::cvtColor(%1,%2,CV_RGB2GRAY);//灰度转换\r\n").arg(inImgDlgVar).arg(outImgDlgVar);

            if(listVariableMainWid.size()>9)
            {
                Mat matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
                listMatMainWid.append(matImageMainWid);
            }
            listMatMainWid.insert(listVariableMainWid.size()-1,dstImage);//添加图元list


            emit sendStr(-1,sendstrg);
            emit sendDataMW(dstImage,strg,outImgDlgVar,0,-1);
            emit sendAction();

            listCodeMainWid.append(sendstrg);

        }
        else
        {
            QMessageBox mesg;
            mesg.about(NULL,"信息","已经取消输入");
        }

    }

}


void MainWindow::on_ac_h_v_triggered()
{
    /*
     * 先判断有没有加载图片，如果有则启动对话框，没有则不启动；
     * 让后判断对话框点击取消或者×时候不进行操作
     * 进入对话框后对其内已有变量进行遍历，如果新变量和之前变量一样或者空则设为默认；
     * 进行图像转换操作；
     * 判断mat list 是否需要开辟新空间
     * 传递参数
     */

    ////**************修改变量位置******************/////
    QString strg="翻转操作";
    QString textDlg="算子：cv::flip(Input src,Outpu dst,int flipCode)";
    QList<QString> listCV_CodeWid1;
    listCV_CodeWid1={"Yes_combox","水平翻转","垂直翻转","水平垂直翻转"};
    QList<QString> listCV_CodeWid2;
    listCV_CodeWid2={"Not"};
    QList<QString> listCV_CodeWid3;
    listCV_CodeWid3={"Not"};

     int VariaeCount=1;
   ////******************************************/////


    Mat dstImage;
    if(listVariableMainWid.isEmpty()){
        QMessageBox emptybox;
        emptybox.about(NULL,"信息","没有加载图片");
    }
    else
    {
        //打开操作对话框，创建时候需要将数据输入，操作名，变量list<T>,算子解释
        VariateDialog *varDlg=new VariateDialog(strg,
                                                listVariableMainWid,
                                                textDlg,VariaeCount,
                                                listCV_CodeWid1,
                                                listCV_CodeWid2,
                                                listCV_CodeWid3);
        varDlg->exec();//显示对话框

        if(varDlg->cancelDlg==1)//做判断如果对话框点击取消或叉则不添加，等于1时才赋值等
        {
            Mat matOri;//创建一个Mat
            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名
            {
                //变量名如果和对话框中的值相等输出对象的Mat
                if(listVariableMainWid.at(i)==varDlg->comboxStr)
                {
                    matOri=listMatMainWid.at(i);
                    break;
                }
            }
            QString inImgDlgVar=varDlg->comboxStr;//输入变量名字
            QString outImgDlgVar=varDlg->lineEditStr;//输出变量名字



            QString var1=varDlg->lineEditStr1;

            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名，如果输出重复设为默认
            {
                if(outImgDlgVar==listVariableMainWid.at(i))//如果没有输入变量
                {
                    mesgbox();
                    QString strVar= QString::number(listVariableMainWid.size(),10,0);
                    QString strVariable=QString("image%1").arg(strVar);
                    outImgDlgVar=strVariable;
                    break;
                }
            }

            if(outImgDlgVar.isEmpty())//如果没有输入变量，设为默认
            {
                mesgbox();
                QString strVar= QString::number(listVariableMainWid.size(),10,0);
                QString strVariable=QString("image%1").arg(strVar);
                outImgDlgVar=strVariable;
            }

            listVariableMainWid.append(outImgDlgVar);//添加变量名list
            listNameMainWid.append(strg);//添加操作名list



            ////**************修改变量位置******************/////
            int variateFlip;

            if(var1.isEmpty()){variateFlip=-1;}

            //"水平翻转","垂直翻转","水平垂直翻转"

            if(var1=="水平翻转"){variateFlip=1;}
            if(var1=="垂直翻转"){variateFlip=0;}
            if(var1=="水平垂直翻转"){variateFlip=-1;}



            //进行，翻转操作
            cv::flip(matOri,dstImage,variateFlip);

            //code内容
            QString sendstrg=QString("cv::filp(%1,%2,%3);//%4操作\r\n")
                    .arg(inImgDlgVar).arg(outImgDlgVar).arg(variateFlip).arg(var1);

            ////******************************************/////



            if(listVariableMainWid.size()>9)//mat list 长度超过9后要开辟空间
            {
                Mat matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
                listMatMainWid.append(matImageMainWid);
            }
            listMatMainWid.insert(listVariableMainWid.size()-1,dstImage);//添加图元list


            emit sendStr(-1,sendstrg);
            emit sendDataMW(dstImage,strg,outImgDlgVar,0,-1);
            emit sendAction();
            listCodeMainWid.append(sendstrg);
        }
        else
        {
            QMessageBox mesg;
            mesg.about(NULL,"信息","已经取消输入");
        }
    }

}



void MainWindow::on_ac_erode_triggered()
{
    /*
     * 1)先判断有没有加载图片，如果有则启动对话框，没有则不启动；
     * 2)让后判断对话框点击取消或者×时候不进行操作
     * 3)进入对话框后对其内已有变量进行遍历，如果新变量和之前变量一样或者空则设为默认；
     * 4)进行图像转换操作；
     * 5)判断mat list 是否需要开辟新空间
     * 6)传递参数
     */


    ////**************修改变量位置******************/////
    QString strg="腐蚀操作";
    QString textDlg="算子：cv::erode(Input src,Outpu dst,element)\n"
                    "变量值1为腐蚀类型为：MORPH_RECT,MORPH_CROSS,MORPH_ELLIPSE;\n"
                    "变量值2为核大小";

    QList<QString> listCV_CodeWid1;
    listCV_CodeWid1={"Yes_combox","MORPH_RECT","MORPH_CROSS","MORPH_ELLIPSE"};
    QList<QString> listCV_CodeWid2;
    listCV_CodeWid2={"Yes_Number_double"};
    QList<QString> listCV_CodeWid3;
    listCV_CodeWid3={"Not"};

    int VariaeCount=2;

    //    MORPH_RECT    = 0, //!< a rectangular structuring element:  \f[E_{ij}=1\f]
    //    MORPH_CROSS   = 1, //!< a cross-shaped structuring element:
    //                       //!< \f[E_{ij} =  \fork{1}{if i=\texttt{anchor.y} or j=\texttt{anchor.x}}{0}{otherwise}\f]
    //    MORPH_ELLIPSE = 2
    ////******************************************/////


    Mat dstImage;
    if(listVariableMainWid.isEmpty()){
        QMessageBox emptybox;
        emptybox.about(NULL,"信息","没有加载图片");
    }
    else
    {
        //打开操作对话框，创建时候需要将数据输入，操作名，变量list<T>,算子解释
        VariateDialog *varDlg=new VariateDialog(strg,
                                                listVariableMainWid,
                                                textDlg,
                                                VariaeCount,
                                                listCV_CodeWid1,
                                                listCV_CodeWid2,
                                                listCV_CodeWid3);
        varDlg->exec();//显示对话框

        if(varDlg->cancelDlg==1)//做判断如果对话框点击取消或叉则不添加，等于1时才赋值等
        {
            Mat matOri;//创建一个Mat
            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名
            {
                //变量名如果和对话框中的值相等输出对象的Mat
                if(listVariableMainWid.at(i)==varDlg->comboxStr)
                {
                    matOri=listMatMainWid.at(i);
                    break;
                }
            }
            QString inImgDlgVar=varDlg->comboxStr;//输入变量名字
            QString outImgDlgVar=varDlg->lineEditStr;//输出变量名字


            ////////////////////////////////
            ///*******获取变量**********//
            ///////////////////////
            QString var1=varDlg->lineEditStr1;//获取变量1
            QString var2=varDlg->lineEditStr2;//获取变量2


            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名，如果输出重复设为默认
            {
                if(outImgDlgVar==listVariableMainWid.at(i))//如果没有输入变量
                {
                    mesgbox();
                    QString strVar= QString::number(listVariableMainWid.size(),10,0);
                    QString strVariable=QString("image%1").arg(strVar);
                    outImgDlgVar=strVariable;
                    break;
                }
            }

            if(outImgDlgVar.isEmpty())//如果没有输入变量，设为默认
            {
                mesgbox();
                QString strVar= QString::number(listVariableMainWid.size(),10,0);
                QString strVariable=QString("image%1").arg(strVar);
                outImgDlgVar=strVariable;
            }

            listVariableMainWid.append(outImgDlgVar);//添加变量名list
            listNameMainWid.append(strg);//添加操作名list



            ////**************修改变量位置******************/////

            if(var1.isEmpty())
            {
                mesgbox();
                var1="MORPH_RECT";
            }
            if(var2.isEmpty())
            {
                mesgbox();
                var2="3";
            }

            //进行，腐蚀操作
            int erosion_type;//类型
            int erosion_size;//核大小

            if(var1=="MORPH_RECT"){erosion_type=MORPH_RECT;}
            if(var1=="MORPH_CROSS"){erosion_type=MORPH_CROSS;}
            if(var1=="MORPH_ELLIPSE"){erosion_type=MORPH_ELLIPSE;}

            erosion_size=var2.toDouble();

            QString typeStrg=var1;
            QString varStrg=var2;//QString::number(erosion_size,10,0);
            Mat element=getStructuringElement(erosion_type,
                                              Size(2*erosion_size+1,2*erosion_size+1),
                                              Point(erosion_size,erosion_size));
            cv::erode(matOri,dstImage,element);


            //code内容
            QString sendstrg=QString("int erosion_type=%1;\r\n"
                                     "int erosion_size=%2;//设置核\r\n"
                                     "Mat element=getStructuringElement(erosion_type,"
                                     "Size(2*erosion_size+1,2*erosion_size+1),"
                                     "Point(erosion_size,erosion_size));\r\n"
                                     "cv::erode(%3,%4,element);//腐蚀操作\r\n")
                    .arg(typeStrg).arg(varStrg).arg(inImgDlgVar).arg(outImgDlgVar);

            ////******************************************/////



            if(listVariableMainWid.size()>9)//mat list 长度超过9后要开辟空间
            {
                Mat matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
                listMatMainWid.append(matImageMainWid);
            }
            listMatMainWid.insert(listVariableMainWid.size()-1,dstImage);//添加图元list


            emit sendStr(-1,sendstrg);
            emit sendDataMW(dstImage,strg,outImgDlgVar,0,-1);
            emit sendAction();
            listCodeMainWid.append(sendstrg);
        }
        else
        {
            QMessageBox mesg;
            mesg.about(NULL,"信息","已经取消输入");
        }
    }

}

void MainWindow::on_ac_dilate_triggered()
{
   /*
    int dilation_size=2;
    int dilation_type;
    dilation_type=MORPH_RECT;
    Mat element=getStructuringElement(dilation_type,
                                      Size(2*dilation_size+1,2*dilation_size+1),
                                      Point(dilation_size,dilation_size));
    Mat dstImage;
    dilate(pImgs,dstImage,element);
    QString strg="膨胀";
    */


    /*
     * 先判断有没有加载图片，如果有则启动对话框，没有则不启动；
     * 让后判断对话框点击取消或者×时候不进行操作
     * 进入对话框后对其内已有变量进行遍历，如果新变量和之前变量一样或者空则设为默认；
     * 进行图像转换操作；
     * 判断mat list 是否需要开辟新空间
     * 传递参数
     */


    ////**************修改变量位置******************/////
    QString strg="膨胀操作";
    QString textDlg="算子：cv::dilate(Input src,Outpu dst,element)\n"
                    "变量值1为腐蚀类型为：MORPH_RECT,MORPH_CROSS,MORPH_ELLIPSE;\n"
                    "变量值2为核大小";

    QList<QString> listCV_CodeWid1;
    listCV_CodeWid1={"Yes_combox","MORPH_RECT","MORPH_CROSS","MORPH_ELLIPSE"};
    QList<QString> listCV_CodeWid2;
    listCV_CodeWid2={"Yes_Number_double"};
    QList<QString> listCV_CodeWid3;
    listCV_CodeWid3={"Not"};

     int VariaeCount=2;

    //    MORPH_RECT    = 0, //!< a rectangular structuring element:  \f[E_{ij}=1\f]
    //    MORPH_CROSS   = 1, //!< a cross-shaped structuring element:
    //                       //!< \f[E_{ij} =  \fork{1}{if i=\texttt{anchor.y} or j=\texttt{anchor.x}}{0}{otherwise}\f]
    //    MORPH_ELLIPSE = 2
   ////******************************************/////


    Mat dstImage;
    if(listVariableMainWid.isEmpty()){
        QMessageBox emptybox;
        emptybox.about(NULL,"信息","没有加载图片");
    }
    else
    {
        //打开操作对话框，创建时候需要将数据输入，操作名，变量list<T>,算子解释
        VariateDialog *varDlg=new VariateDialog(strg,
                                                listVariableMainWid,
                                                textDlg,
                                                VariaeCount,
                                                listCV_CodeWid1,
                                                listCV_CodeWid2,
                                                listCV_CodeWid3);
        varDlg->exec();//显示对话框

        if(varDlg->cancelDlg==1)//做判断如果对话框点击取消或叉则不添加，等于1时才赋值等
        {
            Mat matOri;//创建一个Mat
            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名
            {
                //变量名如果和对话框中的值相等输出对象的Mat
                if(listVariableMainWid.at(i)==varDlg->comboxStr)
                {
                    matOri=listMatMainWid.at(i);
                    break;
                }
            }
            QString inImgDlgVar=varDlg->comboxStr;//输入变量名字
            QString outImgDlgVar=varDlg->lineEditStr;//输出变量名字


            ////////////////////////////////
            ///*******获取变量**********//
            ///////////////////////
            QString var1=varDlg->lineEditStr1;//获取变量1
            QString var2=varDlg->lineEditStr2;//获取变量2


            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名，如果输出重复设为默认
            {
                if(outImgDlgVar==listVariableMainWid.at(i))//如果没有输入变量
                {
                    mesgbox();
                    QString strVar= QString::number(listVariableMainWid.size(),10,0);
                    QString strVariable=QString("image%1").arg(strVar);
                    outImgDlgVar=strVariable;
                    break;
                }
            }

            if(outImgDlgVar.isEmpty())//如果没有输入变量，设为默认
            {
                mesgbox();
                QString strVar= QString::number(listVariableMainWid.size(),10,0);
                QString strVariable=QString("image%1").arg(strVar);
                outImgDlgVar=strVariable;
            }


            listVariableMainWid.append(outImgDlgVar);//添加变量名list
            listNameMainWid.append(strg);//添加操作名list



            ////**************修改变量位置******************/////

            if(var1.isEmpty())
            {
                mesgbox();
                var1="MORPH_RECT";
            }
            if(var2.isEmpty())
            {
                mesgbox();
                var2="3";
            }

               //进行，腐蚀操作
            int control_type;//类型
            int control_size;//核大小

            if(var1=="MORPH_RECT"){control_type=MORPH_RECT;}//矩形
            if(var1=="MORPH_CROSS"){control_type=MORPH_CROSS;}//交叉形
            if(var1=="MORPH_ELLIPSE"){control_type=MORPH_ELLIPSE;}//椭圆形

            control_size=var2.toDouble();


            QString typeStrg=var1;
            QString varStrg=var2;//QString::number(erosion_size,10,0);
            Mat element=getStructuringElement(control_type,
                                              Size(2*control_size+1,2*control_size+1),
                                              Point(control_size,control_size));
            cv::dilate(matOri,dstImage,element);


            //code内容
            QString sendstrg=QString("int control_type=%1;\r\n"
                                     "int control_size=%2;//设置核\r\n"
                                     "Mat element=getStructuringElement(control_type,"
                                     "Size(2*control_size+1,2*control_size+1),"
                                     "Point(control_size,control_size));\r\n"
                                     "cv::dilate(%3,%4,element);//膨胀操作\r\n")
                                      .arg(typeStrg).arg(varStrg).arg(inImgDlgVar).arg(outImgDlgVar);

            ////******************************************/////



            if(listVariableMainWid.size()>9)//mat list 长度超过9后要开辟空间
            {
                Mat matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
                listMatMainWid.append(matImageMainWid);
            }
            listMatMainWid.insert(listVariableMainWid.size()-1,dstImage);//添加图元list


            emit sendStr(-1,sendstrg);
            emit sendDataMW(dstImage,strg,outImgDlgVar,0,-1);
            emit sendAction();
            listCodeMainWid.append(sendstrg);
        }
        else
        {
            QMessageBox mesg;
            mesg.about(NULL,"信息","已经取消输入");
        }
    }
}

void MainWindow::on_ac_histogram_triggered()
{

//    //信号和槽写在这里会导致卡
//    his_dlg=new HistogtamDlg;
//    connect(this,SIGNAL(sendDataToHistogtamdlg(QList<QString>,QList<Mat>)),
//            his_dlg,SLOT(receiveDataForHistogtamdlg(QList<QString>,QList<Mat>)));
//    connect(his_dlg,SIGNAL(sendToShowhistogtam(Mat,int,int)),
//            this,SLOT(receiveHistogtamResult(Mat,int,int)));
//    ////////

        bool flags=false;
        for(int i=0;i<ui->mdiArea->subWindowList().size();i++)
        {
            QString str=ui->mdiArea->subWindowList().at(i)->windowTitle();
            if(str=="直方图分割")
            {
                ui->mdiArea->setActiveSubWindow(ui->mdiArea->subWindowList().at(i));
                flags=true;
                break;
            }
        }

        if(flags==false)
        {
            if(listVariableMainWid.size()!=0)
            {

                his_dlg=new HistogtamDlg;
                connect(this,SIGNAL(sendDataToHistogtamdlg(QList<QString>,QList<Mat>)),
                        his_dlg,SLOT(receiveDataForHistogtamdlg(QList<QString>,QList<Mat>)));
                connect(his_dlg,SIGNAL(sendToShowhistogtam(Mat,int,int)),
                        this,SLOT(receiveHistogtamResult(Mat,int,int)));

                //处理后图像返回
                connect(his_dlg,SIGNAL(hdSendDataMW(Mat,QString,QString,int,int)),
                        this,SLOT(receiveDataHd(Mat,QString,QString,int,int)));
                //程序流程窗口，将操作函数文本发送给程序流程窗口
                connect(his_dlg,SIGNAL(hdSendStr(int,QString)),this,SLOT(receiveStrHd(int,QString)));//发送到code



                ui->mdiArea->addSubWindow(his_dlg);//->resize(550,422);
                his_dlg->show();
                emit sendDataToHistogtamdlg(listVariableMainWid,listMatMainWid);
                his_dlg->refreshSenddata();
//                  qDebug()<<"ok";//输出计时
            }
            else
            {
                QMessageBox msg;
                msg.about(NULL,"信息","无加载图片");
            }
        }

//    his_dlg->refreshSenddata();
}


 void MainWindow::receiveHistogtamResult(Mat HistogtamMat,int left_m,int right_m)
 {

//     ///写在这里会卡
//     show_his_image=new ShowHistogtamResult;
//     connect(this,SIGNAL(sendHistogtam_Mat(Mat)),show_his_image,SLOT(receiveHistogtam_Mat(Mat)));
//     ////

     bool flags2=false;
     for(int i=0;i<ui->mdiArea->subWindowList().size();i++)
     {
         QString str2=ui->mdiArea->subWindowList().at(i)->windowTitle();
         if(str2=="直方图分割处理结果")
         {
             ui->mdiArea->setActiveSubWindow(ui->mdiArea->subWindowList().at(i));
             flags2=true;
             break;
         }
     }
     if(flags2==false)
     {
         if(listVariableMainWid.size()!=0)
         {

             show_his_image=new ShowHistogtamResult;
             connect(this,SIGNAL(sendHistogtam_Mat(Mat)),show_his_image,SLOT(receiveHistogtam_Mat(Mat)));

             ui->mdiArea->addSubWindow(show_his_image)->resize(300,300);
             show_his_image->show();
             show_his_image->showHistogtam_Mat(HistogtamMat);
         }
     }

     show_his_image->reFresh();

     emit sendHistogtam_Mat(HistogtamMat);
 }

void MainWindow::on_ac_demo_triggered()
{



    QString strg="自定义图像处理";
    QString textDlg="算子：无";
    QList<QString> listCV_CodeWid1;
    listCV_CodeWid1={"Not"};
    QList<QString> listCV_CodeWid2;
    listCV_CodeWid2={"Not"};
    QList<QString> listCV_CodeWid3;
    listCV_CodeWid3={"Not"};
    int VariaeCount=1;


    Mat dstImage;
    if(listVariableMainWid.isEmpty()){
        QMessageBox emptybox;
        emptybox.about(NULL,"信息","没有加载图片");
    }
    else
    {
        //打开操作对话框，创建时候需要将数据输入，操作名，变量list<T>,算子解释
        VariateDialog *varDlg=new VariateDialog(strg,
                                                listVariableMainWid,
                                                textDlg,
                                                VariaeCount,
                                                listCV_CodeWid1,
                                                listCV_CodeWid2,
                                                listCV_CodeWid3);
        varDlg->exec();//显示对话框

        if(varDlg->cancelDlg==1)//做判断如果对话框点击取消或叉则不添加，等于1时才赋值等
        {
            Mat matOri;//创建一个Mat
            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名
            {
                //变量名如果和对话框中的值相等输出对象的Mat
                if(listVariableMainWid.at(i)==varDlg->comboxStr)
                {
                    matOri=listMatMainWid.at(i);
                    break;
                }
            }
            QString inImgDlgVar=varDlg->comboxStr;//输入变量名字
            QString outImgDlgVar=varDlg->lineEditStr;//输出变量名字


            for(int i=0;i<listVariableMainWid.size();++i)//遍历变量名，如果输出重复设为默认
            {
                if(outImgDlgVar==listVariableMainWid.at(i))//如果没有输入变量
                {
                    mesgbox();
                    QString strVar= QString::number(listVariableMainWid.size(),10,0);
                    QString strVariable=QString("image%1").arg(strVar);
                    outImgDlgVar=strVariable;
                    break;
                }
            }

            if(outImgDlgVar.isEmpty())//如果没有输入变量，设为默认
            {
                mesgbox();
                QString strVar= QString::number(listVariableMainWid.size(),10,0);
                QString strVariable=QString("image%1").arg(strVar);
                outImgDlgVar=strVariable;
            }

            listVariableMainWid.append(outImgDlgVar);//添加变量名list
            listNameMainWid.append(strg);//添加操作名list


            ////////////////////////////////////////////////////
            ///////////////////////////////
            /////////////////////////////
            ////////////////////////
            ///
//            IplImage *src= &IplImage(matOri);
//            IplImage* gray_plane = cvCreateImage(cvGetSize(src),8,1);

            IplImage *src_back2 = 0;

            IplImage *src2= &IplImage(matOri);
            src_back2 = cvCreateImage(cvGetSize(src2), src2->depth, src2->nChannels);

            IplImage *src = 0; //定义源图像指针
            IplImage *tmp = 0; //定义临时图像指针
            IplImage *src_back = 0; //定义源图像背景指针
            IplConvKernel *element = 0; //定义形态学结构指针

            //1.读取和显示图像
            //    /* the first command line parameter must be image file name */
            //            src = cvLoadImage("D:/ImageTest/rice.png", 0);
            src = cvLoadImage("D:/ImageTest/1.bmp", 0);

        //    src = cvLoadImage("D:/ImageTest/Rice.png", 0);
            cvNamedWindow( "src", CV_WINDOW_AUTOSIZE );
            cvShowImage( "src", src );
            //cvSmooth(src, src, CV_MEDIAN, 3, 0, 0, 0); //中值滤波，消除小的噪声；

            //2.估计图像背景
            tmp = cvCreateImage( cvGetSize(src), src->depth, src->nChannels);
            src_back = cvCreateImage( cvGetSize(src), src->depth, src->nChannels);
            //创建结构元素
            element = cvCreateStructuringElementEx( 4, 4, 1, 1, CV_SHAPE_ELLIPSE, 0);
            //用该结构对源图象进行数学形态学的开操作后，估计背景亮度
            cvErode( src, tmp, element, 10);
            cvDilate( tmp, src_back, element, 10);
            cvNamedWindow( "src_back", CV_WINDOW_AUTOSIZE );
            cvShowImage( "src_back", src_back );

            /////////////////////////////////
            ////////////////////////////////
            ////////////////////////////////
            ////////////////////////////////
            ///////////////////////////////


            dstImage=cvarrToMat(src_back2);//转化语句

            QString sendstrg=QString("code(%1,%2);//输入：%1，输出：%2\r\n")
                    .arg(inImgDlgVar).arg(outImgDlgVar);

            if(listVariableMainWid.size()>9)
            {
                Mat matImageMainWid = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
                listMatMainWid.append(matImageMainWid);
            }
            listMatMainWid.insert(listVariableMainWid.size()-1,dstImage);//添加图元list


            emit sendStr(-1,sendstrg);//发送到code
            emit sendDataMW(dstImage,strg,outImgDlgVar,0,-1);//发送到list
            emit sendAction();//刷新list
            listCodeMainWid.append(sendstrg);//添加到code存储表中


        }
        else
        {
            QMessageBox mesg;
            mesg.about(NULL,"信息","已经取消输入");
        }
    }

}

void MainWindow::receiveDataHd(Mat dstImage,QString strg,QString outImgDlgVar,int a,int b)
{

    QMessageBox mesg;
    mesg.about(NULL,"信息","输出图像");

//     emit sendStr(-1,sendstrg);//发送到code
    emit sendDataMW(dstImage,strg,outImgDlgVar,0,-1);//发送到list
    emit sendAction();//刷新list
}

//灰度阈值分割处理结果code
void MainWindow::receiveStrHd(int a,QString strCode)
{

     emit sendStr(-1,strCode);

}
