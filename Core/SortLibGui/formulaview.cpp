// this file is distributed under
// GPL v 3.0 license
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include "formulaview.h"
#include "ui_formulaview.h"
#include "ecalview.h"
#include "additional.h"
#include <SortLibGui.h>
#include "expressionstringdialog.h"
#include <expression/expression.h>

ExprTreeWidgetItem::ExprTreeWidgetItem(Math_::Expr *expr, QTreeWidget *w):QTreeWidgetItem(w){
	m_expr=expr;
	if(m_expr->Type()==MATH__EXPR_NUMBER){
		setText(0,"value="+QString::number(((Math_::Number*)m_expr)->Value()));
	}
	if(m_expr->Type()==MATH__EXPR_VARIABLE){
		setText(0,"identifier '"+((Math_::Variable*)m_expr)->Name()+"'");
	}
	if(m_expr->Type()==MATH__EXPR_UNARY){
		setText(0,"unary '"+((Math_::Unary*)m_expr)->Name()+"'");
		if(((Math_::Unary*)m_expr)->Operand()!=NULL){
			ExprTreeWidgetItem *item=new ExprTreeWidgetItem(((Math_::Unary*)m_expr)->Operand(),w);
			addChild(item);
		}
	}
	if(m_expr->Type()==MATH__EXPR_BINARY){
		setText(0,"binary '"+((Math_::Binary*)m_expr)->Name()+"'");
		if(((Math_::Binary*)m_expr)->Operand()!=NULL){
			ExprTreeWidgetItem *item=new ExprTreeWidgetItem(((Math_::Binary*)m_expr)->Operand(),w);
			addChild(item);
		}
		if(((Math_::Binary*)m_expr)->Operand2()!=NULL){
			ExprTreeWidgetItem *item=new ExprTreeWidgetItem(((Math_::Binary*)m_expr)->Operand2(),w);
			addChild(item);
		}
	}
}
ExprTreeWidgetItem::~ExprTreeWidgetItem(){}
Math_::Expr *ExprTreeWidgetItem::myobj(){return m_expr;}
bool ExprTreeWidgetItem::changed(Math_::Expr *expr){
	bool res=false;
	if(m_expr==expr){
		res=true;
		setSelected(true);
	}
	for(int i=0; (!res)&(i<childCount());i++)
		res|=((ExprTreeWidgetItem*)child(i))->changed(expr);
	return res;
}

FormulaView::FormulaView(SoFormula *formula, QWidget *parent):
	QGroupBox(parent),
	ui(new Ui::FormulaView)
{
	m_formula=formula;
	connect(m_formula,SIGNAL(changedexpr(Math_::Expr*)),
			this,SLOT(on_expression_changed(Math_::Expr*)));
	ui->setupUi(this);
	ui->lineEdit->setText(m_formula->Name());
	on_expression_changed(NULL);
	if(m_formula->Is(SOT_SPAxis)){
		this->setTitle("Spectrum Axis configuration");
		m_daughterview=new SPAxisView((SoSPAxis*)m_formula,this);
		m_daughterview->setVisible(false);
		m_daughterview->move(0,ui->treeWidget->y()+30);
		ui->additional->setVisible(true);
	}else{
		m_daughterview=NULL;
		ui->additional->setVisible(false);
	}
	if(m_formula->Is(SOT_SPEnergy)){
		ui->calbutt->setVisible(true);
		m_dv2=new ECalView((SoSPEnergy*)m_formula,this);
		m_dv2->setVisible(false);
		m_dv2->move(0,ui->treeWidget->y()+30);
		ui->ParseButton->setVisible(false);
		ui->pushButton->setVisible(false);
		ui->label->setEnabled(false);
		ui->glbinary->setEnabled(false);
		ui->glnumber->setEnabled(false);
		ui->glunary->setEnabled(false);
		ui->glvariable->setEnabled(false);
		ui->treeWidget->setEnabled(false);
	}else{
		ui->calbutt->setVisible(false);
		m_dv2=NULL;
	}

}

FormulaView::~FormulaView()
{
	delete ui;
}

void FormulaView::on_expression_changed(Math_::Expr *expr){
	ui->treeWidget->clear();
	if(m_formula->Expression()!=NULL)
		ui->treeWidget->addTopLevelItem(new ExprTreeWidgetItem(m_formula->Expression()));
	if(expr!=NULL)
		if(ui->treeWidget->topLevelItemCount()>0){
			ExprTreeWidgetItem *item= (ExprTreeWidgetItem*)ui->treeWidget->topLevelItem(0);
			item->changed(expr);
		}
	ui->treeWidget->expandAll();
	on_treeWidget_itemSelectionChanged();
}
Math_::Expr *FormulaView::SelectedExpr(){
	Math_::Expr *res=NULL;
	if(ui->treeWidget->selectedItems().count()>0){
		ExprTreeWidgetItem *item=(ExprTreeWidgetItem*)ui->treeWidget->selectedItems().at(0);
		res=item->myobj();
	}
	return res;
}
void FormulaView::on_treeWidget_itemSelectionChanged()
{
	ui->nameedit->setVisible(false);
	ui->doubleSpinBox->setVisible(false);
	ui->groupBox->setVisible(false);
	if(!ui->treeWidget->isEnabled())return;
	Math_::Expr *expr=SelectedExpr();
			if(expr!=NULL){
				if(m_daughterview!=NULL)
					m_daughterview->setVisible(false);
				ui->groupBox->setVisible(true);
				if(expr->Type()=='#'){
					ui->doubleSpinBox->setValue(((Math_::Number*)expr)->Value());
					ui->doubleSpinBox->setVisible(true);
				}
				if(expr->Type()=='$'){
					ui->nameedit->setText(((Math_::Variable*)expr)->Name());
					ui->nameedit->setVisible(true);
				}
				if((expr->Type()=='!')|(expr->Type()=='@')){
					ui->nameedit->setText(((Math_::Unary*)expr)->Name());
					ui->nameedit->setVisible(true);
				}
			}
}
const QString alltree="This will replace all expression tree. Continue?";
Math_::Unary *newunary(){
	Math_::Unary *res=new Math_::Unary();
	res->SetOperand(new Math_::Number());
	return res;
}
Math_::Binary *newbinary(){
	Math_::Binary *res=new Math_::Binary();
	res->SetOperand(new Math_::Number());
	res->SetOperand2(new Math_::Number());
	return res;
}
void FormulaView::applyexpr(Math_::Expr *expr){
	if(m_formula->Expression()==NULL){
		m_formula->Set_Expression(expr);
	}else{
		if(SelectedExpr()!=NULL){
			Math_::Expr *parent=m_formula->Expression()->LookForParent(SelectedExpr());
			if(parent!=NULL){
				if(parent->Type()==MATH__EXPR_UNARY)
					((Math_::Unary *)parent)->SetOperand(expr);
				else{
					if(parent->Type()==MATH__EXPR_BINARY){
						if(((Math_::Binary *)parent)->Operand()==SelectedExpr())
							((Math_::Binary *)parent)->SetOperand(expr);
						else
							((Math_::Binary *)parent)->SetOperand2(expr);
					}
				}
			}else{
				m_formula->Set_Expression(expr);
			}
		}
	}
}
void FormulaView::on_glnumber_clicked()
{
	applyexpr(new Math_::Number());
}

