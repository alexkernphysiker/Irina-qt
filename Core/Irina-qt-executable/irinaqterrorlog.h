// this file is distributed under
// GPL v 3.0 license
#ifndef ERRORLOG_H
#define ERRORLOG_H
#include <QDialog>
namespace Ui {
	class ErrorLog;
}

class ErrorLog : public QDialog
{
	Q_OBJECT
public:
	static ErrorLog *GetInstance();
public:
	explicit ErrorLog(QWidget *parent = 0);
	~ErrorLog();
public slots:
	void error(QString msg);
private slots:
	void on_textEdit_destroyed();

private:
	Ui::ErrorLog *ui;
};

#endif // ERRORLOG_H
