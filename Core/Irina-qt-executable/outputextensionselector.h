#ifndef OUTPUTEXTENSIONSELECTOR_H
#define OUTPUTEXTENSIONSELECTOR_H
#include <QDialog>
#include <QStringList>

namespace Ui {
class OutputExtensionSelector;
}

class OutputExtensionSelector : public QDialog
{
	Q_OBJECT

public:
	explicit OutputExtensionSelector(QStringList lst, QWidget *parent = 0);
	~OutputExtensionSelector();
	int SelectedIndex();
private slots:
	void on_pushButton_clicked();

private:
	Ui::OutputExtensionSelector *ui;
};

#endif // OUTPUTEXTENSIONSELECTOR_H
