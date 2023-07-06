#include "tcpclient.h"



TcpClient::TcpClient(QObject *parent):QTcpSocket(parent)
{
    buffer.clear();
    ConnectToServer();
    connect(this,&QTcpSocket::readyRead,this,&TcpClient::onReadyRead);

}

TcpClient::~TcpClient()
{
    DisconnectFromServer();
}

int TcpClient::ConnectToServer()
{
    if(m_isConnected){
        return 1;
    }
    this->connectToHost(m_server,m_port);
    if(this->waitForConnected(3000)){
        qDebug()<<"connect to server successfully\n";
        m_isConnected = true;
        return 0;
    }else{
        qDebug()<<"connect to server failed\n";
        return -1;
    }
}

void TcpClient::DisconnectFromServer()
{
    qDebug()<<"disconnect from server\n";
    m_isConnected = false;
}

void TcpClient::SendMsg(QJsonObject Msg)
{
    if(!m_isConnected){
        return ;
    }
    QJsonDocument doc(Msg);
    QByteArray data = doc.toJson(QJsonDocument::Compact); //让数据更紧凑
    int dataLength = data.length();
    char lenBuf[4];
    memcpy(lenBuf,&dataLength,4);
    this->write(lenBuf,4);
    this->write(data);
    this->waitForBytesWritten(1000);
    qDebug()<<"sendLen: "<<dataLength<<"\n";

}

void TcpClient::CmdParse(QJsonObject message)
{
    QJsonObject msg(message);
    int cmd = msg["cmd"].toInt();
    if(cmd == cmd_login || cmd == cmd_regist)
    {
        emit CallLogging(msg);
    }
    if(cmd >= cmd_add_friend_request && cmd <= cmd_group_member_list)
    {
        emit CallClient(msg);
    }
    if(cmd == cmd_friend_search || cmd == cmd_group_search)
    {
        emit CallAddFriend(msg);
    }
}

void TcpClient::onReadyRead()
{
    buffer.append(this->readAll());
    while(buffer.size()>=4){
        int len;
        memcpy(&len,buffer.constData(),4);
        if(buffer.size()>=len+4){
            //包含完整的一个信息包
            QByteArray data = buffer.mid(4,len);
            buffer.remove(0,len+4);
            QJsonDocument doc = QJsonDocument::fromJson(data);
            message = doc.object();
            qDebug() << "readsize: " << len;
            qDebug() << "realReadSize: " << data.length();
            qDebug() << "read: " << data;
            CmdParse(message);
        }
    }
}









