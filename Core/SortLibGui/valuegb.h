#ifndef VALUEGB_H
#define VALUEGB_H

#include <QGroupBox>
#include <SortLib.h>


namespace Ui {
	class valuegb;
}

class valuegb : public QGroupBox
{
	Q_OBJECT

public:
	explicit valuegb(SoValue *value, QWidget *parent = 0);
	~valuegb();

private slots:
	void on_pushButton_clicked();

private:
	Ui::valuegb *ui;
	SoValue *m_value;
};

#endif // VALUEGB_H
