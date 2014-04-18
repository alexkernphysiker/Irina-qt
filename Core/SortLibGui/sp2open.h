#ifndef SP2OPEN_H
#define SP2OPEN_H
#include <QDialog>
 #include <QKeyEvent>
#include <SortLib.h>

namespace Ui {
	class SP2Open;
}

class SP2Open : public QDialog
{
	Q_OBJECT

public:
	explicit SP2Open(SoECSP2 *sp2, SoSPEnergy *cal, QWidget *parent = 0);
	~SP2Open();

private slots:
	void busy(bool val);

	void on_up_clicked();

	void on_down_clicked();

	void on_left_clicked();

	void on_right_clicked();

	void on_scale_valueChanged(int value);

	void on_brightness_valueChanged(int value);

	void on_threshold_valueChanged(int value);

	void on_markersize_valueChanged(int value);


	void on_loctree_itemSelectionChanged();

	void on_e1_valueChanged(double arg1);

	void on_e2_valueChanged(double arg1);

	void on_e3_valueChanged(double arg1);

	void on_peak3_toggled(bool checked);

	void on_peak2_toggled(bool checked);

	void on_peak1_toggled(bool checked);

	void on_toolButton_clicked();

	void refrlocs(SObject *so);

	void close();
	void on_toolButton_2_clicked();

	void on_pushButton_clicked();

	void on_colorslider_valueChanged(int value);

	void on_LocCol_valueChanged(int value);

	void on_MarkerCol_valueChanged(int value);

private:
	void paintEvent(QPaintEvent *ev);
	void keyPressEvent(QKeyEvent *ke);
	void checkpeaks();
	SoEFLoc *SelectedLocus();
	Ui::SP2Open *ui;
	SoECSP2 *m_sp2;
	int origx;
	int origy;
	int offsx;
	int offsy;
	int markerx;
	int markery;
	SoSPEnergy *m_cal;
	QList<QPoint> points;
};

#endif // SP2OPEN_H
