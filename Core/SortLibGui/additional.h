#ifndef ADDITIONAL_H
#define ADDITIONAL_H

#include <QtCore/QObject>
#include <QListWidget>
#include <QTreeWidget>
#include <Core/SortLib/sortlib_.h>

void MsgBox(QString txt);
bool AskYesOrNo(QString str);
class SortProjectView;
class Adder: public QObject{
	Q_OBJECT
public:
	Adder(SoTypeReg* reg, SortProjectView* parent);
	virtual ~Adder();
public slots:
	void Triger();
private:
	SoTypeReg* m_reg;
	SortProjectView* m_parent;
};
class SObjectTreeWidgetItem;
class ExprTreeWidgetItem;
class PeakTreeWidgetItem;
class myTreeWidget:public QTreeWidget{
	Q_OBJECT
  public:
	myTreeWidget(QWidget* parent, SortProject *sp);
	~myTreeWidget();
	SObject *SelectedSObject();
signals:
	void FlushInfoViewer(QWidget*);
public slots:
	void Refresh(SObject* tosel);
	void ViewInfo();
	void OpenItem(QTreeWidgetItem* itm, int /*column*/);
	void changed(SObject *obj);
private:
	void LookForSelection(SObjectTreeWidgetItem *itm, SObject *so);
	SortProject *m_project;
	QWidget *m_view;
};

#endif // ADDITIONAL_H
