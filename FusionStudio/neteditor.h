#pragma once

#include <QMainWindow>
#include <QAction>
#include <QToolBox>

#include "ui_neteditor.h"

#include "editor.h"
#include "DiagramScene.h"
class NetEditor : public QMainWindow
{
	Q_OBJECT

public:
	///�ⲿ����
	NetEditor(QWidget *parent = Q_NULLPTR);
	NetEditor(QString FileName,QWidget *parent = Q_NULLPTR);
	~NetEditor();

private:
	///�ڲ�����
	Ui::NetEditor ui;
	Editor *MyTextEditor;//�ı��༭��

	DiagramScene *scene;//��ͼ����
	QButtonGroup *buttonGroup;
	QButtonGroup *backgroundButtonGroup;
	QGraphicsView *view;
	QButtonGroup *pointerTypeGroup;
	QComboBox *sceneScaleCombo;
	QToolBar *pointerToolbar;
	QAction*deleteAction;//ɾ����ǰͼԪ
	QAction*toFrontAction;
	QAction*sendBackAction;
	QToolBox *toolBox;//������
private:
	///�ڲ�����
	void createToolBox();
	void createToolbars();
	QWidget *createCellWidget(const QString &text, DiagramItem::DiagramType type);//����ͼԪ��ˢ��ť
	QWidget *createBackgroundCellWidget(const QString &text, const QString &image);//���������л���ť
private slots:
	///�ۺ���
	void itemInserted(DiagramItem *item);//����ͼԪ
	void itemSelected(QGraphicsItem *item);
	void bringToFront();
	void sendToBack();
	void deleteItem();
	void pointerGroupClicked(int id);//���ָ��
	void sceneScaleChanged(const QString &scale);//����
	void backgroundButtonGroupClicked(QAbstractButton *button);//���������
	void buttonGroupClicked(int id);//ѡ��ͼԪ,�����ͼ
};
