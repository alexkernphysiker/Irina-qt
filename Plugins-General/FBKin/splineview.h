#ifndef SPLINEVIEW_H
#define SPLINEVIEW_H

#include <QGroupBox>
#include <fbkin.h>

namespace Ui {
class splineview;
}

class splineview : public QGroupBox
{
	Q_OBJECT

public:
	explicit splineview(SoSP1Line *spline, QWidget *parent = 0);
	~splineview();

private slots:
	void refr();
	void on_name_textChanged(const QString &arg1);

	void on_Mp_textChanged(const QString &arg1);

	void on_MT_textChanged(const QString &arg1);

	void on_Mx_textChanged(const QString &arg1);

	void on_Theta_textChanged(const QString &arg1);

	void on_Eres_textChanged(const QString &arg1);

	void on_N_textChanged(const QString &arg1);

	void on_sigma_textChanged(const QString &arg1);

private:
	Ui::splineview *ui;
	SoSP1Line *m_spline;
};

#endif // SPLINEVIEW_H
