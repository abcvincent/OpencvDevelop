#ifndef VARIATEDIALOG_H
#define VARIATEDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QFormLayout>
#include <QFrame>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>

#include <math.h>
#include <QWheelEvent>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>


#pragma execution_character_set("utf-8")


//namespace Ui {
//class VariateDialog;
//}

class VariateDialog : public QDialog
{
    Q_OBJECT

public:
    //dialog初始化函数，窗口标题，输入变量列表，算子解释，算子变量数量，算子变量列表，
    VariateDialog(QString title,
                  QList<QString> ListVariateDlg,
                  QString text,
                  int VariaeCount,
                  QList<QString> ListCV_CodeDlg1,
                  QList<QString> ListCV_CodeDlg2,
                  QList<QString> ListCV_CodeDlg3);

    QString m_title;
    QString m_text;
    QList<QString> m_listVatiate;
    QList<QString> m_listCV_Code1;
    QList<QString> m_listCV_Code2;
    QList<QString> m_listCV_Code3;

    QString comboxStr;
    QString lineEditStr;

//    QString comboxVarStr1;
//    QString comboxVarStr2;
//    QString comboxVarStr3;

    QComboBox *combox;
    QComboBox *comboxVar1;
    QComboBox *comboxVar2;
    QComboBox *comboxVar3;


    QLineEdit *lineEditVar1;
    QLineEdit *lineEditVar2;
    QLineEdit *lineEditVar3;


    QString lineEditStr1;
    QString lineEditStr2;
    QString lineEditStr3;



    int cancelDlg;


private slots:
//    void getVarDlg1(QString);
//    void getVarDlg2(QString);
//    void getVarDlg3(QString);
//    void getVarDlg4(QString);
    void sendDlg(bool);
    void getLineEdit(QString);
    void getLineEdit1(QString);
    void getLineEdit2(QString);
    void getLineEdit3(QString);

//signals:
//    void sendVarDlg(QString,QString,int,int);

};

#endif // VARIATEDIALOG_H
