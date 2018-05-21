#include "browsedatabase.h"
#include <QFileDialog>
#include <QtCore/qmath.h>
#include <qDebug>
#include <QMessageBox>

BrowseDatabase::BrowseDatabase(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.radioButton_lmdb->setChecked(true);
}

BrowseDatabase::~BrowseDatabase()
{
}
//Matת��ΪQImage
QImage BrowseDatabase::MatToQImage(const cv::Mat &mat)
{
	// 8-bits unsigned, NO. OF CHANNELS=1
	if (mat.type() == CV_8UC1)
	{
		// Set the color table (used to translate colour indexes to qRgb values)
		QVector<QRgb> colorTable;
		for (int i = 0; i<256; i++)
			colorTable.push_back(qRgb(i, i, i));
		// Copy input Mat
		const uchar *qImageBuffer = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
		img.setColorTable(colorTable);
		return img;
	}
	// 8-bits unsigned, NO. OF CHANNELS=3
	if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *qImageBuffer = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return img.rgbSwapped();
	}
	else
	{
		//qDebug() << "ERROR: Mat could not be converted to QImage.";
		return QImage();
	}
}
//�������ݿ��ļ�
void BrowseDatabase::LoadDatabase() {
	//1.��·��ѡ�񴰿�
	QString tempInstallPath = QFileDialog::getExistingDirectory(this, QStringLiteral("ѡ�����ݿ��ļ�·��"), QDir::currentPath());
	if (tempInstallPath != "")
	{
		//2.�����ݿ�������
		std::string datatype = (ui.radioButton_lmdb->isChecked() ? "lmdb" : "leveldb");
		
		if (ui.checkBox_ForcedDecoding->isChecked()==true)//ʹ��ǿ�ƽ���
		{
			//��goto�����ж�
			goto VerifyCompleted;
		}
		//2.1��֤���ݿ����Ч��
		if (
			QFile::exists(tempInstallPath) && 
			 (
				(	datatype == "lmdb"&&
					QFile::exists(tempInstallPath + "/data.mdb") &&
					QFile::exists(tempInstallPath + "/lock.mdb")
				) || (
					datatype == "leveldb"&&
					QFile::exists(tempInstallPath + "/CURRENT") &&
					QFile::exists(tempInstallPath + "/LOCK") &&
					QFile::exists(tempInstallPath + "/LOG")
				)
			 )
			)
		{
VerifyCompleted:
			//ͨ����֤
			MyDbHandle = new DbOperator(datatype, tempInstallPath.toStdString());
			//3.��ʾ��һ��ͼƬ
			cv::Mat imageToShow;
			int label = 0;
			MyDbHandle->getImage(imageToShow, label);
			QImage qimageToShow = MatToQImage(imageToShow);//�õ���QImage
			QGraphicsScene *scene = new QGraphicsScene;
			scene->addPixmap(QPixmap::fromImage(qimageToShow));
			ui.graphicsView->setScene(scene);
			//ui.graphicsView->resize(qimageToShow.width() + 10, qimageToShow.height() + 10);
			ui.graphicsView->show();
			ui.lineEdit->setText(QString::number(label, 10));
		}
		else
		{
			QMessageBox::information(this, QStringLiteral("���ݼ����"), 
				QStringLiteral("ѡ�е��ļ�Ŀ¼����ѡ�����ݿ����Ͳ����������ݿ��Ѿ���!"));
			return;
		}
	}
	else
	{
		QMessageBox::information(this, QStringLiteral("���ݼ����"),
			QStringLiteral("��ѡ��һ�����ݿ�Ŀ¼!"));
		return;
	}
}
//��һ��ͼƬ
void BrowseDatabase::Nextimage()
{
	cv::Mat imageToShow;
	int label = 0;
	bool flag = MyDbHandle->getImage(imageToShow, label);
	if (flag==true)
	{
		QImage qimageToShow = MatToQImage(imageToShow);//�õ���QImage
		QGraphicsScene *scene = new QGraphicsScene;
		scene->addPixmap(QPixmap::fromImage(qimageToShow));
		ui.graphicsView->setScene(scene);
		//ui.graphicsView->resize(qimageToShow.width() + 10, qimageToShow.height() + 10);
		ui.graphicsView->show();
		ui.lineEdit->setText(QString::number(label, 10));
	}
	else
	{
		qDebug()<< QStringLiteral("��ͼ")<<"\n";
	}
	
}
//����
void BrowseDatabase::ZoomImage()
{
	qreal scale = qPow(qreal(2), (ui.zoomSlider->value()) / qreal(25));
	QMatrix matrix;//���ž���
	matrix.scale(scale, scale);
	ui.graphicsView->setMatrix(matrix);
}

void BrowseDatabase::CrashWarning()
{
	if (ui.checkBox_ForcedDecoding->isChecked() == true) {
		QMessageBox::information(this, QStringLiteral("��������"),
		QStringLiteral("��ѡ������γ���Ĺ�����Ϲ��ܲ�����ǿ�ƽ���\n���ܻ�������ش����������³������!\n�����ʹ��!"));
	}
	
}
