#pragma once

#include <QtWidgets/QMainWindow>
#include "QProcess"
#include "ui_fusionstudio.h"
#include "Project.h"
#include "trainingsetter.h"
#include "convertmnist.h"
#include "convertcifar.h"
#include "convertimage.h"
#include "browsedatabase.h"
#include "preferences.h"
#include "myhighlighter.h"
#include "typedef.h"
#include "editor.h"
#include "homepage.h"
#include "neteditor.h"
#include "classification.h"
#include "featureexp.h"
#include "about.h"

/*
���:��ʹ΢����
��������:2017��1��4��13:18:08
�ع�:2017��2��19��13:52:02
    �ع���ע:������������,����ʹ��һ���������,�Ա���ģ�黯��д
*/
class FusionStudio : public QMainWindow
{
	Q_OBJECT

public:
	FusionStudio(QWidget *parent = Q_NULLPTR);
	QProcess * p2;
	static void test();
private:
	///�ڲ�����
	Ui::FusionStudioClass ui;
	void configureUI();
	QPoint PrePosition;
	//QMenu *TreeViewMenu = NULL;
	QMenu *TreeViewMenu_Empty = NULL;//�ļ���Ϊ��ʱ���Ҽ������¼��˵�
	QMenu *TreeViewMenu_File = NULL;//�������ļ��ڵ��ϵ��Ҽ������¼��˵�
	QMenu *TreeViewMenu_Path = NULL;//�������ļ��нڵ��ϵ��Ҽ������¼��˵�
	Project * m_project;
	Preferences *m_PreferencesManager;//��ѡ�������,���ڹرղ�������
	QString CurrentTreeViewSelectedPath;//��ǰ�ļ�����ͼ�б�ѡ�е���Ŀ�ľ���·��
private:
	///�ڲ�����
	bool DelDir(const QString &path);
public slots:
	void start_read_output();//����̨��׼����ض���
	void Train();//���Կ���̨�ض�����
	void Refresh();//ˢ���ļ���
	void ShowEditor();
	void on_treeView_customContextMenuRequested(QPoint pos);
	//�ļ���->�Ҽ��˵�
	void newFile();//�½��ļ�
	void newFolder();//�½��ļ���
	void getAbsolutePath();//��ȡ����·��
	void openInSystemResourceManager();//��ϵͳ��Դ�������д�
	void deleteFileOrPath();//ɾ��·�����ļ�
	//�˵�->�ļ�
	void FolderCreater();
	void FolderUnfolder();
	void FolderCloser();
	//�˵�->��ͼ
	void changeVisible_Explorer();
	void changeVisible_Console();
	//�˵�->����
	void MnistConverter();
	void CifarConverter();
	void ImageConverter();
	void DatabaseBrowser();
	//�˵�->����
	void PreferencesSetter();//������ѡ�������
							 //�˵�->��Ŀ
	void WorkingDirScanner();//ɨ�蹤��Ŀ¼
	//�˵�->����
	void DoClassification();//����
	void DoFeaturesExp();//��������
	//�˵�->����
	void ShowHome();
	void ShowAbout();
};
