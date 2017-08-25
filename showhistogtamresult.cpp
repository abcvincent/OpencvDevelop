#include "showhistogtamresult.h"
#include "ui_showhistogtamresult.h"

ShowHistogtamResult::ShowHistogtamResult(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowHistogtamResult)
{
    ui->setupUi(this);
}

ShowHistogtamResult::~ShowHistogtamResult()
{
    delete ui;
}
