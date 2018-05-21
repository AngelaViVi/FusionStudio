#include "preferences.h"
#include "QSettings.h"
#include "QFile.h"
#include "QMessageBox.h"
#include "QFileDialog"
#include <qstandarditemmodel.h>

//========================================================================
//=============================�ڲ�����===================================
//========================================================================
//���캯��
Preferences::Preferences(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//�����ui��ʼ��
	ScriptsModel = new QStandardItemModel();
	ScriptsModel->setColumnCount(2);
	ScriptsModel->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit("����"));
	ScriptsModel->setHeaderData(1, Qt::Horizontal, QString::fromLocal8Bit("����"));
	ui.tableView->setModel(ScriptsModel);
	ui.tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);//��ͷ��Ϣ��ʾ����
	/*==========Ĭ�ϲ���==========*/
	IniFilePath = "./EngineStart.ini";//Ĭ�������ļ���ַ

	FuncList.append("train");//�ű��Ĺ���,ͬʱҲ�������ű����ݵļ�ֵ
	FuncList.append("test");
	FuncList.append("convert_image");

	Scripts.insert("train", " train --solver=");//��Ӧ��ֵ�Ľű���Ĭ������
	Scripts.insert("test", " test_Script");
	Scripts.insert("convert_image", " convert_image_Script");

	defaultInstallPath = "E:\\VisualStudio\\FusionStudio\\FusionStudio\\Components\\caffe-CPU-py27";//Ĭ��installĿ¼
	defaultBinPath="E:\\VisualStudio\\FusionStudio\\FusionStudio\\Components\\caffe-CPU-py27\\bin";//Ĭ��binĿ¼
	defaultLibPath="E:\\VisualStudio\\FusionStudio\\FusionStudio\\Components\\caffe-CPU-py27\\lib";//Ĭ��LibĿ¼

	//defaultScript_Train=" train --solver=";//Ĭ�Ͻű�
	//��ʼ������
	initialize();
}
//��������
Preferences::~Preferences()
{
}
/*
����:��ʼ��
����:�鿴�����ļ��Ƿ����,���������,���������ļ�,��д��Ĭ������
*/
void Preferences::initialize()
{
	//QFile file(IniFilePath);
	QFile *file = new QFile(IniFilePath);
	if (file->exists()) {
		//QMessageBox::information(this, "����", QStringLiteral("�ļ�����"));
	}
	else {//�����ļ�������
		//Qt��ʹ��QSettings���дini�ļ�  
		//QSettings���캯���ĵ�һ��������ini�ļ���·��,�ڶ���������ʾ���ini�ļ�,��������������ȱʡ  
		QSettings *configIniWrite = new QSettings(IniFilePath, QSettings::IniFormat);
		//��ini�ļ���д������,setValue���������������Ǽ�ֵ��  
		//��ini�ļ���д��Ĭ�����ò���
		configIniWrite->setValue("path/install", defaultInstallPath);
		configIniWrite->setValue("path/bin", defaultBinPath);
		configIniWrite->setValue("path/lib", defaultLibPath);

		//������ű��ļ�ֵ(���Ƕ�Ӧ�ű��ĵĹ���)д��ini�ļ���һ����������
		configIniWrite->beginWriteArray("FuncList");
		for (int i = 0; i < FuncList.size(); i++)
		{
			configIniWrite->setArrayIndex(i);
			configIniWrite->setValue("Func", FuncList[i]);
		}
		configIniWrite->endArray();
		//����ֵ��Ӧ��ֵд��ini�ļ�
		configIniWrite->beginGroup("Scripts");
		for (int i = 0; i < FuncList.size(); i++)
		{
			QMap<QString, QString>::iterator it = Scripts.find(FuncList[i]);
			configIniWrite->setValue(FuncList[i], it.value());
		}
		configIniWrite->endGroup();
		//д����ɺ�ɾ��ָ��  
		delete configIniWrite;
	};
	delete file;
}
//�л������ļ�
void Preferences::iniFileSwitch(QString path)
{
	IniFilePath = path;
	initialize();
}
//������ѡ�������
void Preferences::awakenPreferencesManager()
{
	//��Ŀǰ��״̬��ʾ��������
	//��ʾ·������
	ui.lineEdit_BinPath->setText(readBinPath());
	ui.lineEdit_InstallPath->setText(readInstallPath());
	ui.lineEdit_LibPath->setText(readLibPath());
	//��ʾ��������(���)
		///���Ȳ�ѯ�����е�key,����һ����ʱ����(����)����,
	QVector <QString> _tempFuncList;
	QSettings *mod_read = new QSettings(IniFilePath, QSettings::IniFormat, 0);
	int num = mod_read->beginReadArray("FuncList");
	for (int i = 0; i<num; i++)
	{
		mod_read->setArrayIndex(i);
		_tempFuncList.append( mod_read->value("Func", "������Ϊ��").toString());
	}
	mod_read->endArray();
		///Ȼ�����ѭ��,��key���ļ��в�ѯ������,�ٰ�key������д��ͬһ����,Ȼ��ѭ����������
	mod_read->beginGroup("Scripts");
	for (int i = 0; i < num; i++)
	{
	
		ScriptsModel->setItem(i, 0, new QStandardItem(_tempFuncList[i]));//�����ֶ�
		ScriptsModel->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));//�����ַ���ɫ
		ScriptsModel->item(i, 0)->setTextAlignment(Qt::AlignCenter);//�����ַ�λ��
		ScriptsModel->setItem(i, 1, new QStandardItem(mod_read->value(_tempFuncList[i]).toString()));//�����ֶ�
	}
	mod_read->endGroup();
	delete mod_read;
}
//���ļ��ж�ȡinstall·��
QString Preferences::readInstallPath()
{
	QSettings *configIniRead = new QSettings(IniFilePath, QSettings::IniFormat);
	//����ȡ����ini�ļ�������QString�У���ȡֵ��Ȼ��ͨ��toString()����ת����QString����   
	QString Result = configIniRead->value("path/install").toString();
	
	//��������ɺ�ɾ��ָ��   
	delete configIniRead;
	return Result;
}
//���ļ��ж�ȡbin��·��
QString Preferences::readBinPath()
{
	QSettings *configIniRead = new QSettings(IniFilePath, QSettings::IniFormat);  
	QString Result = configIniRead->value("path/bin").toString();
	delete configIniRead;
	return Result;
}
//���ļ��ж�ȡlib��·��
QString Preferences::readLibPath()
{
	QSettings *configIniRead = new QSettings(IniFilePath, QSettings::IniFormat);
	QString Result = configIniRead->value("path/lib").toString();
	delete configIniRead;
	return Result;
}
//���ļ��ж�ȡ��ӦFunc�Ľű�
QString Preferences::readScript(QString Func)
{
	QSettings *configIniRead = new QSettings(IniFilePath, QSettings::IniFormat);
	configIniRead->beginGroup("Scripts");
	QString Result = configIniRead->value(Func,"").toString();
	configIniRead->endGroup();
	delete configIniRead;
	return Result;
}
//��install·��д���ļ�
void Preferences::writeInstallPath(QString path)
{
	QSettings *configIniWrite = new QSettings(IniFilePath, QSettings::IniFormat);
	configIniWrite->setValue("path/install", path);
	delete configIniWrite;
}
//��bin·��д���ļ�
void Preferences::writeBinPath(QString path)
{
	QSettings *configIniWrite = new QSettings(IniFilePath, QSettings::IniFormat);
	configIniWrite->setValue("path/bin", path);
	delete configIniWrite;
}
//��lib·��д���ļ�
void Preferences::writeLibPath(QString path)
{
	QSettings *configIniWrite = new QSettings(IniFilePath, QSettings::IniFormat);
	configIniWrite->setValue("path/lib", path);
	delete configIniWrite;
}
//����ӦFunc�Ľű�д���ļ�
void Preferences::writeScript(QString Func, QString script)
{
	QSettings *configIniWrite = new QSettings(IniFilePath, QSettings::IniFormat);
	configIniWrite->beginGroup("Scripts");
	configIniWrite->setValue(Func, script);
	configIniWrite->endGroup();
	delete configIniWrite;
}

