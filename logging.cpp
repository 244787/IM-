#include "logging.h"
#include "ui_logging.h"
#include <QBitmap>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include<QPropertyAnimation>
#include <QThread>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QTimer>
#include <QShortcut>
#include<QtDebug>
#include<QRegExp>
void logging::mousePressEvent(QMouseEvent *event)
{
    mouseWindowTopLeft = event->pos();
}

void logging::mouseMoveEvent(QMouseEvent *event)
{
    //qDebug()<<"鼠标移动\n";
    if(event->buttons() & Qt::LeftButton){
        //qDebug()<<"鼠标移动\n";
        mouseDeskTopLeft= event->globalPos();
        windowDeskTopLeft = mouseDeskTopLeft - mouseWindowTopLeft;
        this->move(windowDeskTopLeft);

    }
}

void logging::PageSwitch(int Pageindex)
{

    if(Pageindex<0||Pageindex>=ui->stackedWidget->count()){
        return;
    }
    ui->stackedWidget->setCurrentIndex(Pageindex);
}

logging::logging(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::logging)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint); // 最小化按钮
    setWindowFlags(windowFlags() | Qt::WindowContextHelpButtonHint); // 帮助按钮

    int width = this->width()-10;
    int height = this->height()-10;
    ui->centerWidget->setGeometry(5,5,width,height);
    ui->centerWidget->setStyleSheet("QWidget{border-radius:4px;background:rgba(255,255,255,1);}");  //设置圆角

    this->setWindowFlags(Qt::FramelessWindowHint);          //去掉标题栏无边框
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor(39,40,43,100));
    //设置阴影圆角
    shadow->setBlurRadius(10);
    //给嵌套QWidget设置阴影
    ui->centerWidget->setGraphicsEffect(shadow);
    t= new TcpClient(this);
    msgBox = new QMessageBox();
    accExp = QRegExp("^[0-9A-Za-z_]{3,10}$");
    pwdExp = QRegExp("^.*(?=.{6,20})(?=.*\d)(?=.*[A-Z])(?=.*[a-z])(?=.*[!@#$%^&*?]).*$");
    nicExp = QRegExp("^[0-9A-Za-z_]{3,10}$");
    ui->lineEdit_accout->setValidator(new QRegExpValidator(accExp,this));
    ui->lineEdit_password->setValidator(new QRegExpValidator(pwdExp,this));
    ui->lineEdit_accout2->setValidator(new QRegExpValidator(accExp,this));
    ui->lineEdit_password2->setValidator(new QRegExpValidator(pwdExp,this));
    ui->lineEdit_passwordConfirm->setValidator(new QRegExpValidator(pwdExp,this));
    ui->lineEdit_nickname->setValidator(new QRegExpValidator(nicExp,this));
    ui->pushButton_login->setEnabled(false);
    ui->pushButton_regist2->setEnabled(false);
    conn();
}

logging::~logging()
{
    delete ui;
}

void logging::on_pushButton_login_clicked()
{
    if(!t->m_isConnected){
        msgBox->setText("连接服务器失败，请检查网络设置");
        msgBox->showNormal();
        return;
    }
    QJsonObject msg;
    msg.insert("cmd",cmd_login);
    msg.insert("account",ui->lineEdit_accout->text());
    msg.insert("password",ui->lineEdit_password->text());
    t->SendMsg(msg);
}

void logging::on_pushButton_regist_clicked()
{
    PageSwitch(1);
}

void logging::on_pushButton_seePassword_clicked()
{
    if(ui->lineEdit_password->echoMode() == QLineEdit::Password)
            ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    else
           ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

void logging::on_pushButton_hide_clicked()
{
    QWidget* pWindow = this->window();
    if(pWindow->isWindow()){
        pWindow->showMinimized();
    }
}

void logging::on_pushButton_close_clicked()
{
    QJsonObject msg;
    msg.insert("cmd",cmd_logout);
    t->SendMsg(msg);
    this->close();
}

void logging::on_pushButton_hide2_clicked()
{
    QWidget* pWindow = this->window();
    if(pWindow->isWindow()){
        pWindow->showMinimized();
    }
}

void logging::on_pushButton_close2_clicked()
{
    QJsonObject msg;
    msg.insert("cmd",cmd_logout);
    t->SendMsg(msg);
    this->close();
}

void logging::on_pushButton_regist2_clicked()
{
    if(!t->m_isConnected){
        msgBox->setText("网络连接错误");
        msgBox->showNormal();
        return;
    }
    if(ui->lineEdit_password2->text()!=ui->lineEdit_passwordConfirm->text()){
        msgBox->setText("两次输入的密码不一致");
       msgBox->showNormal();
       return;
    }
    QJsonObject msg;
    msg.insert("cmd",cmd_regist);
    msg.insert("account",ui->lineEdit_accout2->text());
    msg.insert("password",ui->lineEdit_password2->text());
    msg.insert("nickname",ui->lineEdit_nickname->text());
    t->SendMsg(msg);

}

void logging::on_pushButton_return_clicked()
{
    PageSwitch(0);
}

void logging::on_lineEdit_textChanged()
{
    if(ui->lineEdit_accout->text().length() &&
            ui->lineEdit_password->text().length()){
        ui->pushButton_login->setEnabled(true);
     }
    else
        ui->pushButton_login->setEnabled(false);

}

void logging::on_lineEdit_textChanged2()
{
    if(ui->lineEdit_accout2->text().length() &&
            ui->lineEdit_password2->text().length()&&
            ui->lineEdit_passwordConfirm->text().length()&&
            ui->lineEdit_nickname->text().length()){
        ui->pushButton_regist2->setEnabled(true);
     }
    else
        ui->pushButton_regist2->setEnabled(false);
}

void logging::conn()
{
    connect(ui->lineEdit_accout,&QLineEdit::textChanged,this,&logging::on_lineEdit_textChanged );
    connect(ui->lineEdit_password,&QLineEdit::textChanged,this,&logging::on_lineEdit_textChanged );
    connect(ui->lineEdit_accout2,&QLineEdit::textChanged,this,&logging::on_lineEdit_textChanged2 );
    connect(ui->lineEdit_password2,&QLineEdit::textChanged,this,&logging::on_lineEdit_textChanged2 );
    connect(ui->lineEdit_passwordConfirm,&QLineEdit::textChanged,this,&logging::on_lineEdit_textChanged2 );
    connect(ui->lineEdit_nickname,&QLineEdit::textChanged,this,&logging::on_lineEdit_textChanged2 );
}
