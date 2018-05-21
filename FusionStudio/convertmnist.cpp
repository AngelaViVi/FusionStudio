#include "convertmnist.h"
#include <string>
#include <QMessageBox>
#include <QTimer>

ConvertMnist::ConvertMnist(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	MyConverter = new DataConverter();
	ui.radioButton_Lmdb->setChecked(true);
}

ConvertMnist::~ConvertMnist()
{
}
//�����洫��Bin·��
void ConvertMnist::TellMeWhereIsTheBin(QString BinPath)
{
}

//ѡ��ͼƬ��
void ConvertMnist::Browser1() {
	/*
	����1��������
	����2���Ի���ı���
	����3��Ĭ�ϵĴ򿪵�λ�ã��硱�ҵ��ĵ�����
	����4���ļ��Ĺ�������ע���ļ�����֮����  ����  �ֿ�
	*/
	ui.lineEdit_MnistImageSet->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("ѡ��ͼƬ��"),
			" ",
			QStringLiteral("MnistͼƬ��(*.idx3-ubyte);;��������(*.*)")));
}
//ѡ���ǩ��
void ConvertMnist::Browser2() {
	/*
	����1��������
	����2���Ի���ı���
	����3��Ĭ�ϵĴ򿪵�λ�ã��硱�ҵ��ĵ�����
	����4���ļ��Ĺ�������ע���ļ�����֮����  ����  �ֿ�
	*/
	ui.lineEdit_MnistlabelSet->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("ѡ���ǩ��"),
			" ",
			QStringLiteral("Mnist��ǩ��(*.idx1-ubyte);;��������(*.*)")));
}
//ѡ������ŵ�ַ
void ConvertMnist::Browser3() {
	/*
	����1��������
	����2���Ի���ı���
	����3��Ĭ�ϵĴ򿪵�λ�ã��硱�ҵ��ĵ�����
	����4���ļ��Ĺ�������ע���ļ�����֮����  ����  �ֿ�
	*/
	ui.lineEdit_MnistResult->setText(
		QFileDialog::getExistingDirectory(this,
			QStringLiteral("ѡ�����Ŀ¼"), QDir::currentPath()));

}
//ת��Mnist����
void ConvertMnist::DoConvert_Mnist_Data()
{

	QString tempQS = ui.lineEdit_MnistResult->text();
	tempQS.append("\\");
	tempQS.append(ui.lineEdit_ResultName->text());


	std::string _temp = (ui.radioButton_Lmdb->isChecked() ? "lmdb" : "leveldb");
	bool flag=MyConverter->MnistToDb(ui.lineEdit_MnistImageSet->text().toStdString(),
									 ui.lineEdit_MnistlabelSet->text().toStdString(),
									 tempQS.toStdString(), _temp);
	if (flag==false)
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