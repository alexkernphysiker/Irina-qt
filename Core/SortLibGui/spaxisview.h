#ifndef SPAXISVIEW_H
#define SPAXISVIEW_H

#include <SortLib.h>
#include <QGroupBox>


namespace Ui {
	class SPAxisView;
}

class SPAxisView : public QGroupBox
{
	Q_OBJECT

public:
	explicit SPAxisView(SoSPAxis *axis, QWidget *parent = 0);
	~SPAxisView();

private slots:
	void on_min_valueChanged(double arg1);

	void on_max_valueChanged(double arg1);

	void on_chw_valueChanged(double arg1);

	void refr();

	void on_ApplyButton_clicked();

private:
	Ui::SPAxisView *ui;
	SoSPAxis *m_axis;

};

#endif // SPAXISVIEW_H
