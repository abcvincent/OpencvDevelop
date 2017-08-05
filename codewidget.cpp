#include "codewidget.h"
#include "ui_codewidget.h"

codeWidget::codeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::codeWidget)
{
    ui->setupUi(this);
    fontsize=10;
}

codeWidget::~codeWidget()
{
    delete ui;
}

void codeWidget::receiveStr(int rows, QString codes)
{
    if(rows==0)
    {
        ui->textEdit->clear();
    }
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText(codes);//setText(codes);
    ui->textEdit->setFont(QFont("Timers" , fontsize ,  QFont::Bold));
    //insertPlainText("456");
}

void codeWidget::on_spinBox_valueChanged(int arg1)
{

    fontsize=arg1;
    ui->textEdit->setFont(QFont("Timers" , fontsize ,  QFont::Bold));
}
