#include "fusionstudio.h"
#include "QDebug"
#include <QFileDialog>
#include <QSplitter>
#include <QDirModel>
#include <QMessageBox>
#include <QInputDialog>
#include <QClipboard>
#include <QFileInfo>
#include <QProcess>

//=====================================================
//=====================�෽��==========================
//=====================================================
//����ͳ�ʼ��
FusionStudio::FusionStudio(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->configureUI();

	//��ʾ��ʼҳ��
	HomePage *_homePage = new HomePage();
	ui.mdiArea->addSubWindow(_homePage);
	_homePage->showMaximized();

	m_project = new Project();//��ǰ���̳�ʼ��
	m_PreferencesManager = new Preferences();//��ѡ�������
	
							  //����̨��׼����ض���
	p2 = new QProcess();
	p2->setProcessChannelMode(QProcess::MergedChannels);
	connect(p2, SIGNAL(readyRead()), this, SLOT(start_read_output()));
	connect(m_project, SIGNAL(ValueChanged()), this, SLOT(Refresh()));
}

//�Զ���UI����
void FusionStudio::configureUI()
{
	QAction* action_New_Working_Dir = new QAction(QStringLiteral("&�½�����Ŀ¼"), ui.treeView);
	connect(action_New_Working_Dir, SIGNAL(triggered()), this, SLOT(FolderCreater()));

	QAction* action_Open_Working_Dir = new QAction(QStringLiteral("&�򿪹���Ŀ¼"), ui.treeView);
	connect(action_Open_Working_Dir, SIGNAL(triggered()), this, SLOT(FolderUnfolder()));

	QAction* action_New_File = new QAction(QStringLiteral("&�½��ļ�"), this);
	connect(action_New_File, SIGNAL(triggered()), this, SLOT(newFile()));

	QAction* action_New_Folder = new QAction(QStringLiteral("&�½��ļ���"), this);
	connect(action_New_Folder, SIGNAL(triggered()), this, SLOT(newFolder()));

	QAction* action_Open_In_Explorer = new QAction(QStringLiteral("&����Դ�������д�"), this);
	connect(action_Open_In_Explorer, SIGNAL(triggered()), this, SLOT(openInSystemResourceManager()));

	QAction* action_Open_Father_Dir = new QAction(QStringLiteral("&������Ŀ¼"), ui.treeView);
	connect(action_Open_Father_Dir, SIGNAL(triggered()), this, SLOT(openInSystemResourceManager()));

	QAction* action_Get_Abs_Path = new QAction(QStringLiteral("&���ƾ���·��"), this);
	connect(action_Get_Abs_Path, SIGNAL(triggered()), this, SLOT(getAbsolutePath()));

	QAction* action_Delete_File = new QAction(QStringLiteral("&ɾ���ļ�"), this);
	connect(action_Delete_File, SIGNAL(triggered()), this, SLOT(deleteFileOrPath()));

	QAction* action_Delete_Path = new QAction(QStringLiteral("&ɾ���ļ���"), this);
	connect(action_Delete_Path, SIGNAL(triggered()), this, SLOT(deleteFileOrPath()));

	//================�Ҽ��˵��Ķ���======================
	//�ļ���Ϊ��ʱ�������Ҽ��¼��˵�
	TreeViewMenu_Empty = new QMenu(ui.treeView);
	TreeViewMenu_Empty->addAction(action_New_Working_Dir);
	TreeViewMenu_Empty->addAction(action_Open_Working_Dir);
	//�������ļ��ڵ��ϵ��Ҽ��¼��˵�
	TreeViewMenu_File = new QMenu(ui.treeView);
	TreeViewMenu_File->addAction(action_Open_Father_Dir);
	TreeViewMenu_File->addAction(action_Get_Abs_Path);
	TreeViewMenu_File->addSeparator();
	TreeViewMenu_File->addAction(action_Delete_File);
	//�������ļ��нڵ������Ҽ��¼��˵�
	TreeViewMenu_Path = new QMenu(ui.treeView);
	//TreeViewMenu_Path->addAction(action_New_File);
	TreeViewMenu_Path->addAction(action_New_Folder);
	TreeViewMenu_Path->addSeparator();
	TreeViewMenu_Path->addAction(action_Open_In_Explorer);
	TreeViewMenu_Path->addAction(action_Get_Abs_Path);
	TreeViewMenu_Path->addSeparator();
	TreeViewMenu_Path->addAction(action_Delete_Path);
	//================����ui����===========================
	ui.action_view_Console->setChecked(true);
	ui.action_view_Explorer->setChecked(true);
}

