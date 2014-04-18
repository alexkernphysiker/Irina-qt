#ifndef PLOTTERVIEW_H
#define PLOTTERVIEW_H

#include <QDialog>

namespace Ui {
class PlotterView;
}

class PlotParameters;

class PlotterView : public QDialog
{
	Q_OBJECT

public:
	explicit PlotterView(QWidget *parent = 0);
	~PlotterView();
public slots:
	void TakeData(QStringList &data);
private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
	void on_pushButton_clicked();

protected:
	void closeEvent(QCloseEvent *);
signals:
	void info(bool,PlotParameters*);
	void settings();
private:
	Ui::PlotterView *ui;
	bool acc;
	PlotParameters* params;
};

#endif // PLOTTERVIEW_H
