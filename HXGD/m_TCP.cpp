#include "m_TCP.h"

m_TCPClient::m_TCPClient(QString ip, qint16 port)
{
	this->ip = ip;
	this->port = port;
}

m_TCPClient::~m_TCPClient()
{
}

void m_TCPClient::Init()
{
	//�ͻ���
	timerHeartBeat = new QTimer();
	timerReconnect = new QTimer();
	timerInitConnect = new QTimer();
	tcpSocket = new QTcpSocket();
	timerHeartBeat->setInterval(10000);//ÿ��10S����������
	connect(timerHeartBeat,&QTimer::timeout,this,&m_TCPClient::slotHeartBeat);
	connect(tcpSocket, &QTcpSocket::connected, this, &m_TCPClient::slotIsConnection);
	connect(tcpSocket, &QTcpSocket::disconnected, this, &m_TCPClient::slotIsDisconnect);
	connect(timerInitConnect, &QTimer::timeout, this, &m_TCPClient::ConnectToServer);
	timerInitConnect->start(2000);
}

void m_TCPClient::ConnectToServer()
{
	// δ����ʱ��ÿ�������Զ���ָ��������������������
	if (tcpSocket->state() == QAbstractSocket::UnconnectedState)
	{
		qDebug() << "connect state" << tcpSocket->state();
		tcpSocket->abort();
		tcpSocket->connectToHost(ip, port);
		slotClientSendMessage();
	}
	// ���ӳɹ���رճ�ʼ�����Ӷ�ʱ��������Ķ�����������޹�
	else if (tcpSocket->state() == QAbstractSocket::ConnectedState)
	{
		timerInitConnect->stop();
	}
}

void m_TCPClient::slotIsConnection()
{
	qDebug() << "connect sucess";
	count = 1;
	timerHeartBeat->start();//��ʼ����
}

void m_TCPClient::slotIsDisconnect()
{
	count = 1;
	qDebug() << "*******�Ͽ�*******";
	timerHeartBeat->stop();//�Ͽ����Ӻ�����ֹͣ
	connect(timerReconnect, &QTimer::timeout, this, &m_TCPClient::slotReconnect);
	timerReconnect->start(5000);//ÿ5�����Զ�����
}

void m_TCPClient::slotReconnect()
{
	if (tcpSocket->state() == QAbstractSocket::UnconnectedState)
	{
		tcpSocket->abort();
		qDebug() <<
			"connect tring...("
			<< count++ <<
			")" <<count++ ;
		tcpSocket->connectToHost(ip, port);
		
	}
	else if (tcpSocket->state() == QAbstractSocket::ConnectedState)
	{
		timerReconnect->stop();//
	}
}

void m_TCPClient::slotClientSendMessage()
{
	QString str = "qqqqqqqqqqqq";
	int ret = tcpSocket->write(str.toUtf8().data());
}



void m_TCPClient::slotHeartBeat()
{
	qDebug() << tr("ʱ�䵽������������");
	// �������ĸ�ʽ����
	QString heart = "heartPackage$";
	QByteArray ba = heart.toLocal8Bit();
	int ret = tcpSocket->write(ba);
	if (ret < 0)
	{
		qDebug() << tr("���Ͳ��ɹ�");
		//ToDo::����


	}
}


//��������
m_TCPServer::m_TCPServer(qint16 port)
{
	//�����
	tcpServer = new QTcpServer();
	bool b = tcpServer->listen(QHostAddress::Any, port);
	connect(tcpServer, &QTcpServer::newConnection, [=]() {
		//ȡ�����������ӵĵ��׽���
		tcpSocketServer = tcpServer->nextPendingConnection();
		//��ȡ�Է���IP�Ͷ˿�
		QString tempip = tcpSocketServer->peerAddress().toString();
		qint16  tempport = tcpSocketServer->peerPort();
		QString temp = QString("[%1:%2]:�ɹ�����").arg(tempip).arg(tempport);
		//ui->textEditRead->setText(temp);
		connect(tcpSocketServer, &QTcpSocket::readyRead, [=]() {
			//��ͨ���׽�����ȡ������
			QByteArray array = tcpSocketServer->readAll();
			//ui->textEditRead->append(array);
			qDebug() << array;
			});
		});

}

m_TCPServer::~m_TCPServer()
{

}

void m_TCPServer::slotServerSendMessage()
{
	////��ȡ�༭��������
	//QString str = ui->textEditWrite->toPlainText();
	//tcpSocket->write(str.toUtf8().data());

}