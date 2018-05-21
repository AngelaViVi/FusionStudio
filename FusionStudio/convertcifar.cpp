#include "convertcifar.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

ConvertCifar::ConvertCifar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	MyConverter = new DataConverter();
	ui.LmDbButton->setChecked(true);
}

ConvertCifar::~ConvertCifar()
{
}

void ConvertCifar::Browser1()
{
	ui.lineEdit_Inputfolder->setText(
		QFileDialog::getExistingDirectory(this,
			QStringLiteral("ѡ��Cifar���ݼ�Ŀ¼"), QDir::currentPath()));
}
void ConvertCifar::Browser2()
{
	ui.lineEdit_outputfolder->setText(
		QFileDialog::getExistingDirectory(this,
			QStringLiteral("ѡ�����Ŀ¼"), QDir::currentPath()));
}

void ConvertCifar::DoConvert()
{
	std::string _temp = (ui.LmDbButton->isChecked()?"lmdb":"leveldb");//lmdb
	
	bool flag = MyConverter->CifarToDb(ui.lineEdit_Inputfolder->text().toStdString(),
						               ui.lineEdit_outputfolder->text().toStdString(), _temp);
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
