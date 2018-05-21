#pragma once

#include <QWidget>
#include "ui_browsedatabase.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include "DbOperator.h"

class BrowseDatabase : public QWidget
{
	Q_OBJECT

public:
	BrowseDatabase(QWidget *parent = Q_NULLPTR);
	~BrowseDatabase();

private:
	Ui::BrowseDatabase ui;
	DbOperator *MyDbHandle;//���ݿ�������
private:
	QImage MatToQImage(const cv::Mat&);
public slots:
	void LoadDatabase();//
	void Nextimage();
	void ZoomImage();
	void CrashWarning();
};
