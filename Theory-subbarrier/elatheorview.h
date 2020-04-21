// this file is distributed under
// GPL v 3.0 license
#ifndef ELATHEORVIEW_H
#define ELATHEORVIEW_H

#include <QDialog>
#include "sbdi_theor.h"

namespace Ui {
	class ElaTheorView;
}

class ElaTheorView : public QDialog
{
	Q_OBJECT

public:
	explicit ElaTheorView(ElaTheor *ela, QWidget *parent = 0);
	~ElaTheorView();

private slots:
	void on_Z_valueChanged(int arg1);

	void on_min_valueChanged(double arg1);

	void on_max_valueChanged(double arg1);

	void on_step_valueChanged(double arg1);

	void on_second_valueChanged(double arg1);

	void on_nameedit_textChanged(const QString &arg1);

private:
	Ui::ElaTheorView *ui;
	ElaTheor *m_ela;
};

#endif // ELATHEORVIEW_H
