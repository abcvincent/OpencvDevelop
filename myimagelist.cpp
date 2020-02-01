#include "myimagelist.h"
#include "ui_myimagelist.h"
#include<Qsplitter>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardItemModel>
#include "imagelook.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


#pragma execution_character_set("utf-8")


MyImageList::MyImageList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyImageList)
{
    ui->setupUi(this);
    setWindowTitle("图元窗口");
    Mat matImage;
    for (int i=0;i<10;++i){
        matImage = Mat(Size(2200, 2200), CV_8UC3);//QPixmap(2200,2200);
        matList<<matImage;
    }
}

MyImageList::~MyImageList()
{
    delete ui;
}


void MyImageList::showListItem(Mat dstImage,QString strName,QString controlName)
{

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
        // cvtColor(dstImage,dstImage,CV_BGR2RGB);
        // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
        QImage imagesOriginal((const uchar*)dstImage.data, dstImage.cols,
                              dstImage.rows,dstImage.cols*dstImage.channels(),QImage::Format_RGB888);
        images=imagesOriginal;
    }



    //    //实验Mat与QImage互转换
    //    Mat mat = cv::Mat(image.height(), image.width(), CV_8UC3,
    //                      (void*)image.constBits(), image.bytesPerLine());
    //   // cv::cvtColor(mat, mat, CV_BGR2RGB);

    //    QImage images((const uchar*)mat.data, mat.cols,
    //                mat.rows,mat.cols*dstImage.channels(),QImage::Format_RGB888);
    //    // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。

    QPixmap pixmapCV =QPixmap::fromImage(images);
    QString strg=strName;
    QListWidgetItem *LItem = new QListWidgetItem(QIcon(pixmapCV.scaled(50,50)), strg);
    ui->listWidget->setIconSize(QSize(70, 80));
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    LItem->setSizeHint(QSize(70,80));
    ui->listWidget->addItem(LItem);//insertItem(3,LItem);
    showTableView(dstImage,listText.at(ui->listWidget->row(LItem)),listData.at(ui->listWidget->row(LItem)),ui->listWidget->row(LItem));

}


//点击list图显示imagelook图像
void MyImageList::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(!item)
    {
        return;
    }
    int rowNb=ui->listWidget->row(item);
    emit sendDataList(matList.at(rowNb),listData.at(rowNb),listText.at(rowNb));//imagelook获得数据，窗口名字和图片
    showTableView(matList.at(rowNb),listText.at(rowNb),listData.at(rowNb),rowNb);
}


//void MyImageList:: on_listWidget_itemDoubleClicked(QListWidgetItem *item)
//{
//    if(!item)
//    {
//        return;
//    }
//    int rowNb=ui->listWidget->row(item);
//    qDebug() <<"双击了"<<rowNb;
//}

//void MyImageList:: on_listWidget_itemActivated(QListWidgetItem *item)
//{

//    if(!item)
//    {
//        return;
//    }
//    int rowNb=ui->listWidget->row(item);
//    qDebug() <<"信号"<<rowNb;
//}


void MyImageList::intoList(QString listName,QString listV, Mat listMat, int listRow, int marks)
{
    if(marks==0)//初始化
    {
        listText.clear();
        listData.clear();
        listData.append(listV);
        listText.append(listName);
        matList.insert(listRow,listMat);
    }

    if(marks==-1)//末尾添加
    {
        listText.append(listName);
        listData.append(listV);
        if(listRow>9)
        {
            Mat matImage=Mat(Size(2220, 2200), CV_8UC3);
            matList.append(matImage);
        }
        matList.insert(listText.size()-1,listMat);
    }
    if(marks>0)
    {
        listText.insert(listRow,listName);
        listData.insert(listRow,listV);
        matList.insert(listRow,listMat);
    }


}

void MyImageList::updataListShow(QString listNames,Mat listMats,int listRow)
{

    // 将抓取到的帧，转换为QImage格式。QImage::Format_RGB888不同的摄像头用不同的格式。
    QImage images((const uchar*)listMats.data, listMats.cols,
                  listMats.rows,listMats.cols*listMats.channels(),QImage::Format_RGB888);

    //    QPixmap pixmapCV = QPixmap::fromImage(images.scaled(size(), Qt::KeepAspectRatio) );
     QPixmap pixmapCV =QPixmap::fromImage(images);

    // QString strg=strName;
    QListWidgetItem *LItem = new QListWidgetItem(QIcon(pixmapCV.scaled(50,50)), listNames);
    ui->listWidget->setIconSize(QSize(70, 80));
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setMovement(QListView::Static);
    LItem->setSizeHint(QSize(70,80));
    ui->listWidget->insertItem(listRow,LItem);

}

 void MyImageList::updataListShows(int marks)
 {
     if(marks==1)
     {
         ui->listWidget->clear();
         int j=listText.size();
//         QString str=QString::number(j,10,0); // 这是你的变量
//         QMessageBox mesg;
//         mesg.about(NULL,"信息" , str);

         for ( int i=0; i<j; ++i )
         {
             showListItem(matList.at(i),listData.at(i),listText.at(i));
         }
     }

 }
void MyImageList::receiveDataWM(Mat imageData,QString textData,QString vData,int rowData,int marks)
{
//    QString str=QString::number(a,10,0); // 这是你的变量
//    QString str=textData;//"收到信号";
//    QMessageBox mesg;
//    mesg.about(NULL,"信息" , str);
    intoList(textData,vData,imageData,rowData,marks);

}

void MyImageList::receiveAction()//收到消息刷新列表
{
    updataListShows(1);
}

void MyImageList::showTableView(Mat imageData,QString textData,QString strVariable,int rows)
{

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAutoScroll(true);
    QStandardItemModel *model = new QStandardItemModel;
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->hide();

    model->setItem(0,0,new QStandardItem("名称： "));
    model->setItem(1,0,new QStandardItem("变量： "));
    model->setItem(2,0,new QStandardItem("宽度： "));
    model->setItem(3,0,new QStandardItem("高度： "));
    model->setItem(4,0,new QStandardItem("其他： "));

    model->setItem(0,1,new QStandardItem(textData));//图像操作名称

    QString strh=QString::number(imageData.cols,10,0);
    QString strw=QString::number(imageData.rows,10,0);

   // QString strVariable= QString("image%1").arg(rows);

    model->setItem(1,1,new QStandardItem(strVariable));
    model->setItem(2,1,new QStandardItem(strh));
    model->setItem(3,1,new QStandardItem(strw));
    model->setItem(4,1,new QStandardItem(""));
    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(1);
}


