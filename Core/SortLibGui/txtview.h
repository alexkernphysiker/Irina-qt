// this file is distributed under
// GPL v 3.0 license
#ifndef TXTVIEW_H
#define TXTVIEW_H

#include <QGroupBox>
#include <SortLib.h>

namespace Ui {
class txtview;
}

class txtview : public QGroupBox
{
	Q_OBJECT

public:
	explicit txtview(SoTblFuncTxt *father, QWidget *parent = 0);
	~txtview();

private slots:
	void on_toolButton_clicked();
	void refr();
	void on_col0_valueChanged(int arg1);

	void on_col1_valueChanged(int arg1);

	void on_col2_valueChanged(int arg1);

private:
	Ui::txtview *ui;
	SoTblFuncTxt *master;
};

#endif // TXTVIEW_H