//=====================================================
//=====================�ۺ���==========================
//=====================================================
//�ļ������Ҽ��˵���Ӧ
void FusionStudio::on_treeView_customContextMenuRequested(QPoint pos)
{
	//qDebug() << "�Ҽ�����"<<pos.x() << "," << pos.y();
	//ͨ�����Է���,�������������ε��õĴ�������һ����
	//��ʱͨ�������δ����Ƿ����һ�η�����ͬһ��λ������ܸ�����

	if (PrePosition != pos) {//��δ�������������
		PrePosition = pos;
		if (ui.treeView->model() == NULL) {
			//��ʱ�ļ����ǿյ�
			TreeViewMenu_Empty->exec(QCursor::pos());//��ʾ�Ҽ��˵�
		}
		else//�ļ����ǿյ�ʱ������������
		{
			QModelIndex indexSelect = ui.treeView->indexAt(pos);//��ǰ�ڵ�����
			QString AbsPath = indexSelect.data().toString();       //��ǰ�ڵ�����
			if (AbsPath != "")//�����ǰ�һ����������νڵ���
			{
				//��������ǰ�ڵ�ľ���·��
				QString temp;
				QModelIndex indexParent;
				do {
					indexParent = indexSelect.parent();//��ǰ�ڵ�ĸ��ڵ�
					temp = indexParent.data().toString();
					if (temp == "")
					{
						break;
					}
					AbsPath = temp.append("\\" + AbsPath);
					indexSelect = indexParent;
				} while (true);
				CurrentTreeViewSelectedPath = AbsPath;//�����·��ͨ��ȫ�ִ���ȥ
				//�ֱ�һ�µ㵽��ʲô
				QFileInfo _fio(AbsPath);
				if (_fio.isFile()==true)//�ļ�
				{
					TreeViewMenu_File->exec(QCursor::pos());
				}
				if (_fio.isDir()==true)//�ļ���
				{
					TreeViewMenu_Path->exec(QCursor::pos());
				}
			}
		}
	}
	else
	{
		//��δ����Ƿ���������
		PrePosition.setX(-1);
		PrePosition.setY(-1);
	}
}

//����̨��׼����ض���
void FusionStudio::start_read_output()
{
	QString result = QString::fromLocal8Bit(p2->readAll());
	ui.plainTextEdit->appendPlainText(result);
	//ui.plainTextEdit->appendPlainText("Ok,i am working...");
}
//ˢ���ļ���
void FusionStudio::Refresh()
{
	//QMessageBox::warning(this, QStringLiteral("����ѧ����ʺ�"), QStringLiteral("�յ���ֵ������Ϣ"));
	if (m_project->getWorkingDir().isEmpty()) {
		//Ŀ¼�ǿյ�,�����������,Ҳ������ִ���˹رչ���Ŀ¼,ͳһ����Ϊ����ļ���
		ui.treeView->setModel(NULL);
	}
	else
	{
		QDirModel *model = new QDirModel();
		ui.treeView->setModel(model);
		ui.treeView->setColumnHidden(1, true);
		ui.treeView->setColumnHidden(2, true);
		ui.treeView->setColumnHidden(3, true);
		ui.treeView->setRootIndex(model->index(m_project->getWorkingDir()));
		ui.treeView->expandAll();//ȫ��չ��
	}
}
//==================�ļ����Ҽ��˵�=====================
//�ļ����Ҽ��˵�->�½��ļ�
void FusionStudio::newFile()
{

}

