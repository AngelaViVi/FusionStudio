#pragma once

#include <QObject>
/*
���:��ʹ΢����
�޸�ʱ��:2017��1��29��17:39:01
����:��װһ�����̵���������
��ע:��Ӧ����һ��������,ͬһʱ�����ֻ����һ����Ծ�Ĺ���
*/
class Project : public QObject
{
	Q_OBJECT

public:
	///���������
	Project(QObject *parent = 0);
	~Project();
private:
	///���̵�˽������
	QString WorkingDir;          //�ù��̵Ĺ���Ŀ¼
public:
	///���Ե�get��set����
	void setWorkingDir(QString);//����Ŀ¼��set����
	QString getWorkingDir();    //����Ŀ¼��get����

public:
	///���̵Ĺ��з���

private:
	///���̵�˽�з���

signals :
	void ValueChanged();        //�Զ���ֵ�����ź�
};
