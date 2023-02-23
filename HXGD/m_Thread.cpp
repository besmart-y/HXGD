#include "m_Thread.h"

m_Thread& m_Thread::get_Instance()
{
	static m_Thread Instance;
	return Instance;
}

m_Thread::m_Thread()
{
	
}

void m_Thread::addThread(QObject* t)
{
	QThread *t_thread = new QThread();
	t_thread->setObjectName(t->objectName());
	m_ThreadList.append(t_thread);
	
	t->moveToThread(t_thread);//把工作对象转移到线程t_thread里面
	connect(t_thread, &QThread::finished, t, &QObject::deleteLater);
	
	t_thread->start();
	//qWarning() << "m_ThreadList size: " << m_ThreadList.size();
}

void m_Thread::closeThread()
{
	for (auto temp : m_ThreadList)
	{
		temp->quit();
		temp->wait();
		temp->deleteLater();
	}
}

m_Thread::~m_Thread()
{

}