//========================================================================
//=============================�ⲿ�ӿ�===================================
//========================================================================

//��ȡinstallĿ¼
QString Preferences::getInstallPath()
{
	return readInstallPath();
}
//��ȡbinĿ¼
QString Preferences::getBinPath()
{
	return readBinPath();
}
//��ȡlibĿ¼
QString Preferences::getLibPath()
{
	return readLibPath();
}
//��ȡָ���ű�
QString Preferences::getScript(QString Func)
{
	return readScript(Func);
}

//=========================================================================
//=============================�ۺ���======================================
//=========================================================================

//���1
void Preferences::Browser1()
{
	QString tempInstallPath = QFileDialog::getExistingDirectory(this,tr("ѡ��installĿ¼"), QDir::currentPath());
	if (tempInstallPath!="")
	{
		ui.lineEdit_InstallPath->setText(tempInstallPath);
	}
}
//���2
void Preferences::Browser2()
{
	QString tempBinPath = QFileDialog::getExistingDirectory(this,tr("ѡ��binĿ¼"), QDir::currentPath());
	if (tempBinPath!="")
	{
		ui.lineEdit_BinPath->setText(tempBinPath);
	}
}
//���3
void Preferences::Browser3()
{
	QString tempLibPath = QFileDialog::getExistingDirectory(this, tr("ѡ��libĿ¼"), QDir::currentPath());
	if (tempLibPath != "") {
		ui.lineEdit_LibPath->setText(tempLibPath);
	}
}
//������״̬
void Preferences::checkComponent()
{
	//�������
	QString CheckList[]{ "caffe.exe","classification.exe","compute_image_mean.exe",
		"convert_cifar_data.exe","convert_imageset.exe","convert_mnist_data.exe",
		"convert_mnist_siamese_data.exe","device_query.exe","extract_features.exe",
		"finetune_net.exe","net_speed_benchmark.exe","test_net.exe",
		"train_net.exe","upgrade_net_proto_binary.exe","upgrade_net_proto_text.exe",
		"upgrade_solver_proto_text.exe" };
	ui.plainTextEdit_ComponentState->clear();
	QString tempBinPath = readBinPath();
	if (tempBinPath!="") 
	{
		ui.plainTextEdit_ComponentState->appendPlainText(QStringLiteral("����У��������:")+ tempBinPath);
		tempBinPath = tempBinPath.append("\\");
		for (int i = 0; i < 16; i++)
		{
			QString _currentFile = tempBinPath;
			_currentFile.append(CheckList[i]);
			QFile file(_currentFile);
			if (file.exists()) {
				ui.plainTextEdit_ComponentState->appendPlainText(CheckList[i] + "...OK!");
			}
			else
			{
				ui.plainTextEdit_ComponentState->appendPlainText(CheckList[i] + "...ERROR!");
			}
		}
	}
	else
	{
		ui.plainTextEdit_ComponentState->appendPlainText(QStringLiteral("binĿ¼Ϊ��!"));
	}

}
//ȫ��·�����ó�Ĭ�ϲ���
void Preferences::setDefaultPath()
{
	
	ui.lineEdit_BinPath->setText(defaultBinPath);
	ui.lineEdit_InstallPath->setText(defaultInstallPath);
	ui.lineEdit_LibPath->setText(defaultLibPath);
}

