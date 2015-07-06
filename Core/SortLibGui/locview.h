// this file is distributed under
// GPL v 3.0 license
#ifndef LOCVIEW_H
#define LOCVIEW_H

#include <QGroupBox>
#include <SortLib.h>


namespace Ui {
	class LocView;
}

class LocView : public QGroupBox
{
	Q_OBJECT

public:
	explicit LocView(SoEFLoc *loc, QWidget *parent = 0);
	~LocView();

private slots:
	void on_xbut_clicked();

	void on_ybut_clicked();
	void refr();
private:

	SoSPAxis *SelectedAxis();
	Ui::LocView *ui;
	SoEFLoc *m_loc;
};

#endif // LOCVIEW_H
