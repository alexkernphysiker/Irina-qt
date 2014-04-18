#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(Gnuplotter *father,QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::SettingsWindow)
{
	ui->setupUi(this);
	master=father;
	ui->lineEdit->setText(master->gnuplotpath);
	ui->checkBox->setChecked(master->create_ps);
	ui->checkBox_2->setChecked(master->create_png);
	ui->lineEdit_2->setText(master->font);
	ui->pngsize->setText(master->pngsize);
}

SettingsWindow::~SettingsWindow()
{
	delete ui;
}

void SettingsWindow::on_lineEdit_textChanged(const QString &arg1)
{
	master->gnuplotpath=arg1;
}

void SettingsWindow::on_checkBox_toggled(bool checked)
{
	master->create_ps=checked;
}

void SettingsWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
	master->font=arg1;
}



void SettingsWindow::on_checkBox_2_toggled(bool checked)
{
	master->create_png=checked;
	ui->pngsize->setEnabled(checked);
}

void SettingsWindow::on_pngsize_textChanged(const QString &arg1)
{
	master->pngsize=arg1;
}
