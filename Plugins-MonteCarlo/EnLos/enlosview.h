// this file is distributed under
// GPL v 3.0 license
#ifndef ENLOSVIEW_H
#define ENLOSVIEW_H

#include <QGroupBox>
#include <enlos.h>

namespace Ui {
class EnLosView;
}

class EnLosView : public QGroupBox
{
	Q_OBJECT

public:
	explicit EnLosView(EnLos *los, QWidget *parent = 0);
	~EnLosView();

private slots:
	void refr();

	void on_name_textChanged(const QString &arg1);

	void on_energyname_textChanged(const QString &arg1);

	void on_lengthname_textChanged(const QString &arg1);

	void on_dx_valueChanged(double arg1);

private:
	Ui::EnLosView *ui;
	EnLos *m_los;
};

#endif // ENLOSVIEW_H
