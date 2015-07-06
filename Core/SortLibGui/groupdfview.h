// this file is distributed under
// GPL v 3.0 license
#ifndef GROUPDFVIEW_H
#define GROUPDFVIEW_H

#include <QGroupBox>
#include <QTreeWidget>
#include <SortLib.h>


namespace Ui {
	class GroupDFView;
}

class GroupDFView : public QGroupBox
{
	Q_OBJECT

public:
	explicit GroupDFView(SoGroupDF *group, QWidget *parent = 0);
	~GroupDFView();

private slots:
	void on_nameedit_textChanged(const QString &arg1);

	void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_avtree_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_usedtree_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_expredit_textChanged(const QString &arg1);

private:
	void refr();
	Ui::GroupDFView *ui;
	SoGroupDF *m_group;
};

#endif // GROUPDFVIEW_H
