#include "mixedview.h"
#include "ui_mixedview.h"

MixedView::MixedView(MixDistributions *father, QWidget *parent):
	QGroupBox(parent),
	ui(new Ui::MixedView)
{
	master=father;
	ui->setupUi(this);
	refr();
	connect(master,SIGNAL(changed(SObject*)),this, SLOT(refr()));
}

MixedView::~MixedView()
{
	delete ui;
}
void MixedView::refr(){
	QString msg="";
	for(uint i=0; i<master->Count();i++){
		msg+=QString::number(i)+") Func = "+master->Function(i)+" ; P = "+QString::number(master->Probability(i))+"\n";
	}
	ui->plainTextEdit->setPlainText(msg);
}

void MixedView::on_add_clicked()
{
	master->Add(ui->name->text(),ui->prob->value());
}


void MixedView::on_remove_clicked()
{
	master->Remove(ui->spinBox->value());
}
