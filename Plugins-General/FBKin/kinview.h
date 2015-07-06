// this file is distributed under
// GPL v 3.0 license
#ifndef KINVIEW_H
#define KINVIEW_H

#include <QGroupBox>
#include <SortLib.h>
#include <QTreeWidget>
#include "fbkin.h"

namespace Ui {
	class KinView;
}

class KinView : public QGroupBox
{
	Q_OBJECT

public:
	explicit KinView(SoFBKin *kin, QWidget *parent = 0);
	~KinView();

private slots:
	void on_nameedit_textChanged(const QString &arg1);

	void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_mpedit_textChanged(const QString &arg1);

	void on_mtedit_textChanged(const QString &arg1);

	void on_mredit_textChanged(const QString &arg1);

	void on_epedit_textChanged(const QString &arg1);

	void on_qedit_textChanged(const QString &arg1);

	void on_thetaedit_textChanged(const QString &arg1);

private:
	Ui::KinView *ui;
	SoFBKin *m_kin;
};

#endif // KINVIEW_H
