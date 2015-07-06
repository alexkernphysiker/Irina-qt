// this file is distributed under
// GPL v 3.0 license
#ifndef SP1VIEW_H
#define SP1VIEW_H

#include <QGroupBox>
#include <SortLib.h>
#include "additional.h"


namespace Ui {
	class SP1View;
}

class SP1View : public QGroupBox
{
	Q_OBJECT

public:
	explicit SP1View(SoECSP1 *sp1, QWidget *parent = 0);
	~SP1View();

private slots:
	void on_AxisTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_refrbutt_clicked();

	void busy(bool val);

	void on_axis_button_clicked();

	void on_open_but_clicked();

	void on_DataOutButton_clicked();

private:
	void refr();
	Ui::SP1View *ui;
	SoECSP1 *m_sp1;
};

#endif // SP1VIEW_H
