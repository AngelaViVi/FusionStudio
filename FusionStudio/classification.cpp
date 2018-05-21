#include "classification.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/qmath.h>

Classification::Classification(QString coredir, QProcess *addprocess, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	CoreDir= coredir;
	_AdditionalProcess= addprocess;
}

Classification::~Classification()
{
}

void Classification::Browser1()
{
	ui.lineEdit_deploy->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("ѡ�����粿�������ļ�"),
			" ",
			QStringLiteral("��������(*.prototxt);;��������(*.*)")));
}

void Classification::Browser2()
{
	ui.lineEdit_caffemodel->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("ѡ��Ԥѵ�������ļ�"),
			" ",
			QStringLiteral("caffeԤѵ������(*.caffemodel);;��������(*.*)")));
}

void Classification::Browser3()
{
	ui.lineEdit_binaryproto->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("ѡ��ͼƬ��ֵ�ļ�"),
			" ",
			QStringLiteral("ͼƬ��ֵ(*.binaryproto);;��������(*.*)")));
}

void Classification::Browser4()
{
	ui.lineEdit_labels->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("ѡ���ǩ�ǵ��ļ�"),
			" ",
			QStringLiteral("��ǩ�ǵ�(*.txt);;��������(*.*)")));
}

void Classification::Browser5()
{
	ui.lineEdit_img->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("ѡ��ͼƬ"),
			" ",
			QStringLiteral("ͼƬ(*.jpg);;��������(*.*)")));
}
//����
void Classification::DoClassification()
{
	//У�����
	if (ui.lineEdit_deploy->text().toStdString() != ""&&
		ui.lineEdit_caffemodel->text().toStdString() != ""&&
		ui.lineEdit_binaryproto->text().toStdString() != ""&&
		ui.lineEdit_labels->text().toStdString() != ""&&
		ui.lineEdit_img->text().toStdString() != "")
	{
	//��ʾͼƬ
		QImage *_img = new QImage();
		if (_img->load(ui.lineEdit_img->text()) == true) 
		{
			QGraphicsScene *scene = new QGraphicsScene;
			scene->addPixmap(QPixmap::fromImage(*_img));
			ui.graphicsView->setScene(scene);
			//��������
			QString cmd="";
			cmd.append(CoreDir);
			cmd.append("/classification.exe");cmd.append(" ");
			cmd.append(ui.lineEdit_deploy->text()); cmd.append(" ");
			cmd.append(ui.lineEdit_caffemodel->text()); cmd.append(" ");
			cmd.append(ui.lineEdit_binaryproto->text()); cmd.append(" ");
			cmd.append(ui.lineEdit_labels->text()); cmd.append(" ");
			cmd.append(ui.lineEdit_img->text());
			//���������߳�
			_AdditionalProcess->start(cmd);
		}
		else
		{
			QMessageBox::information(this, QStringLiteral("�������"), QStringLiteral("ͼƬ��ʧ��!"));
			return;
		};
	}
	else
	{
		QMessageBox::information(this, QStringLiteral("�������"), QStringLiteral("��������!"));
		return;
	}
	

}

void Classification::ZoomImage()
{
	qreal scale = qPow(qreal(2), (ui.horizontalSlider->value()) / qreal(25));
	QMatrix matrix;//���ž���
	matrix.scale(scale, scale);
	ui.graphicsView->setMatrix(matrix);
}
