#include "showhistogtamresult.h"
#include "ui_showhistogtamresult.h"
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QPalette>

#pragma execution_character_set("utf-8")



///////////////////////////////////
/// \brief ShowHistogtamResult::ShowHistogtamResult
/// \param parent
///直方图显示程序会让程序特别的卡，可能是paintEvent(QPaintEvent *)的缘故
/// 目前没有更好的方法解决个bug;
///
///
///
///

ShowHistogtamResult::ShowHistogtamResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowHistogtamResult)
{
    ui->setupUi(this);

  ui->show_label->setScaledContents(true);//自动调整为Qlabel大小。

//  QWidget *widget = new QWidget();
//       widget->setAutoFillBackground(true);
//       QPalette palette;
//       QPixmap pixmap(":/Resources/Penguins.jpg");
//       palette.setBrush(QPalette::Window, QBrush(pixmap));
//       widget->setPalette(palette);
//       widget->show();

this->setAutoFillBackground(true);


}

ShowHistogtamResult::~ShowHistogtamResult()
{
    delete ui;
}

void ShowHistogtamResult::showHistogtam_Mat(Mat receive_Mat)
{
    source_gray_Mat=receive_Mat;
    setPicture(resultMat);

}


void ShowHistogtamResult::setPicture(Mat his_Mat)
{
    ui->show_label->clear();

    QImage image;
    Mat pImgs=his_Mat;//m_listMat_glob.at(0);

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

//    image = image.scaled(this->geometry().size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    QPixmap pixmapCV =QPixmap::fromImage(image);
//    ui->show_label->setPixmap(pixmapCV);
//    ui->show_label->setAlignment(Qt::AlignCenter);



    QPalette palette;
    palette.setBrush(this->backgroundRole(),
                     QBrush(pixmapCV.scaled(this->size(),
                                            Qt::IgnoreAspectRatio,
                                            Qt::SmoothTransformation)));

    this->setPalette(palette);

}

// void HistogtamDlg::closeEvent(QCloseEvent *event)
// {
//     //在这里添加你希望执行关闭事件需要处理的事情
//     //弹出消息框，关闭其他窗口
// }

void ShowHistogtamResult::paintEvent(QPaintEvent *)
{
//    ui->show_label->clear();
//    ui->show_label->resize(this->geometry().size());
//    setPicture(resultMat);
    reFresh();

}



void ShowHistogtamResult::reFresh()
{
//    ui->show_label->clear();
    setPicture(resultMat);
}


void ShowHistogtamResult::receiveHistogtam_Mat(Mat re_Mat)
{
    ui->show_label->clear();
    resultMat=re_Mat;
    setPicture(resultMat);
 }
