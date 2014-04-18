#ifndef MIXEDVIEW_H
#define MIXEDVIEW_H

#include <QGroupBox>
#include <simulation.h>

namespace Ui {
class MixedView;
}

class MixedView : public QGroupBox
{
	Q_OBJECT

public:
	explicit MixedView(MixDistributions *father, QWidget *parent = 0);
	~MixedView();
private slots:
	void refr();
	void on_add_clicked();

	void on_remove_clicked();

private:
	Ui::MixedView *ui;
	MixDistributions *master;
};

#endif // MIXEDVIEW_H
