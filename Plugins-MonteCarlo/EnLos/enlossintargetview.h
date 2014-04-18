#ifndef ENLOSSINTARGETVIEW_H
#define ENLOSSINTARGETVIEW_H

#include <QGroupBox>
#include <SortLib.h>
#include <enlos.h>

namespace Ui {
class EnLossInTargetView;
}

class EnLossInTargetView : public QGroupBox
{
	Q_OBJECT

public:
	explicit EnLossInTargetView(EnLossInTarget *loss, QWidget *parent = 0);
	~EnLossInTargetView();
private slots:
	void refr();
	void on_name_textChanged(const QString &arg1);

	void on_energy_textChanged(const QString &arg1);

	void on_thetad_textChanged(const QString &arg1);

	void on_thetat_textChanged(const QString &arg1);

	void on_targetthickness_textChanged(const QString &arg1);

	void on_pstp_textChanged(const QString &arg1);

	void on_xstp_textChanged(const QString &arg1);

	void on_dx_valueChanged(double arg1);

	void on_crosssection_textChanged(const QString &arg1);

	void on_de_valueChanged(double arg1);

	void on_emax_valueChanged(double arg1);

private:
	Ui::EnLossInTargetView *ui;
	EnLossInTarget *m_loss;
};

#endif // ENLOSSINTARGETVIEW_H
