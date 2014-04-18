#include "spaxisview.h"
#include "ui_spaxisview.h"

SPAxisView::SPAxisView(SoSPAxis *axis, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::SPAxisView)
{
	ui->setupUi(this);
	m_axis=axis;
	refr();
	connect(m_axis,SIGNAL(changed(SObject*)),this,SLOT(refr()));
}
void SPAxisView::refr(){
	ui->min->setValue(m_axis->Min());
	ui->max->setValue(m_axis->Max());
	ui->min->setValue(m_axis->Min());
	ui->chw->setValue(m_axis->ChannelW());
}

SPAxisView::~SPAxisView()
{
	delete ui;
}

void SPAxisView::on_min_valueChanged(double /*arg1*/)
{
	if(ui->max->value()<=ui->min->value())
		ui->min->setValue(ui->max->value()-0.00001);
}

void SPAxisView::on_max_valueChanged(double /*arg1*/)
{
	if(ui->max->value()<=ui->min->value())
		ui->max->setValue(ui->min->value()+0.00001);
}

void SPAxisView::on_chw_valueChanged(double /*arg1*/)
{
	if(ui->chw->value()<0.00001)ui->chw->setValue(0.00001);
}

void SPAxisView::on_ApplyButton_clicked()
{
	double min=ui->min->value();
	double max=ui->max->value();
	double chw=ui->chw->value();
	m_axis->SetMin(min);
	m_axis->SetMax(max);
	m_axis->SetMin(min);
	m_axis->SetChannelW(chw);
}
