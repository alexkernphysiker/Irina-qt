#include "extmaskview.h"
#include "ui_extmaskview.h"

ExtMaskView::ExtMaskView(ExtendedMask *father, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::ExtMaskView)
{
	master=father;
	ui->setupUi(this);
	connect(master,SIGNAL(changed(SObject*)),this,SLOT(refr()));
	refr();
}

ExtMaskView::~ExtMaskView()
{
	delete ui;
}

void ExtMaskView::refr(){
	ui->opr1->setText(master->Func1());
	ui->opr2->setText(master->Func2());
	ui->pred->setText(master->Predicate());
}

void ExtMaskView::on_pushButton_clicked()
{
	QString o2=ui->opr2->text();
	QString p=ui->pred->text();
	master->SetFunc1(ui->opr1->text());
	master->SetFunc2(o2);
	master->SetPredicate(p);
}
