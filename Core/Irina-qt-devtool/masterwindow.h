#ifndef MASTERWINDOW_H
#define MASTERWINDOW_H

#include <QMainWindow>

namespace Ui {
class MasterWindow;
}

class MasterWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MasterWindow(QWidget *parent = 0);
	~MasterWindow();

private slots:
	void on_browse_clicked();

	void on_create_clicked();

	void on_choose_data_toggled(bool checked);

	void on_choose_func_toggled(bool checked);

	void on_choose_filter_toggled(bool checked);

	void on_choose_out_toggled(bool checked);

	void on_choose_other_toggled(bool checked);

private:
	Ui::MasterWindow *ui;
};

#endif // MASTERWINDOW_H
