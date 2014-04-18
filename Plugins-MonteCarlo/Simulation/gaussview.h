#ifndef GAUSSVIEW_H
#define GAUSSVIEW_H

#include <QGroupBox>
#include <simulation.h>

namespace Ui {
class GaussView;
}

class GaussView : public QGroupBox
{
	Q_OBJECT

public:
	explicit GaussView(GaussMagnitude *father, QWidget *parent = 0);
	~GaussView();

private slots:
	void on_average_valueChanged(double arg1);
	void refr();

	void on_sigma_valueChanged(double arg1);

	void on_precision_valueChanged(int arg1);

private:
	Ui::GaussView *ui;
	GaussMagnitude *master;
};

#endif // GAUSSVIEW_H
