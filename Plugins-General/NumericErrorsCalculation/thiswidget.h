#ifndef THISWIDGET_H
#define THISWIDGET_H

#include <QGroupBox>
#include "numericerrorscalculation.h"

namespace Ui {
	class ThisWidget;
}

class ThisWidget : public QGroupBox
{
	Q_OBJECT

public:
	explicit ThisWidget(NumericErrorsCalculation *numerr, QWidget *parent = 0);
	~ThisWidget();

private slots:
	void on_name_textChanged(const QString &arg1);

	void on_funcname_textChanged(const QString &arg1);

	void refr();

	void on_SetButton_clicked();

	void on_Removebutton_clicked();

private:
	Ui::ThisWidget *ui;
	NumericErrorsCalculation *m_numerr;
};

#endif // THISWIDGET_H
