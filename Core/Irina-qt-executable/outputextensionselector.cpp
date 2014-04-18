#include <Core/SortLib/sortlib_.h>
#include "outputextensionselector.h"
#include "ui_outputextensionselector.h"
#include "plugin_output_interface.h"

OutputExtensionSelector::OutputExtensionSelector(QStringList lst, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::OutputExtensionSelector)
{
	ui->setupUi(this);
	setWindowTitle(OUTPUT_PLUGIN_INTERFACE);
	foreach(QString s,lst){
		ui->listWidget->addItem(s);
	}
	if(ui->listWidget->count()>0)
		ui->listWidget->setCurrentRow(0);
}

int OutputExtensionSelector::SelectedIndex(){
	return ui->listWidget->currentRow();
}

OutputExtensionSelector::~OutputExtensionSelector()
{
	delete ui;
}

void OutputExtensionSelector::on_pushButton_clicked()
{
	close();
}