//�ļ����Ҽ��˵�->�½��ļ���
void FusionStudio::newFolder()
{
	//�����Ի���ѯ�����ļ��е�����
	bool isOK;
	QString FolderName = QInputDialog::getText(NULL, QStringLiteral("�½��ļ���"),
		QStringLiteral("��Ϊ�����½����ļ�������:"),
		QLineEdit::Normal, QStringLiteral("�½��ļ���"), &isOK);
	if (isOK)//���ȷ��
	{
		QString _temppath = CurrentTreeViewSelectedPath;
		_temppath.append("/");
		_temppath.append(FolderName);

		QDir *_tempdir = new QDir;
		if (_tempdir->exists(_temppath))
		{
			QMessageBox::warning(this, QStringLiteral("�½��ļ���"), QStringLiteral("�ļ����Ѿ����ڣ�"));
		}
		else
		{
			if (_tempdir->mkdir(_temppath))
			{
				QMessageBox::warning(this, QStringLiteral("�����ļ���"), QStringLiteral("�ļ��д����ɹ���"));
				Refresh();
			}
			else
			{
				QMessageBox::warning(this, QStringLiteral("�½��ļ���"), QStringLiteral("δ֪�����µĴ���ʧ��!"));
			}
		}
	}
}

//�ļ����Ҽ��˵�->��ȡ����·����������
void FusionStudio::getAbsolutePath()
{
	//QMessageBox::warning(this, QStringLiteral("�ڵ�����"), CurrentTreeViewSelectedPath);
	//ʹ�� QApplication::clipboard() �������ϵͳ�����������������ķ���ֵ�� QClipboard ָ�롣  
	QClipboard *board = QApplication::clipboard();
	//ͨ�� setText()��setImage() ���� setPixmap() �������Խ����ݷ��õ��������ڣ�Ҳ����ͨ����˵�ļ������߸��ƵĲ�����  
	board->setText(CurrentTreeViewSelectedPath);
	//ʹ��text()��image() ���� pixmap() ��������ԴӼ����������ݣ�Ҳ����ճ����  
	//QString str = board->text(); 
}

