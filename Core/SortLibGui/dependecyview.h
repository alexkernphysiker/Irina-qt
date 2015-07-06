// this file is distributed under
// GPL v 3.0 license
#ifndef DEPENDECYVIEW_H
#define DEPENDECYVIEW_H

#include <QGroupBox>
#include <SortLib.h>
#include <QTreeWidget>

namespace Ui {
	class DependecyView;
}

class DependecyView : public QGroupBox
{
	Q_OBJECT

public:
	explicit DependecyView(SoDependency *dep, QWidget *parent = 0);
	~DependecyView();

private slots:
	void on_nameedit_textChanged(const QString &arg1);

	void on_argedit_textChanged(const QString &arg1);

	void on_valedit_textChanged(const QString &arg1);

	void on_erredit_textChanged(const QString &arg1);

	void on_avtree_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_usedtree_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_pushButton_clicked();

	void on_sewname_textChanged(const QString &arg1);

	void on_xerror_textChanged(const QString &arg1);

private:
	void refr();
	Ui::DependecyView *ui;
	SoDependency *m_dependency;
};

#endif // DEPENDECYVIEW_H
