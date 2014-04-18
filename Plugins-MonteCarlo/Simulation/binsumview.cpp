#include "binsumview.h"
#include "ui_binsumview.h"

BinSumView::BinSumView(SoBinarySumm *summ, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::BinSumView)
{
	m_summ=summ;
	ui->setupUi(this);
	refr();
	connect(m_summ, SIGNAL(changed(SObject*)),this, SLOT(refr()));
}

BinSumView::~BinSumView()
{
	delete ui;
}

void BinSumView::refr(){
	ui->name->setText(m_summ->Name());
	QString msg="";
	for(int i=0; i<m_summ->Count();i++){
		msg+=m_summ->Summand(i)+"\n";
	}
	ui->sumnames->setPlainText(msg);
}

void BinSumView::on_name_textChanged(const QString &arg1)
{
	m_summ->Set_Name(arg1);
}

void BinSumView::on_ApplyButton_clicked()
{
	QStringList lst=ui->sumnames->toPlainText().split('\n',QString::SkipEmptyParts);
	m_summ->Reset();
	for(int i=0; i<lst.count();i++)
		m_summ->AddSummand(lst.at(i));
}