//�ļ����Ҽ��˵�->����ϵͳ�ļ���Դ�������д�·��
void FusionStudio::openInSystemResourceManager()
{
	QFileInfo _fio(CurrentTreeViewSelectedPath);
	if (_fio.isFile() == true)//�ļ�
	{//�ļ��򿪸�Ŀ¼
		QString cmd = "explorer.exe ";
		cmd.append(_fio.canonicalPath());
		cmd = cmd.replace(QRegExp("\\/"), "\\");//�����滻��"Ʋ"����"��"
		QProcess::execute(cmd);
	}
	if (_fio.isDir() == true)//�ļ���
	{//�ļ���ֱ�Ӵ�
		QString cmd = "explorer ";
		cmd.append(CurrentTreeViewSelectedPath);
		QProcess::execute(cmd);
	}
}
//�ݹ�ɾ���ļ��м�����ȫ���ļ�
bool FusionStudio::DelDir(const QString & path)
{
	if (path.isEmpty()) {
		return false;
	}
	QDir dir(path);
	if (!dir.exists()) {
		return true;
	}
	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //���ù���  
	QFileInfoList fileList = dir.entryInfoList(); // ��ȡ���е��ļ���Ϣ  
	foreach(QFileInfo file, fileList) { //�����ļ���Ϣ  
		if (file.isFile()) { // ���ļ���ɾ��  
			file.dir().remove(file.fileName());
		}
		else { // �ݹ�ɾ��  
			DelDir(file.absoluteFilePath());
		}
	}
	return dir.rmpath(dir.absolutePath()); // ɾ���ļ���  
}
//�ļ����Ҽ��˵�->ɾ��·�����ļ�
void FusionStudio::deleteFileOrPath()
{
	//�ж���·�������ļ�,�Ե�����ͬ��ɾ��ȷ�Ͽ�
	QFileInfo _fio(CurrentTreeViewSelectedPath);
	if (_fio.isFile() == true)//�ļ�
	{//�ļ�ֱ��ɾ��
		QMessageBox::StandardButton rb = QMessageBox::question(NULL, QStringLiteral("ȷ��ɾ��?"), QStringLiteral("��ѡ�ļ�����Ӵ�������ʧ!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (rb == QMessageBox::Yes) 
		{
			_fio.dir().remove(_fio.fileName());
			Refresh();
		}
	}
	if (_fio.isDir() == true)//�ļ���
	{//�ļ��еݹ�ɾ��ȫ������
		QMessageBox::StandardButton rb = QMessageBox::question(NULL, QStringLiteral("ȷ��ɾ��?"), QStringLiteral("ɾ���ļ��н���ɾ���������е��ļ�!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		if (rb == QMessageBox::Yes) 
		{//�Ѿ�ȷ��ɾ��
			DelDir(CurrentTreeViewSelectedPath);
			Refresh();
		}
	}
}

//================�˵�->�ļ�===========================

//�ļ�->����Ŀ¼
void FusionStudio::FolderCreater()
{
	//1.ѡ��Ŀ¼
	QString ParentDir = QFileDialog::getExistingDirectory(this,
		QStringLiteral("ѡ�����ļ��еĸ�Ŀ¼"),
		QDir::currentPath());
	if (ParentDir.isEmpty() == false)//��Ŀ¼�ǿ�
	{
		//2.ѯ��Ŀ¼��
		bool isOK;
		QString FolderName = QInputDialog::getText(NULL, QStringLiteral("�½�����Ŀ¼"),
			QStringLiteral("��Ϊ�����½��Ĺ���Ŀ¼����:"),
			QLineEdit::Normal, "NewFolder", &isOK);
		if (isOK) {//�����ȷ��
			if (FolderName.isEmpty() == false)//Ŀ¼���ǿ�
			{
				//3.����
				QString CompleteDir;
				CompleteDir.append(ParentDir);
				CompleteDir.append("\\");
				CompleteDir.append(FolderName);
				QDir *DirManager = new QDir;
				if (DirManager->exists(CompleteDir) == true)//�Ѿ�����ͬ��Ŀ¼
				{
					QMessageBox::warning(this, QStringLiteral("����ʧ��"), QStringLiteral("�ļ����Ѿ����ڣ�"));
				}
				else
				{
					if (DirManager->mkdir(CompleteDir) == true)//�����ɹ�
					{
						QMessageBox::warning(this, QStringLiteral("�����ļ���"), QStringLiteral("�ļ��д����ɹ���"));
						//4.���ù���Ŀ¼
						//setWorkingDir(CompleteDir);
						m_project->setWorkingDir(CompleteDir);
					}
					else
					{
						QMessageBox::warning(this, QStringLiteral("����ʧ��"), QStringLiteral("δ֪����!"));
					}
				}
			}
		}
	}
	else
	{
		//��Ŀ¼Ϊ��
		QMessageBox::warning(this, QStringLiteral("����ʧ��"), QStringLiteral("��Ŀ¼Ϊ�գ�"));
	}


}

//�ļ�->����Ŀ¼
void FusionStudio::FolderUnfolder()
{
	/*
	��һ���Ѿ����ڵ�Ŀ¼,��Ϊ��ǰ�Ĺ���Ŀ¼
	*/
	//1.���ļ�������
	QString ExistedDir = QFileDialog::getExistingDirectory(this,
		QStringLiteral("ѡ��Ŀ¼��Ϊ����Ŀ¼"),
		QDir::currentPath());
	if (ExistedDir.isEmpty() == false)//����Ŀ¼�ǿ�
	{
		//�����Ŀ¼����Ϊ��ǰ����Ŀ¼,����ֵ�����¼�
		//setWorkingDir(ExistedDir);
		m_project->setWorkingDir(ExistedDir);
	}

}

//�ļ�->�ر�Ŀ¼
void FusionStudio::FolderCloser()
{
	//setWorkingDir(NULL);//����·����Ϊ��
	m_project->setWorkingDir(NULL);
}

//================�˵�->��ͼ===========================

//�����Դ�������ͼ�Ŀɼ���
void FusionStudio::changeVisible_Explorer()
{
	if (ui.dockWidget_Explorer->isHidden() == true) {
		ui.dockWidget_Explorer->show();//����ʵ�ù���
		ui.action_view_Explorer->setChecked(true);
	}
	else
	{
		ui.dockWidget_Explorer->hide();//����ʵ�ù���
		ui.action_view_Explorer->setChecked(false);
	}
}

//�������̨��ͼ�Ŀɼ���
void FusionStudio::changeVisible_Console()
{
	if (ui.dockWidget_Console->isHidden() == true) {
		ui.dockWidget_Console->show();//����ʵ�ù���
		ui.action_view_Console->setChecked(true);
	}
	else
	{
		ui.dockWidget_Console->hide();//����ʵ�ù���
		ui.action_view_Console->setChecked(false);
	}
}

//================�˵�->��Ŀ============================

//��Ŀ->ɨ�蹤��Ŀ¼
void FusionStudio::WorkingDirScanner()
{
	Refresh();
}

//================�˵�->����============================

//�½��༭��
void FusionStudio::ShowEditor()
{
	NetEditor *child = new NetEditor();
	ui.mdiArea->addSubWindow(child);
	child->show();
	//��ʱ��һ��
}

//ѵ��
void FusionStudio::Train()
{
	TrainingSetter *w = new TrainingSetter(m_PreferencesManager->getBinPath(), p2);
	ui.mdiArea->addSubWindow(w);
	w->show();
}

//����
void FusionStudio::DoClassification()
{
	Classification*w = new Classification(m_PreferencesManager->getBinPath(),p2);
	ui.mdiArea->addSubWindow(w);
	w->show();
}

//����->��������
void FusionStudio::DoFeaturesExp()
{
	FeatureExp*w = new FeatureExp(m_PreferencesManager->getBinPath(), p2);
	ui.mdiArea->addSubWindow(w);
	w->show();
}

//================�˵�->����============================

//����->����Mnist����ת������
void FusionStudio::MnistConverter()
{
	ConvertMnist *w = new ConvertMnist();
	ui.mdiArea->addSubWindow(w);
	w->show();
}

//����->����Cifarת������
void FusionStudio::CifarConverter()
{
	ConvertCifar *w = new ConvertCifar();
	ui.mdiArea->addSubWindow(w);
	w->show();
}

//����->����ͼƬת������
void FusionStudio::ImageConverter()
{
	ConvertImage *w = new ConvertImage();
	ui.mdiArea->addSubWindow(w);
	w->show();
}

//����->�������ݼ��������
void FusionStudio::DatabaseBrowser()
{
	BrowseDatabase *w = new BrowseDatabase();
	ui.mdiArea->addSubWindow(w);
	w->show();
}

//================�˵�->����============================

//����->������ѡ�������
void FusionStudio::PreferencesSetter()
{
	m_PreferencesManager->awakenPreferencesManager();//����
	//ui.mdiArea->addSubWindow(m_PreferencesManager);//��Ϊģ̬�Ի���ʹ��
	m_PreferencesManager->show();
}

//================�˵�->����============================

//����->��ʾ��ʼҳ��
void FusionStudio::ShowHome()
{
	HomePage *_homePage = new HomePage();
	ui.mdiArea->addSubWindow(_homePage);
	_homePage->showMaximized();
}
//����->��ʾ����ҳ��
void FusionStudio::ShowAbout()
{
	About *_aboutPage = new About();
	//ui.mdiArea->addSubWindow(_aboutPage);
	_aboutPage->show();
}

