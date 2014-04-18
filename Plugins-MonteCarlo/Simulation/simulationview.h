#ifndef SIMULATIONVIEW_H
#define SIMULATIONVIEW_H

#include <QGroupBox>
#include <simulation.h>

namespace Ui {
class SimulationView;
}

class SimulationView : public QGroupBox
{
	Q_OBJECT

public:
	explicit SimulationView(Simulation *father, QWidget *parent = 0);
	~SimulationView();
private slots:
	void refr();
	void on_nameedit_textChanged(const QString &arg1);

	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_evcnt_clicked();

	void on_sp2_clicked();

	void on_sp1_clicked();

	void on_spinBox_valueChanged(int arg1);

private:
	Ui::SimulationView *ui;
	Simulation *master;
	bool busy;
};

#endif // SIMULATIONVIEW_H
