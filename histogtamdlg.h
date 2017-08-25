#ifndef HISTOGTAMDLG_H
#define HISTOGTAMDLG_H

#include <QWidget>

namespace Ui {
class HistogtamDlg;
}

class HistogtamDlg : public QWidget
{
    Q_OBJECT

public:
    explicit HistogtamDlg(QWidget *parent = 0);
    ~HistogtamDlg();

private:
    Ui::HistogtamDlg *ui;
};

#endif // HISTOGTAMDLG_H
