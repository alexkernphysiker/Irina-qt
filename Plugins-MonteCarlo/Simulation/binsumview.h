#ifndef BINSUMVIEW_H
#define BINSUMVIEW_H

#include <QGroupBox>
#include <simulation.h>

namespace Ui {
class BinSumView;
}

class BinSumView : public QGroupBox
{
	Q_OBJECT

public:
	explicit BinSumView(SoBinarySumm *summ, QWidget *parent = 0);
	~BinSumView();

private slots:
	void refr();
	void on_name_textChanged(const QString &arg1);

	void on_ApplyButton_clicked();

private:
	Ui::BinSumView *ui;
	SoBinarySumm *m_summ;
};

#endif // BINSUMVIEW_H
