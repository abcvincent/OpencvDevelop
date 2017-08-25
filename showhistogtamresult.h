#ifndef SHOWHISTOGTAMRESULT_H
#define SHOWHISTOGTAMRESULT_H

#include <QWidget>

namespace Ui {
class ShowHistogtamResult;
}

class ShowHistogtamResult : public QWidget
{
    Q_OBJECT

public:
    explicit ShowHistogtamResult(QWidget *parent = 0);
    ~ShowHistogtamResult();

private:
    Ui::ShowHistogtamResult *ui;
};

#endif // SHOWHISTOGTAMRESULT_H
