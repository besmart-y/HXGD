#pragma once
# pragma execution_character_set("utf-8")  
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qtimer.h>
#include <qwidget.h>

class m_TCPClient:public QWidget
{
	Q_OBJECT
public:
	m_TCPClient(QString ip,qint16 port);
	~m_TCPClient();
	void Init();
	void ConnectToServer();

private:
	QTimer* timerHeartBeat;
	QTimer* timerReconnect;
	QTimer* timerInitConnect;
	//�ͻ���ͨ��
	QTcpSocket* tcpSocket=NULL;
	
	bool isClient = false;
	int count = 1;//��������
	QString ip = "";
	qint16 port = 0;

public slots:
	void slotHeartBeat();
	void slotIsConnection();
	void slotIsDisconnect();
	void slotReconnect();
	void slotClientSendMessage();
	
};

class m_TCPServer:public QWidget
{
	Q_OBJECT
public:
	m_TCPServer(qint16 port);
	~m_TCPServer();

private:
	bool isServer = false;
	QString ip = "";
	qint16 port;
	//������ͨ��
	QTcpSocket* tcpSocketServer = NULL;
	//����������
	QTcpServer* tcpServer = NULL;
public slots:
	void slotServerSendMessage();

};



