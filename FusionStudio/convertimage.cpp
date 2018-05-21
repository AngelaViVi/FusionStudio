#include "convertimage.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

ConvertImage::ConvertImage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	MyConverter = new DataConverter();
	ui.radioButton_Lmdb->setChecked(true);
}

ConvertImage::~ConvertImage()
{
}
//ѡ���ļ��б��ļ�
void ConvertImage::Browser1()
{
	ui.lineEdit_ImgFileList->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("ѡ��ͼƬ�ļ��б�"),
			" ",
			QStringLiteral("�ļ��б�(*.txt);;��������(*.*)")));
}
//ѡ���Ŀ¼
void ConvertImage::Browser2()
{
	ui.lineEdit_RootDir->setText(
		QFileDialog::getExistingDirectory(this,
			QStringLiteral("ѡ��ͼƬ�ļ���Ŀ¼"), QDir::currentPath()));
}
//ѡ�����Ŀ¼
void ConvertImage::Browser3()
{
	ui.lineEdit_OutputDir->setText(
		QFileDialog::getExistingDirectory(this,
			QStringLiteral("ѡ�����Ŀ¼"), QDir::currentPath()));
}
//��һЩ������ΪĬ�ϲ���
void ConvertImage::SetDefault()
{
}
//��������ת��
void ConvertImage::DoConvert()
{

	QString absDbName = ui.lineEdit_OutputDir->text();
	absDbName.append("\\");
	absDbName.append(ui.lineEdit_DBName->text());

	std::string _temp = (ui.radioButton_Lmdb->isChecked() ? "lmdb" : "leveldb");
	bool flag = MyConverter->ImagesToDb(ui.lineEdit_ImgFileList->text().toStdString(),
		ui.lineEdit_RootDir->text().toStdString(),
		absDbName.toStdString(),
		ui.checkBox_isGray->isChecked(),
		ui.checkBox_isShuffle->isChecked(),
		_temp,
		ui.lineEdit_resize_width->text().toInt(),
		ui.lineEdit_resize_height->text().toInt(),
		ui.checkBox_isCheck_size->isChecked(),
		ui.checkBox_isEncoded->isChecked(),
		ui.lineEdit_encode_type->text().toStdString()
	);
	if (flag == false)
	{
		QMessageBox::information(this, QStringLiteral("����ת��"), QStringLiteral("ת��ʧ��!"));
	}
	else {
		//����ת���߳�
		MyConverter->start();
		int cnt = 0; // ѭ������������������"����"��ǰ�Ľ���ֵ
		while (MyConverter->isRunning())  // ֻҪ���̻߳�û����ɣ���һֱѭ���������½�����
		{
			ui.progressBar->setValue((cnt++ % 20) * 5); // ����ֵÿ�ε���5%���ﵽ100�����ٴδ�0��ʼ

			QEventLoop eventloop; // ʹ���¼�ѭ���������߳�
			QTimer::singleShot(500, &eventloop, SLOT(quit())); // wait 0.5s
			eventloop.exec(); // ÿ0.5��ִ��һ���¼�ѭ����Ȼ����½�����
		}
		ui.progressBar->setValue(100);
		QMessageBox::information(this, QStringLiteral("����ת��"), QStringLiteral("ת�����!"));
	}
}
