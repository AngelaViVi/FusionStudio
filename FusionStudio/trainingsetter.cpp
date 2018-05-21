#include "trainingsetter.h"
//#include "caffe/proto/caffe.pb.h"//�޷���������ļ�,��Ҫ�����м��
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

/*==========================================================================*/
/*                            �������������                                */
/*==========================================================================*/
TrainingSetter::TrainingSetter(QString coredir, QProcess *addprocess, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	CoreDir = coredir;
	_AdditionalProcess = addprocess;
}

TrainingSetter::~TrainingSetter()
{
}
/*==========================================================================*/
/*                                 �ۺ���                                   */
/*==========================================================================*/
//ѡ��������ļ�
void TrainingSetter::BrowseSolver()
{
	/*
	����1��������
	����2���Ի���ı���
	����3��Ĭ�ϵĴ򿪵�λ�ã��硱�ҵ��ĵ�����
	����4���ļ��Ĺ�������ע���ļ�����֮����  ����  �ֿ�
	*/
	ui.lineEdit_solverFilePath->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("�����"),
			" ",
			QStringLiteral("����������ļ�(*.prototxt);;��������(*.*)")));
}
//ѡ��������յ����λ��
void TrainingSetter::BrowseSnapPath()
{
	ui.lineEdit_snapshot_prefix->setText(
		QFileDialog::getExistingDirectory(this,
			QStringLiteral("ѡ����յ����Ŀ¼"), QDir::currentPath()));
}
//ѡ������ṹ�����ļ�
void TrainingSetter::BrowseNet()
{
	ui.lineEdit_net->setText(
		QFileDialog::getOpenFileName(this,
			QStringLiteral("����ṹ�����ļ�"),
			" ",
			QStringLiteral("����ṹ�����ļ�(*.prototxt);;��������(*.*)")));
}
//���������ز���������������ļ���
void TrainingSetter::SaveSolver()
{
	QFile file(ui.lineEdit_solverFilePath->text());
	if (file.open(QFile::WriteOnly | QFile::Text)) {
		QTextStream out(&file);
		QString _target, target;
		if (ui.lineEdit_net->text()!="")
		{
			out << "net: " << "\"" << ui.lineEdit_net->text() << "\""<<"\n";
		}
		if (ui.lineEdit_test_iter->text()!="")
		{
			out << "test_iter: " << ui.lineEdit_test_iter->text() << "\n";
		}
		if (ui.lineEdit_test_interval->text() != "") {
			out << "test_interval: " << ui.lineEdit_test_interval->text() << "\n";
		}
		if (ui.lineEdit_base_lr->text() != "") {
			out << "base_lr: " << ui.lineEdit_base_lr->text() << "\n";
		}
		if (ui.lineEdit_momentum->text() != "") {
			out << "momentum: " << ui.lineEdit_momentum->text() << "\n";
		}
		if (ui.lineEdit_weight_decay->text() != "") {
			out << "weight_decay: " << ui.lineEdit_weight_decay->text() << "\n";
		}
		
		QString _temp;//lr_policy
		switch (ui.comboBox_lr_policy->currentIndex())
		{
		case 0:
			_temp = "fixed";
			break;
		case 1:
			_temp = "step";
			break;
		case 2:
			_temp = "exp";
			break;
		case 3:
			_temp = "inv";
			break;
		case 4:
			_temp = "multistep";
			break;
		case 5:
			_temp = "poly";
			break;
		case 6:
			_temp = "sigmoid";
			break;
		default:
			break;
		}
		out << "lr_policy: " << "\""<<_temp << "\""<<"\n";
				
		if (ui.lineEdit_gamma->text() != "") {
			out << "gamma: " << ui.lineEdit_gamma->text() << "\n";
		}
		if (ui.lineEdit_power->text() != "") {
			out << "power: " << ui.lineEdit_power->text() << "\n";
		}
		if (ui.lineEdit_display->text() != "") {
			out << "display: " << ui.lineEdit_display->text() << "\n";
		}
		if (ui.lineEdit_max_iter->text() != "") {
			out << "max_iter: " << ui.lineEdit_max_iter->text() << "\n";
		}
		if (ui.lineEdit_snapshot->text() != "") {
			out << "snapshot: " << ui.lineEdit_snapshot->text() << "\n";
		}
		if (ui.lineEdit_snapshot_prefix->text() != "") {
			out << "snapshot_prefix: " << "\"" << ui.lineEdit_snapshot_prefix->text() << "\"" << "\n";
		}
		if (ui.comboBox_solver_mode->currentIndex()==0)
		{
			out << "solver_mode: " << "CPU" << "\n";
		}
		else
		{
			out << "solver_mode: " << "GPU" << "\n";
		}
	}//end of if (file.open)
	else 
	{
		//�ļ���ʧ��
		QMessageBox::information(this, QStringLiteral("ѵ��"), QStringLiteral("�ļ���ʧ��!"));
	}
	
}
//��������ļ��ж�ȡ����д��������Ӧ�ؼ���
void TrainingSetter::ReadSolver()
{
	QFile file(ui.lineEdit_solverFilePath->text());
	if (file.open(QFile::ReadOnly | QFile::Text)) {
		QTextStream QTS(&file);
		while (QTS.atEnd() == false)
		{
			QString _temp = QTS.readLine();
			if (!_temp.startsWith("#"))//ע�Ͳ�����
			{
				int sat = _temp.indexOf(" ");
				QString target = _temp.mid(sat + 1, _temp.length() - (1 + sat));
				if (_temp.startsWith("net"))
				{
					//��������
					QString _target = target.mid(1, target.length() - 2);
					ui.lineEdit_net->setText(_target);
				}
				if (_temp.startsWith("test_iter"))
				{
					ui.lineEdit_test_iter->setText(target);
				}
				if (_temp.startsWith("test_interval"))
				{
					ui.lineEdit_test_interval->setText(target);
				}
				if (_temp.startsWith("base_lr"))
				{
					ui.lineEdit_base_lr->setText(target);
				}
				if (_temp.startsWith("momentum"))
				{
					ui.lineEdit_momentum->setText(target);
				}
				if (_temp.startsWith("weight_decay"))
				{
					ui.lineEdit_weight_decay->setText(target);
				}
				if (_temp.startsWith("lr_policy:"))//lr_policy
				{
					QString _target = target.mid(1, target.length() - 2);//���������
					if (_target == "fixed")
					{
						ui.comboBox_lr_policy->setCurrentIndex(0);
					}
					if (_target == "step")
					{
						ui.comboBox_lr_policy->setCurrentIndex(1);
					}
					if (_target == "exp")
					{
						ui.comboBox_lr_policy->setCurrentIndex(2);
					}
					if (_target == "inv")
					{
						ui.comboBox_lr_policy->setCurrentIndex(3);
					}
					if (_target == "multistep")
					{
						ui.comboBox_lr_policy->setCurrentIndex(4);
					}
					if (_target == "poly")
					{
						ui.comboBox_lr_policy->setCurrentIndex(5);
					}
					if (_target == "sigmoid")
					{
						ui.comboBox_lr_policy->setCurrentIndex(6);
					}
				}

				if (_temp.startsWith("gamma"))
				{
					ui.lineEdit_gamma->setText(target);
				}
				if (_temp.startsWith("power"))
				{
					ui.lineEdit_power->setText(target);
				}
				if (_temp.startsWith("display"))
				{
					ui.lineEdit_display->setText(target);
				}
				if (_temp.startsWith("max_iter"))
				{
					ui.lineEdit_max_iter->setText(target);
				}
				if (_temp.startsWith("snapshot:"))
				{
					ui.lineEdit_snapshot->setText(target);
				}
				if (_temp.startsWith("snapshot_prefix"))
				{
					//��������
					QString _target = target.mid(1, target.length() - 2);
					ui.lineEdit_snapshot_prefix->setText(_target);
				}
				if (_temp.startsWith("solver_mode"))//CPU&GPU
				{
					if (target == "GPU")
					{
						ui.comboBox_solver_mode->setCurrentIndex(1);
					}
					else {
						ui.comboBox_solver_mode->setCurrentIndex(0);
					}
				}
			}
		}//end of while (QTS.atEnd() == false)
	}//end of if (file.open)
	else
	{
		//�ļ���ʧ��
		QMessageBox::information(this, QStringLiteral("ѵ��"), QStringLiteral("�ļ���ʧ��!"));
	}
}
//ִ��ѵ��
void TrainingSetter::ExecuteTraining()
{
	//У�����
	if (ui.lineEdit_solverFilePath->text() != "") {
		//��������
		QString cmd = "";
		cmd.append(CoreDir);
		cmd.append("/caffe.exe"); cmd.append(" ");
		cmd.append("train"); cmd.append(" ");
		cmd.append("--solver"); cmd.append("=");
		cmd.append(ui.lineEdit_solverFilePath->text());
		//���������߳�
		_AdditionalProcess->start(cmd);
	}
	else
	{
		QMessageBox::information(this, QStringLiteral("ѵ��"), QStringLiteral("��������!"));
		return;
	}
}
//ѧϰ�ʲ��Ա��
void TrainingSetter::policyChange()
{
	switch (ui.comboBox_lr_policy->currentIndex())
	{
	case 0://fix
		ui.lineEdit_gamma->setEnabled(false);
		ui.lineEdit_power->setEnabled(false);
		ui.lineEdit_stepsize->setEnabled(false);
		ui.lineEdit_stepvalue->setEnabled(false);
		break;
	case 1://step(base_lr * gamma ^ (floor(iter / stepsize)))
		ui.lineEdit_gamma->setEnabled(true);
		ui.lineEdit_power->setEnabled(false);
		ui.lineEdit_stepsize->setEnabled(true);
		ui.lineEdit_stepvalue->setEnabled(false);
		break;
	case 2://exp(base_lr * gamma ^ iter)
		ui.lineEdit_gamma->setEnabled(true);
		ui.lineEdit_power->setEnabled(false);
		ui.lineEdit_stepsize->setEnabled(false);
		ui.lineEdit_stepvalue->setEnabled(false);
		break;
	case 3://inv(base_lr * (1 + gamma * iter) ^ (- power))
		ui.lineEdit_gamma->setEnabled(true);
		ui.lineEdit_power->setEnabled(true);
		ui.lineEdit_stepsize->setEnabled(false);
		ui.lineEdit_stepvalue->setEnabled(false);
		break;
	case 4://multistep(����stepvalue���µ�step����)
		ui.lineEdit_gamma->setEnabled(true);
		ui.lineEdit_power->setEnabled(false);
		ui.lineEdit_stepsize->setEnabled(true);
		ui.lineEdit_stepvalue->setEnabled(true);
		break;
	case 5://poly(base_lr (1 - iter/max_iter) ^ (power))
		ui.lineEdit_gamma->setEnabled(false);
		ui.lineEdit_power->setEnabled(true);
		ui.lineEdit_stepsize->setEnabled(false);
		ui.lineEdit_stepvalue->setEnabled(false);
		break;
	case 6://sigmoid(base_lr ( 1/(1 + exp(-gamma * (iter - stepsize)))))
		ui.lineEdit_gamma->setEnabled(true);
		ui.lineEdit_power->setEnabled(false);
		ui.lineEdit_stepsize->setEnabled(true);
		ui.lineEdit_stepvalue->setEnabled(false);
		break;
	default:
		break;
	}
}
