// this file is distributed under
// GPL v 3.0 license
#ifndef EXTMASKVIEW_H
#define EXTMASKVIEW_H

#include <masks.h>
#include <QGroupBox>

namespace Ui {
class ExtMaskView;
}

class ExtMaskView : public QGroupBox
{
	Q_OBJECT

public:
	explicit ExtMaskView(ExtendedMask *father, QWidget *parent = 0);
	~ExtMaskView();

private slots:
	void refr();
	void on_pushButton_clicked();

private:
	ExtendedMask *master;
	Ui::ExtMaskView *ui;
};

#endif // EXTMASKVIEW_H
