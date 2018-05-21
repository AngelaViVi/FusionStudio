#include "featureexp.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

FeatureExp::FeatureExp(QString coredir, QProcess *addprocess, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.radioButton_lmdb->setChecked(true);
	CoreDir = coredir;
	_AdditionalProcess = addprocess;
}

FeatureExp::~FeatureExp()
{
}
//ѡ��ѵ���õ�����ģ��
void FeatureExp::Browser1()
{
	ui.lineEdit_Model->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("ѡ��ѵ���õ�����ģ��"),
			" ",
			QStringLiteral("caffe����ģ��(*.caffemodel);;��������(*.*)")));
}
//ѡ�����粿�������ļ�
void FeatureExp::Browser2()
{
	ui.lineEdit_Prototxt->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("ѡ�����粿�������ļ�"),
			" ",
			QStringLiteral("���������ļ�(*.prototxt);;��������(*.*)")));
}
//ѡ�񵼳�λ��
void FeatureExp::Browser3()
{
	ui.lineEdit_ExpPath->setText(
		QFileDialog::getExistingDirectory(this,
			QStringLiteral("ѡ�����Ŀ¼"), QDir::currentPath()));
}
//��֤?��֤ʲô��???ֱ�������˷����Ѿ�ûʱ����
void FeatureExp::Verification()
{
}
//ִ����������
void FeatureExp::DoFeatureExp()
{
	QString tempQS = ui.lineEdit_ExpPath->text();
	tempQS.append("/");
	tempQS.append(ui.lineEdit_DbName->text());

	std::string _temp = "lmdb";

	//ֱ�ӵ���ҵ���߼�
	//У�����
	if (ui.lineEdit_Model->text() != ""&&
		ui.lineEdit_Prototxt->text() != ""&&
		ui.lineEdit_LayerID->text() != ""&&
		ui.lineEdit_MiniBatches->text() != ""&&
		tempQS!="")
	{
		//��������
		QString cmd = "";
		cmd.append(CoreDir);
		cmd.append("/extract_features.exe"); cmd.append(" ");
		cmd.append(ui.lineEdit_Model->text()); cmd.append(" ");
		cmd.append(ui.lineEdit_Prototxt->text()); cmd.append(" ");
		cmd.append(ui.lineEdit_LayerID->text()); cmd.append(" ");
		cmd.append(tempQS); cmd.append(" ");
		cmd.append(ui.lineEdit_MiniBatches->text()); cmd.append(" ");
		cmd.append((ui.radioButton_lmdb->isChecked() ? "lmdb" : "leveldb")); cmd.append(" ");
		cmd.append((ui.comboBox_cupgpu->currentIndex()==0 ? "CPU" : "GPU"));
		//���������߳�
		_AdditionalProcess->start(cmd);
	}
	else
	{
		QMessageBox::information(this, QStringLiteral("��������"), QStringLiteral("��������!"));
		return;
	}

}
