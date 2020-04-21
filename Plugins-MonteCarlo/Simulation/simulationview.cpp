// this file is distributed under
// GPL v 3.0 license
#include "simulationview.h"
#include "ui_simulationview.h"

SimulationView::SimulationView(Simulation *father, QWidget *parent):
	QGroupBox(parent),
	ui(new Ui::SimulationView)
{
	master=father;
	ui->setupUi(this);
	busy=false;
	refr();
	connect(master, SIGNAL(changed(SObject*)),this, SLOT(refr()));
}

SimulationView::~SimulationView()
{
	delete ui;
}

void SimulationView::on_nameedit_textChanged(const QString &arg1)
{
	master->Set_Name(arg1);
}

void SimulationView::refr(){
	if(busy) return;
	busy=true;
	//ui->nameedit->setText(master->Name());
	ui->spinBox->setValue(master->N());
	QString msg="";
	for(char i=0; i<master->NADC();i++){
		msg+=master->Func(i);
		if(i<(master->NADC()-1))msg+="\n";
	}
	ui->textEdit->setPlainText(msg);
	busy=false;
}

void SimulationView::on_pushButton_clicked()
{
	if(busy) return;
	busy=true;
	QStringList lst=ui->textEdit->toPlainText().split('\n');
	int N=lst.count();
	if(N>128)N=128;
	master->SetNADC((char)N);
	master->SetFuncs(lst);
	busy=false;
}

void SimulationView::on_pushButton_2_clicked()
{
	master->Read();
}

void SimulationView::on_evcnt_clicked()
{
	master->Owner()->Add(new SoECounter(master));
}

void SimulationView::on_sp2_clicked()
{
	master->Owner()->Add(new SoECSP2(master));
}

void SimulationView::on_sp1_clicked()
{
	master->Owner()->Add(new SoECSP1(master));
}

void SimulationView::on_spinBox_valueChanged(int arg1)
{
	master->SetN(arg1);
}