void FormulaView::on_glvariable_clicked()
{
	applyexpr(new Math_::Variable());
}

void FormulaView::on_glunary_clicked()
{
	applyexpr(newunary());
}

void FormulaView::on_glbinary_clicked()
{
	applyexpr(newbinary());
}
void FormulaView::on_additional_clicked()
{
	if(m_dv2!=NULL){
		if(m_dv2->isVisible())on_calbutt_clicked();
	}
	if(m_daughterview!=NULL){
		m_daughterview->setVisible(!m_daughterview->isVisible());
		if(m_daughterview->isVisible()){
			ui->treeWidget->clearSelection();
			ui->treeWidget->setVisible(false);
			ui->label->setVisible(false);
			ui->glbinary->setVisible(false);
			ui->glnumber->setVisible(false);
			ui->glunary->setVisible(false);
			ui->glvariable->setVisible(false);
		}else {
			ui->treeWidget->setVisible(true);
			ui->label->setVisible(true);
			ui->glbinary->setVisible(true);
			ui->glnumber->setVisible(true);
			ui->glunary->setVisible(true);
			ui->glvariable->setVisible(true);
		}
	}
}

void FormulaView::on_applybutton_clicked()
{
	Math_::Expr *expr=SelectedExpr();
			if(expr!=NULL){
				if(expr->Type()==MATH__EXPR_NUMBER){
					((Math_::Number*)expr)->SetValue(ui->doubleSpinBox->value());
				}
				if(expr->Type()==MATH__EXPR_VARIABLE){
					((Math_::Variable*)expr)->SetName(ui->nameedit->text());
				}
				if((expr->Type()==MATH__EXPR_UNARY)
						|(expr->Type()==MATH__EXPR_BINARY)){
					((Math_::Unary*)expr)->SetName(ui->nameedit->text());
				}
			}
}

void FormulaView::on_lineEdit_textChanged(const QString &arg1)
{
	m_formula->Set_Name(arg1);
}

const QString subtreequestion="This will replace expression's subtree. Continue?";


void FormulaView::on_pushButton_clicked()
{
	MsgBox(
				"Variables and operators are input as text identifiers\nHere is description, how to use them:\n\nUnary operators: sin, cos, sind, cosd(argument\n is in degrees), sqr, sqrt\n\nBinary operators: + - * / pow \n\nVariables can be either global (user defined) or be\nattached to soome(or all) datafile(s). Last ones\ncan be defined either automatically or \nby user. They can be viewed, when datafile is opened\nFor spectra axes you should also use variables\nattached to separate data events. They are written\nautomatically: #ND - Detector Number code(optional),\n#0, #1, #2 ... values from ADCs\nIf there's no variable matching input name\nzero is returned.\nYou can also use one formula as a variable in another one."
				);
}

void FormulaView::on_calbutt_clicked()
{
	if(m_daughterview->isVisible())on_additional_clicked();
	if(m_dv2!=NULL){
		m_dv2->setVisible(!m_dv2->isVisible());
		if(m_dv2->isVisible()){
			ui->treeWidget->clearSelection();
			ui->treeWidget->setVisible(false);
			ui->label->setVisible(false);
			ui->glbinary->setVisible(false);
			ui->glnumber->setVisible(false);
			ui->glunary->setVisible(false);
			ui->glvariable->setVisible(false);
		}else{
			ui->treeWidget->setVisible(true);
			ui->label->setVisible(true);
			ui->glbinary->setVisible(true);
			ui->glnumber->setVisible(true);
			ui->glunary->setVisible(true);
			ui->glvariable->setVisible(true);
		}
	}

}


void FormulaView::on_ParseButton_clicked()
{
	QString expr_str="";
	{
		Math_::Expr *cur=m_formula->Expression();
		if(cur!=NULL){
			expr_str=cur->ExprStr();
		}
	}
	ExpressionStringDialog *diag=new ExpressionStringDialog();
	diag->SetExprStr(expr_str);
	int res=diag->exec();
	if(res){
		expr_str=diag->ExprStr();
		Math_::Expr* res=Math_::ParseExprFromString(expr_str);
		if(res!=NULL)
			m_formula->Set_Expression(res);
	}
	delete diag;
}
