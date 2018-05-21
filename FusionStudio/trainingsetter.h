#pragma once

#include <QWidget>
#include "ui_trainingsetter.h"
#include <QProcess>
#include <QString>

class TrainingSetter : public QWidget
{
	Q_OBJECT

public:
	TrainingSetter(QString coredir, QProcess *addprocess,QWidget *parent = Q_NULLPTR);
	~TrainingSetter();

private:
	///�ڲ�����
	Ui::TrainingSetter ui;
	QString CoreDir;//�������Ŀ¼
	QProcess *_AdditionalProcess;//ָ�������������Ѿ��ض�����Ǹ��߳�,ǳ����,���츳ֵ
private:
	///�ڲ�����
public slots :
	///�ۺ���
	void BrowseSolver();//���ѵ�������ļ�
	void BrowseSnapPath();
	void BrowseNet();
	void ReadSolver();
	void SaveSolver();
	void ExecuteTraining();
	void policyChange();//˥�����Ա��
};
