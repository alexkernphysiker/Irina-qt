// this file is distributed under
// GPL v 3.0 license
#ifndef SP1NORMERRVIEW_H
#define SP1NORMERRVIEW_H

#include <QGroupBox>
#include <SortLib.h>

namespace Ui {
class SP1NormErrView;
}

class SP1NormErrView : public QGroupBox
{
	Q_OBJECT

public:
	explicit SP1NormErrView(SP1Norm_CoefErr *sp1, QWidget *parent = 0);
	~SP1NormErrView();

private slots:
	void refr();
	void on_Apply_clicked();

private:
	Ui::SP1NormErrView *ui;
	SP1Norm_CoefErr *m_sp1;
};

#endif // SP1NORMERRVIEW_H
