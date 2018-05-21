#pragma once
#include "Diagramitem.h"

#include <QGraphicsScene>
#include <QAction>
/*************************************************************************
���:��ʹ΢����
ʱ��:2017��4��7��15:34:15

��־:����QGraphicsScene�ؼ���,��д���е������Ӧ����,ʵ�ֶ����ͼ�ȹ���
**************************************************************************/

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

//! [0]
class DiagramScene : public QGraphicsScene
{
	Q_OBJECT

public:
	enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

	explicit DiagramScene(QObject *parent = 0);
	QFont font() const { return myFont; }
	QColor textColor() const { return myTextColor; }
	QColor itemColor() const { return myItemColor; }
	QColor lineColor() const { return myLineColor; }
	void setLineColor(const QColor &color);
	void setTextColor(const QColor &color);
	void setItemColor(const QColor &color);
	void setFont(const QFont &font);

	public slots:
	void setMode(Mode mode);
	void setItemType(DiagramItem::DiagramType type);
	//void editorLostFocus(DiagramTextItem *item);

signals:
	void itemInserted(DiagramItem *item);//�������µ�ͼԪ 
	void textInserted(QGraphicsTextItem *item);//����������
	void itemSelected(QGraphicsItem *item);//ѡ����ͼԪ

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
private:
	///˽�з���

private:
	///�ڲ�����
	bool isItemChange(int type);

	DiagramItem::DiagramType myItemType;
	Mode myMode;
	bool leftButtonDown;
	QPointF startPoint;
	QGraphicsLineItem *line;
	QFont myFont;
	//DiagramTextItem *textItem;
	QColor myTextColor;
	QColor myItemColor;
	QColor myLineColor;
};
