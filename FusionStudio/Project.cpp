#include "Project.h"

/*
���:��ʹ΢����
�޸�ʱ��:2017��1��29��17:39:01
�������ʵ�ֺ���
*/

Project::Project(QObject *parent)
	: QObject(parent)
{
	WorkingDir = "";
}

Project::~Project()
{
}

void Project::setWorkingDir(QString NewValue)
{
	if (NewValue != WorkingDir)
	{
		WorkingDir = NewValue;
		emit ValueChanged();//ֵ����,�����ź�
	}
}

QString Project::getWorkingDir()
{
	return WorkingDir;
}
