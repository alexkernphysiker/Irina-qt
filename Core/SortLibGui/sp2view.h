// this file is distributed under
// GPL v 3.0 license
#ifndef SP2VIEW_H
#define SP2VIEW_H

#include <QGroupBox>
#include <SortLib.h>


namespace Ui {
	class SP2View;
}

class   SP2View : public QGroupBox
{
	Q_OBJECT

public:
	explicit SP2View(SoECSP2 *sp2, QWidget *parent = 0);
	~SP2View();

private slots:
	void busy(bool val);
	void on_makeaxis_clicked();

	void on_setx_clicked();

	void on_sety_clicked();

	void on_refrbut_clicked();

	void on_openbut_clicked();

	void on_exportbut_clicked();

private:
	void refr();
	SoSPAxis *SelectedAxis();
	Ui::SP2View *ui;
	SoECSP2 *m_sp2;
};

#endif // SP2VIEW_H