void Preferences::creatEnvironmentVariable()
{
	//���뻷������
}
//ȫ���ű���ΪĬ��
void Preferences::setDefaultScripts()
{
	for (int i = 0; i < FuncList.size(); i++)
	{
		QMap<QString, QString>::iterator it = Scripts.find(FuncList[i]);
		ScriptsModel->setItem(i, 0, new QStandardItem(FuncList[i]));//�����ֶ�
		ScriptsModel->item(i, 0)->setForeground(QBrush(QColor(255, 0, 0)));//�����ַ���ɫ
		ScriptsModel->item(i, 0)->setTextAlignment(Qt::AlignCenter);//�����ַ�λ��
		ScriptsModel->setItem(i, 1, new QStandardItem(it.value()));//�����ֶ�

	}
}
//����
void Preferences::save()
{
	//����·������
	writeBinPath(ui.lineEdit_BinPath->text());
	writeInstallPath(ui.lineEdit_InstallPath->text());
	writeLibPath(ui.lineEdit_LibPath->text());
	//����ű�����
	///��ȡ�������,�ǿյĽ��и���
	ScriptsModel->rowCount();//һ��������
	QString _tempFunc = ScriptsModel->data(ScriptsModel->index(3, 1)).toString();//��3�е�1�е�����
	QString _tempScript;
	for (int i = 0; i < ScriptsModel->rowCount(); i++)
	{
		_tempFunc = ScriptsModel->data(ScriptsModel->index(i, 0)).toString();//��i�е�1�е�����
		_tempScript= ScriptsModel->data(ScriptsModel->index(i, 1)).toString();//��i�е�2�е�����
		if (_tempFunc!=""&&_tempScript!="")
		{
			writeScript(_tempFunc, _tempScript);
		}
	}
	//�����������

}

