#pragma once

#include <QWidget>
#include "ui_preferences.h"
#include "QStandardItemModel.h"
/*
���:��ʹ΢����
����ʱ��;2017��2��20��09:06:24
��ע:1.��������ڹ������������ļ�,����ѡ�����õ����ļ�����,�����⿪�ŷ��ʽӿ�.
     2.�������нϸߵ����ȼ�,���ĳ�ʼ��˳������������֮ǰ.
	 3.�ṩһ������,�ú���������ѡ�������.
	 4.����Ӧ�ù̻�һ��Ĭ�ϲ�����������Ĭ�ϵ������ļ����߶������ļ����г�ʼ��.
	 5.��ȡһ�����Ե�ʱ��,���Ȳ��������ļ�,�����ȡʧ�ܺ�����֤ʧ��,���ȡĬ�ϲ���.
*/

class Preferences : public QWidget
{
	Q_OBJECT

public:
	Preferences(QWidget *parent = Q_NULLPTR);
	~Preferences();
private:
	Ui::Preferences ui;
///ҵ�����
private:
	void initialize();//��ʼ��
	void iniFileSwitch(QString path);//�����ļ��л�
	
	///���ļ���ȡ,�����ⷵ��NULL
	QString readInstallPath();//��ȡinstallĿ¼
	QString readBinPath();//��ȡbinĿ¼
	QString readLibPath();//��ȡLibĿ¼
	QString readScript(QString Func);//��ȡ��Ӧfunc���ܵĽű�
	///д���ļ�
	void writeInstallPath(QString path);//д��installĿ¼
	void writeBinPath(QString path);//д��binĿ¼
	void writeLibPath(QString path);//д��LibĿ¼
	void writeScript(QString Func,QString script);//д���Ӧfunc���ܵĽű�
public:
	void awakenPreferencesManager();//������ѡ�������
	///�Ӵ����ⲿ��������
	QString defaultInstallPath;//Ĭ��installĿ¼
	QString defaultBinPath;//Ĭ��binĿ¼
	QString defaultLibPath;//Ĭ��LibĿ¼
	QVector <QString> FuncList;//�洢�ű���key,��������Ϊ��Ӧ�ű��Ĺ���
	QMap <QString, QString> Scripts;//�洢���еĽű�
	QString defaultScript_Train;//Ĭ�Ͻű�
public:
	///����Ĭ�ϲ���
	QString getInstallPath();//��ȡinstallĿ¼
	QString getBinPath();//��ȡbinĿ¼
	QString getLibPath();//��ȡLibĿ¼
	QString getScript(QString Func);//��ȡ��Ӧfunc���ܵĽű�
private:
	///ά�ֱ�����������������ڲ�����
	QString IniFilePath;//���浱ǰ��ʹ�õ������ļ�·��
	QStandardItemModel  *ScriptsModel;//������ģ��
private slots:
	void Browser1();
	void Browser2();
	void Browser3();
	void checkComponent();
	void setDefaultPath();
	void creatEnvironmentVariable();
	void setDefaultScripts();
	void save();
};
