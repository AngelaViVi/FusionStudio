#pragma once

#include <QWidget>
#include "ui_classification.h"
#include <QProcess>
#include <QString>

/*
���:��ʹ΢����
ʱ��:2017��5��1��14:50:37
��ע:�������û�취ʹ���м��������Qmake,����ֻ��ʹ���ⲿί����ʵ��
     �������,�����̵߳�ָ��Ĵ��ݱȽ��вο���ֵ
*/
class Classification : public QWidget
{
	Q_OBJECT

public:
	Classification(QString coredir,QProcess *addprocess,QWidget *parent = Q_NULLPTR);
	~Classification();

private:
	Ui::Classification ui;
	QString CoreDir;//�������Ŀ¼
	QProcess *_AdditionalProcess;//ָ�������������Ѿ��ض�����Ǹ��߳�,ǳ����,���츳ֵ
public slots:
	void Browser1();
	void Browser2();
	void Browser3();
	void Browser4();
	void Browser5();
	void DoClassification();
	void ZoomImage();

};
