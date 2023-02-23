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
	//客户端
	timerHeartBeat = new QTimer();
	timerReconnect = new QTimer();
	timerInitConnect = new QTimer();
	tcpSocket = new QTcpSocket();
	timerHeartBeat->setInterval(10000);//每隔10S发送心跳包
	connect(timerHeartBeat,&QTimer::timeout,this,&m_TCPClient::slotHeartBeat);
	connect(tcpSocket, &QTcpSocket::connected, this, &m_TCPClient::slotIsConnection);
	connect(tcpSocket, &QTcpSocket::disconnected, this, &m_TCPClient::slotIsDisconnect);
	connect(timerInitConnect, &QTimer::timeout, this, &m_TCPClient::ConnectToServer);
	timerInitConnect->start(2000);
}

void m_TCPClient::ConnectToServer()
{
	// 未连接时，每隔两秒自动向指定服务器发送连接请求
	if (tcpSocket->state() == QAbstractSocket::UnconnectedState)
	{
		qDebug() << "connect state" << tcpSocket->state();
		tcpSocket->abort();
		tcpSocket->connectToHost(ip, port);
		slotClientSendMessage();
	}
	// 连接成功后关闭初始化连接定时器，后面的断线重连与此无关
	else if (tcpSocket->state() == QAbstractSocket::ConnectedState)
	{
		timerInitConnect->stop();
	}
}

void m_TCPClient::slotIsConnection()
{
	qDebug() << "connect sucess";
	count = 1;
	timerHeartBeat->start();//开始心跳
}

void m_TCPClient::slotIsDisconnect()
{
	count = 1;
	qDebug() << "*******断开*******";
	timerHeartBeat->stop();//断开连接后心跳停止
	connect(timerReconnect, &QTimer::timeout, this, &m_TCPClient::slotReconnect);
	timerReconnect->start(5000);//每5秒钟自动重连
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
	qDebug() << tr("时间到，发送心跳包");
	// 心跳包的格式报文
	QString heart = "heartPackage$";
	QByteArray ba = heart.toLocal8Bit();
	int ret = tcpSocket->write(ba);
	if (ret < 0)
	{
		qDebug() << tr("发送不成功");
		//ToDo::报警


	}
}


//服务器类
m_TCPServer::m_TCPServer(qint16 port)
{
	//服务端
	tcpServer = new QTcpServer();
	bool b = tcpServer->listen(QHostAddress::Any, port);
	connect(tcpServer, &QTcpServer::newConnection, [=]() {
		//取出建立好连接的的套接字
		tcpSocketServer = tcpServer->nextPendingConnection();
		//获取对方的IP和端口
		QString tempip = tcpSocketServer->peerAddress().toString();
		qint16  tempport = tcpSocketServer->peerPort();
		QString temp = QString("[%1:%2]:成功连接").arg(tempip).arg(tempport);
		//ui->textEditRead->setText(temp);
		connect(tcpSocketServer, &QTcpSocket::readyRead, [=]() {
			//从通信套接字中取出内容
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
	////获取编辑区的内容
	//QString str = ui->textEditWrite->toPlainText();
	//tcpSocket->write(str.toUtf8().data());

}