#ifndef LOGGING_H
#define LOGGING_H

#include <QWidget>
#include<QMessageBox>
#include<QMouseEvent>
#include"tcpclient.h"
#include<QRegExp>
namespace Ui {
class logging;
}
class logging : public QWidget
{
    Q_OBJECT
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
public:
    QPoint mouseWindowTopLeft; //鼠标相对于窗口左上角的坐标
    QPoint mouseDeskTopLeft;  //鼠标相对于桌面左上角坐标
    QPoint windowDeskTopLeft;  //窗口左上角相对于桌面左上角的坐标


public:
    void PageSwitch(int Pageindex);
    explicit logging(QWidget *parent = nullptr);
    ~logging();
    void conn();
private slots:

    void on_pushButton_login_clicked();

    void on_pushButton_regist_clicked();

    void on_pushButton_seePassword_clicked();

    void on_pushButton_hide_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_hide2_clicked();

    void on_pushButton_close2_clicked();

    void on_pushButton_regist2_clicked();

    void on_pushButton_return_clicked();
    void on_lineEdit_textChanged();
    void on_lineEdit_textChanged2();

private:
    Ui::logging *ui;
    QMessageBox *msgBox;
    TcpClient* t;
    QRegExp accExp;
    QRegExp pwdExp;
    QRegExp nicExp;

};

#endif // LOGGING_H
