// this file is distributed under
// GPL v 3.0 license
#include "thiswidget.h"
#include "ui_thiswidget.h"

ThisWidget::ThisWidget(NumericErrorsCalculation *numerr, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::ThisWidget)
{
	ui->setupUi(this);
	m_numerr=numerr;
	ui->name->setText( m_numerr->Name());
	ui->funcname->setText( m_numerr->FuncName());
	refr();
	connect(m_numerr,SIGNAL(changed(SObject*)),this, SLOT(refr()));
}

ThisWidget::~ThisWidget()
{
	delete ui;
}

void ThisWidget::on_name_textChanged(const QString &arg1)
{
	m_numerr->Set_Name(arg1);
}

void ThisWidget::on_funcname_textChanged(const QString &arg1)
{
	m_numerr->SetFuncName(arg1);
}

void ThisWidget::refr(){
	QString msg="";
	for(int i=0;i<m_numerr->Count();i++){
		QString n=m_numerr->ErrName(i);
		double v=m_numerr->ErrValue(i);
		msg+=QString::number(i)+") delta("+n+") = "+QString::number(v)+"\n";
	}
	ui->info->setText(msg);
}

void ThisWidget::on_SetButton_clicked()
{
	m_numerr->SetError(ui->nname->text(),ui->doubleSpinBox->value());
}

void ThisWidget::on_Removebutton_clicked()
{
	m_numerr->RemoveErr(ui->spinBox->value());
}
