// this file is distributed under
// GPL v 3.0 license
#ifndef DATAGROUPVIEW_H
#define DATAGROUPVIEW_H
#include "datagroup.h"
#include <QGroupBox>
#include <QTreeWidgetItem>

namespace Ui {
class DataGroupView;
}

class DataGroupView : public QGroupBox
{
	Q_OBJECT

public:
	explicit DataGroupView(DataGroup* group,QWidget *parent = 0);
	~DataGroupView();
public slots:
	void changed(SObject* );

private slots:
	void on_DGTypes_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_SourceTypes_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_RemoveButton_clicked();

private:
	Ui::DataGroupView *ui;
	DataGroup* m_master;
};

#endif // DATAGROUPVIEW_H
