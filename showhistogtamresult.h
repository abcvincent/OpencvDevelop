#ifndef SHOWHISTOGTAMRESULT_H
#define SHOWHISTOGTAMRESULT_H

#include <QWidget>
#include "histogtamdlg.h"

#include "math.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

namespace Ui {
class ShowHistogtamResult;
}

class ShowHistogtamResult : public QWidget
{
    Q_OBJECT

public:
    explicit ShowHistogtamResult(QWidget *parent = 0);
    ~ShowHistogtamResult();

    void setPicture(Mat);
    void showHistogtam_Mat(Mat);
    void reFresh();
    Mat source_gray_Mat;
    Mat resultMat;


private:
    Ui::ShowHistogtamResult *ui;
    void paintEvent(QPaintEvent *);


private slots:
    void receiveHistogtam_Mat(Mat);

};
#endif // SHOWHISTOGTAMRESULT_H
