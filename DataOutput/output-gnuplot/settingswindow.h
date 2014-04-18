#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include "gnuplotter.h"
namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit SettingsWindow(Gnuplotter *father,QWidget *parent = 0);
	~SettingsWindow();

private slots:
	void on_lineEdit_textChanged(const QString &arg1);

	void on_checkBox_toggled(bool checked);

	void on_lineEdit_2_textChanged(const QString &arg1);

	void on_checkBox_2_toggled(bool checked);

	void on_pngsize_textChanged(const QString &arg1);

private:
	Ui::SettingsWindow *ui;
	Gnuplotter *master;
};

#endif // SETTINGSWINDOW_H
