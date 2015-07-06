// this file is distributed under
// GPL v 3.0 license
#ifndef FILTERVIEW_H
#define FILTERVIEW_H
#include <SortLib.h>
#include <QGroupBox>


namespace Ui {
	class FilterView;
}

class FilterView : public QGroupBox
{
	Q_OBJECT

public:
	explicit FilterView(SoEFilter *filter, QWidget *parent = 0);
	~FilterView();

private slots:
	void on_spinBox_valueChanged(int arg1);

	void on_nameedit_textChanged(const QString &arg1);

private:
	Ui::FilterView *ui;
	SoEFilter *m_filter;
};

#endif // FILTERVIEW_H
