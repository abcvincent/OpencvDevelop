#include "histogtamdlg.h"
#include "ui_histogtamdlg.h"

HistogtamDlg::HistogtamDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogtamDlg)
{
    ui->setupUi(this);
}

HistogtamDlg::~HistogtamDlg()
{
    delete ui;
}
