// this file is distributed under
// GPL v 3.0 license
#ifndef RANDOMVIEW_H
#define RANDOMVIEW_H

#include <simulation.h>
#include <QGroupBox>

namespace Ui {
class RandomView;
}

class RandomView : public QGroupBox
{
	Q_OBJECT

public:
	explicit RandomView(RandomMagnitude *father, QWidget *parent = 0);
	~RandomView();
	void AddWidget(QWidget *widg, QString name);
private slots:
	void on_lineEdit_textChanged(const QString &arg1);
	void refr();
private:
	Ui::RandomView *ui;
	RandomMagnitude *master;
};

#endif // RANDOMVIEW_H
