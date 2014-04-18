#ifndef SP1NORMVIEW_H
#define SP1NORMVIEW_H

#include <QGroupBox>
#include <SortLib.h>

namespace Ui {
class Sp1NormView;
}

class Sp1NormView : public QGroupBox
{
	Q_OBJECT

public:
	explicit Sp1NormView(SP1Norm *sp1, QWidget *parent = 0);
	~Sp1NormView();

private slots:
	void refr();
	void on_pushButton_clicked();

private:
	Ui::Sp1NormView *ui;
	SP1Norm *m_sp1;
};

#endif // SP1NORMVIEW_H
