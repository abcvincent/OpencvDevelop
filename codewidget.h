#ifndef CODEWIDGET_H
#define CODEWIDGET_H

#include <QWidget>

namespace Ui {
class codeWidget;
}

class codeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit codeWidget(QWidget *parent = 0);
    ~codeWidget();

private slots:
    void receiveStr(int,QString);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::codeWidget *ui;

    int fontsize;
};

#endif // CODEWIDGET_H
