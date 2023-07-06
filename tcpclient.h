#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include  <QNetworkRequest>
#include <QNetworkReply>
#include <QVector>
#include <utility>

enum command{
    cmd_regist = 0,
    cmd_login,
    cmd_logout,
    cmd_friend_search,
    cmd_add_friend_request,
    cmd_add_friend_response,
    cmd_friend_list,
    cmd_friend_chat,

    cmd_group_create,
    cmd_group_search,
    cmd_group_join_search,
    cmd_group_join_request,
    cmd_group_list,
    cmd_group_chat,
    cmd_group_member_list,
    cmd_group_member_add,
    cmd_group_member_del,

    cmd_set_icon


};

enum ContentType{
    textOnly,ImageOnly,MixedContent
};

typedef struct selfInfo{
    int account =0;
    QString password;
    QString name;
    QString sig;
    QString icon;
}Selfinfo ;

typedef struct friendInfo{
    int account =0;
    QString name;
    QString sig;
    bool isOnline = false;
    QString icon;
    friendInfo(int _account,QString _name){
        name=std::move(_name);account=_account;
    }
    friendInfo()= default;

}FriendInfo;

typedef struct member_info
{
    int account = 0;
    QString name;
}MemberInfo;

typedef struct group_info
{
    int groupAccount;
    QString groupName;
    QVector <MemberInfo> memberList;
}GroupInfo;


class TcpClient :public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(QObject * parent = nullptr);
    ~TcpClient() override;
    int ConnectToServer();
    void DisconnectFromServer();
    void SendMsg(QJsonObject Msg);
    void CmdParse(QJsonObject message);
Q_SIGNALS:
    void CallClient(QJsonObject msg);
    void CallLogging(QJsonObject msg);
    void CallAddFriend(QJsonObject msg);
public slots:
    void onReadyRead();
public:
    QString m_server = "192.168.77.145";
    int m_port = 2339;
    QJsonObject message;
    QByteArray buffer;
    bool m_isConnected;


};

#endif // TCPCLIENT_H
