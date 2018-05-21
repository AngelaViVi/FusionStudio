#pragma once

#include <QWidget>
#include "ui_featureexp.h"
#include <QProcess>

class FeatureExp : public QWidget
{
	Q_OBJECT

public:
	FeatureExp(QString coredir, QProcess *addprocess, QWidget *parent = Q_NULLPTR);
	~FeatureExp();

private:
	Ui::FeatureExp ui;
	QString CoreDir;//�������Ŀ¼
	QProcess *_AdditionalProcess;//ָ�������������Ѿ��ض�����Ǹ��߳�,ǳ����,���츳ֵ
public slots:
	void Browser1();
	void Browser2();
	void Browser3();
	void Verification();
	void DoFeatureExp();
};
