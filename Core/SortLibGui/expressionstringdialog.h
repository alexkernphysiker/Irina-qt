#ifndef EXPRESSIONSTRINGDIALOG_H
#define EXPRESSIONSTRINGDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class ExpressionStringDialog;
}

class ExpressionStringDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ExpressionStringDialog(QWidget *parent = 0);
	~ExpressionStringDialog();
	QString ExprStr();
	void SetExprStr(QString str);

private:
	Ui::ExpressionStringDialog *ui;
};

#endif // EXPRESSIONSTRINGDIALOG_H
