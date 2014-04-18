#ifndef RUTHVIEW_H
#define RUTHVIEW_H

#include <QGroupBox>
#include <fbkin.h>

namespace Ui {
class RuthView;
}

class RuthView : public QGroupBox
{
	Q_OBJECT

public:
	explicit RuthView(SoRuth *r, QWidget *parent = 0);
	~RuthView();

private slots:
	void refr();
	void on_name_textChanged(const QString &arg1);

	void on_zp_textChanged(const QString &arg1);

	void on_zt_textChanged(const QString &arg1);

	void on_e_textChanged(const QString &arg1);

	void on_theta_textChanged(const QString &arg1);

private:
	Ui::RuthView *ui;
	SoRuth *ruth;
};

#endif // RUTHVIEW_H
