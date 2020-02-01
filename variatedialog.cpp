#include "variatedialog.h"
//#include "ui_variatedialog.h"
#include "mainwindow.h"
#include <QString>
#include <QMessageBox>

VariateDialog::VariateDialog(QString title,
                             QList<QString> ListVariateDlg,
                             QString text,
                             int VariaeCount,
                             QList<QString> ListCV_CodeDlg1,
                             QList<QString> ListCV_CodeDlg2,
                             QList<QString> ListCV_CodeDlg3)//:QDialog(parent)
{

    m_title=title;
    m_text=text;
    m_listVatiate=ListVariateDlg;
    m_listCV_Code1=ListCV_CodeDlg1;
    m_listCV_Code2=ListCV_CodeDlg2;
    m_listCV_Code3=ListCV_CodeDlg3;


    cancelDlg=0;

    if(m_title.isEmpty()) {m_title="无";}
    if(m_text.isEmpty())  {m_text="无";}

    setWindowTitle(m_title);
    setModal(true);
    setFixedSize(380,220+VariaeCount*30);

    combox=new QComboBox(this);

    QLabel *labName1=new QLabel(this);//算子标题
    QLabel *labName12=new QLabel(this);//算子解释
    QLabel *labName2=new QLabel(this);//输入变量
    QLabel *labName3=new QLabel(this);//输出变量

    QFrame *line=new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QFrame *line2=new QFrame(this);
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);

    QFont lb_font;
    lb_font.setPointSize(12);
    QPushButton *btOK=new QPushButton(this);
    QPushButton *btCancel=new QPushButton(this);
    QLineEdit *lineEdit1=new QLineEdit;//输出变量填写框
    QGridLayout *layouts=new QGridLayout;//创建对齐对象



    // 只能输入英文和数字
    // QRegExp regx("[a-zA-Z0-9]+$");
    QRegExp regx("^[a-zA-Z][a-zA-Z0-9_]*$");//开头必须英文
    QValidator *validator = new QRegExpValidator(regx,this);
    lineEdit1->setValidator(validator);
    //不能重复变量


    //    lineEdit2->setText("1");
    lineEdit1->setToolTip("只能输入英文及数字");
    btOK->setText("确定");
    btCancel->setText("取消");
    labName1->setText(m_title);
    labName12->setText(m_text);
    labName2->setText("输入对象 （Input src）：");
    labName3->setText("输出变量（Output dst）：");
    labName1->setFont(lb_font);//标题字体
    layouts->addWidget(labName1,0,1,1,2);
    layouts->addWidget(labName12,1,1,1,2);
    layouts->addWidget(line,2,1,1,2);
    layouts->addWidget(labName2,3,1,1,1);
    layouts->addWidget(combox,3,2,1,1);
    layouts->addWidget(labName3,4,1,1,1);
    layouts->addWidget(lineEdit1,4,2,1,1);



    for(int i=0;i<m_listVatiate.size();++i)
    {
        QString stri=m_listVatiate.at(i);//QString::number(i,10,0);
        combox->addItem(stri);
    }


    if(m_listCV_Code1.at(0)=="Yes_combox")//列表
    {
//        QMessageBox mesg;
//        mesg.about(NULL,"提示",m_listCV_Code1.at(0));
        comboxVar1=new QComboBox(this);//变量box
        for(int i=1;i<m_listCV_Code1.size();++i)
        {
            QString stri=m_listCV_Code1.at(i);//QString::number(i,10,0);
            comboxVar1->addItem(stri);
        }

        QLabel *labNameVar1=new QLabel(this);
        labNameVar1->setText("变量值1（int flipCode1）：");
        layouts->addWidget(labNameVar1,5,1,1,1);
        layouts->addWidget(comboxVar1,5,2,1,1);

    }


    if(m_listCV_Code1.at(0)=="Yes_Number_int")//只呢输入数字整数
    {
//        QMessageBox mesg;
//        mesg.about(NULL,"提示",m_listCV_Code1.at(0));
        lineEditVar1=new QLineEdit(this);
        QLabel *labNameVar1=new QLabel(this);
        labNameVar1->setText("变量值1（int flipCode1）：");
        layouts->addWidget(labNameVar1,5,1,1,1);
        layouts->addWidget(lineEditVar1,5,2,1,1);       

        //只能输入整数
        QRegExp rx("^-?\\d+$");
        QRegExpValidator *pReg = new QRegExpValidator(rx, this);
        lineEditVar1->setValidator(pReg);       
        connect(lineEditVar1,SIGNAL(textChanged(QString)),this,SLOT(getLineEdit1(QString)));
    }


    if(m_listCV_Code1.at(0)=="Yes_Number_double")//输入浮点数
    {
        lineEditVar1=new QLineEdit(this);
        QLabel *labNameVar1=new QLabel(this);
        labNameVar1->setText("变量值1（int flipCode1）：");
        layouts->addWidget(labNameVar1,5,1,1,1);
        layouts->addWidget(lineEditVar1,5,2,1,1);


        //限制浮点数输入范围为[-999999.9999,999999.9999]
        QRegExp rx("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)");
        QRegExpValidator *pReg = new QRegExpValidator(rx, this);
        lineEditVar1->setValidator(pReg);
        connect(lineEditVar1,SIGNAL(textChanged(QString)),this,SLOT(getLineEdit1(QString)));

    }


    if(m_listCV_Code2.at(0)=="Yes_combox")
    {

        comboxVar2=new QComboBox(this);//变量box
        for(int i=1;i<m_listCV_Code2.size();++i)
        {
            QString stri=m_listCV_Code2.at(i);//QString::number(i,10,0);
            comboxVar2->addItem(stri);
        }

        QLabel *labNameVar2=new QLabel(this);
        labNameVar2->setText("变量值2（int flipCode2）：");
        layouts->addWidget(labNameVar2,6,1,1,1);
        layouts->addWidget(comboxVar2,6,2,1,1);
    }



    if(m_listCV_Code2.at(0)=="Yes_Number_int")
    {


        lineEditVar2=new QLineEdit(this);
        QLabel *labNameVar2=new QLabel(this);
        labNameVar2->setText("变量值2（int flipCode2）：");
        layouts->addWidget(labNameVar2,6,1,1,1);
        layouts->addWidget(lineEditVar2,6,2,1,1);

        //只能输入整数
        QRegExp rx("^-?\\d+$");
        QRegExpValidator *pReg = new QRegExpValidator(rx, this);
        lineEditVar2->setValidator(pReg);
        connect(lineEditVar2,SIGNAL(textChanged(QString)),this,SLOT(getLineEdit2(QString)));
    }


    if(m_listCV_Code2.at(0)=="Yes_Number_double")//输入浮点数
    {

        lineEditVar2=new QLineEdit(this);
        QLabel *labNameVar2=new QLabel(this);
        labNameVar2->setText("变量值2（int flipCode2）：");
        layouts->addWidget(labNameVar2,6,1,1,1);
        layouts->addWidget(lineEditVar2,6,2,1,1);


        //限制浮点数输入范围为[-999999.9999,999999.9999]
        QRegExp rx("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)");
        QRegExpValidator *pReg = new QRegExpValidator(rx, this);
        lineEditVar2->setValidator(pReg);
        connect(lineEditVar2,SIGNAL(textChanged(QString)),this,SLOT(getLineEdit2(QString)));
    }



    if(m_listCV_Code3.at(0)=="Yes_combox")
    {


        comboxVar3=new QComboBox(this);//变量box
        for(int i=1;i<m_listCV_Code3.size();++i)
        {
            QString stri=m_listCV_Code3.at(i);//QString::number(i,10,0);
            comboxVar3->addItem(stri);
        }

        QLabel *labNameVar3=new QLabel(this);
        labNameVar3->setText("变量值3（int flipCode3）：");
        layouts->addWidget(labNameVar3,7,1,1,1);
        layouts->addWidget(comboxVar3,7,2,1,1);

    }




    if(m_listCV_Code3.at(0)=="Yes_Number_int")
    {

        lineEditVar3=new QLineEdit(this);
        QLabel *labNameVar3=new QLabel(this);
        labNameVar3->setText("变量值2（int flipCode2）：");
        layouts->addWidget(labNameVar3,7,1,1,1);
        layouts->addWidget(lineEditVar3,7,2,1,1);

        //只能输入整数
        QRegExp rx("^-?\\d+$");
        QRegExpValidator *pReg = new QRegExpValidator(rx, this);
        lineEditVar3->setValidator(pReg);
        connect(lineEditVar3,SIGNAL(textChanged(QString)),this,SLOT(getLineEdit3(QString)));
    }


    if(m_listCV_Code3.at(0)=="Yes_Number_double")//输入浮点数
    {

        lineEditVar3=new QLineEdit(this);
        QLabel *labNameVar3=new QLabel(this);
        labNameVar3->setText("变量值1（int flipCode1）：");
        layouts->addWidget(labNameVar3,7,1,1,1);
        layouts->addWidget(lineEditVar3,7,2,1,1);


        //限制浮点数输入范围为[-999999.9999,999999.9999]
        QRegExp rx("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)");
        QRegExpValidator *pReg = new QRegExpValidator(rx, this);
        lineEditVar3->setValidator(pReg);
        connect(lineEditVar3,SIGNAL(textChanged(QString)),this,SLOT(getLineEdit3(QString)));
    }

    //    comboxVar1=new QComboBox(this);//变量box
    //    comboxVar2=new QComboBox(this);//变量box
    //    comboxVar3=new QComboBox(this);//变量box

    //    QLabel *labName4=new QLabel(this);
    //    QLabel *labName5=new QLabel(this);

    //    QLineEdit *lineEdit2=new QLineEdit;
    //    QLineEdit *lineEdit3=new QLineEdit;

    //    lineEdit2->setEnabled(false);
    //    lineEdit3->setEnabled(false);

    //    labName4->setText("变量值1（int flipCode1）：");
    //    labName5->setText("变量值2（int flipCode2）：");


    //    //限制浮点数输入范围为[-999999.9999,999999.9999]
    //    QRegExp rx("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)");
    //    QRegExpValidator *pReg = new QRegExpValidator(rx, this);
    //    lineEdit2->setValidator(pReg);
    //    lineEdit3->setValidator(pReg);

    //    layouts->addWidget(labName4,5,1,1,1);
    //    layouts->addWidget(lineEdit2,5,2,1,1);
    //    layouts->addWidget(labName5,6,1,1,1);
    //    layouts->addWidget(lineEdit3,6,2,1,1);


    layouts->addWidget(line2,6+VariaeCount,1,1,2);
    layouts->addWidget(btCancel,7+VariaeCount,1,1,1);
    layouts->addWidget(btOK,7+VariaeCount,2,1,1);
    layouts->setVerticalSpacing(10);



    setLayout(layouts);

    // lineEditStr1=combox->currentText();
    // lineEditStr=lineEdit1->text();
    //    QString var1,var2;
    //    var1=lineEdit2->text();//输入变量1
    //    var2=lineEdit3->text();//输入变量2
    //    double val=str.toDouble();
    //    lineInt1=QString::number(var1,10,0);
    //    lineInt2=QString::number(var2,10,0);

    connect(btCancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(btOK,SIGNAL(clicked(bool)),this,SLOT(sendDlg(bool)));
    connect(lineEdit1,SIGNAL(textChanged(QString)),this,SLOT(getLineEdit(QString)));

}

void VariateDialog::sendDlg(bool)
{
    comboxStr=combox->currentText();

    if(m_listCV_Code1.at(0)=="Yes_combox") {lineEditStr1=comboxVar1->currentText();}
    if(m_listCV_Code2.at(0)=="Yes_combox") {lineEditStr2=comboxVar2->currentText();}
    if(m_listCV_Code3.at(0)=="Yes_combox") {lineEditStr3=comboxVar3->currentText();}

    cancelDlg=1;
    this->close();
}

void VariateDialog::getLineEdit(QString linestr)
{
    for(int i=0;i<m_listVatiate.size();++i)
    {
        if(linestr==m_listVatiate.at(i)){
            //QString str=QString::number(ui->mdiArea->subWindowList().size(),10,0);
            QMessageBox mesg;
            mesg.about(NULL,"提示","输出变量不能等于输入变量");
        }
        else{lineEditStr=linestr;}
    }
}

void VariateDialog::getLineEdit1(QString linestr)
{
    lineEditStr1=linestr;
}

void VariateDialog::getLineEdit2(QString linestr)
{
     lineEditStr2=linestr;
}


void VariateDialog::getLineEdit3(QString linestr)
{
     lineEditStr3=linestr;
}

