// this file is distributed under
// GPL v 3.0 license
#ifndef SORTPROJECTVIEW_H
#define SORTPROJECTVIEW_H
#include <QtCore/qglobal.h>

#if defined(SORTLIBGUI_LIBRARY)
#  define SORTLIBGUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define SORTLIBGUISHARED_EXPORT Q_DECL_IMPORT
#endif
#include <QMainWindow>
#include <QGroupBox>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "SortLib.h"

//these classes are implemented in this library but are not exported
namespace Ui {
	class SortProjectView;
	class DataFileView;
	class TblFuncView;
}
class myTreeWidget;
//======================================



// Display a text in message box
SORTLIBGUISHARED_EXPORT void MsgBox(QString txt);




/******************************************************************
								IRINA-QT MAIN WINDOW
 ******************************************************************/
class SORTLIBGUISHARED_EXPORT SortProjectView : public QMainWindow
{
	Q_OBJECT
public:
	explicit SortProjectView(SortProject *project, QWidget *parent = 0);// Constructor requires a SortProject instance
	~SortProjectView();
	SortProject *Project();// gets current project
	SObject *SelectedObj();// gets the object currently selected in the project tree
protected:
	void closeEvent(QCloseEvent *event);
private:
	Ui::SortProjectView *ui;
	SortProject *m_project;
	myTreeWidget *m_tree;
	bool m_busy;
	int top_;
	int height_;
private slots:
	void busy(bool val);
	void statusmsg(QString msg);
	void on_actionSave_activated();
	void on_actionSave_triggered();
	void TakeInfoView(QWidget *widg);
	void on_actionRecalculate_activated();
	void on_actionAdd_activated();
	void on_actionAdd_2_activated();
	void on_actionAdd_constant_activated();
	void on_actionAdd_spectrum_axis_activated();
	void on_actionRemove_selected_element_activated();
	void on_actionClear_project_activated();
	void on_actionWindow_activated();
	void on_actionLocus_activated();
	void on_actionX_Y_deltaY_activated();
	void on_actionAdd_DataFile_s_function_activated();
	void on_actionScatter_with_appendix_activated();
	void on_actionEnergy_calibration_at_SP2_activated();
	void on_actionOutput_settings_activated();
	void on_actionDisplay_About_message_triggered();
signals:
	void closing();
	void OutputSetup();// when user opens data output configuration
};




/*************************************************************
								DataFile view
 *************************************************************/
class SORTLIBGUISHARED_EXPORT DataFileView : public QGroupBox
{
	Q_OBJECT
public:
	explicit DataFileView(SoDFReader *obj, QWidget *parent = 0);
	~DataFileView();
	void AddTab(QString name, QWidget *control);// Adds custom tabs. Should be used for custom datafile readers
private slots:
	void changed(SObject *so);
	void busy(bool val);
	void on_addbut_clicked();
	void on_rembut_clicked();
	void on_evcnt_clicked();
	void on_lineEdit_textChanged(const QString &arg1);
	void on_pushButton_clicked();
	void on_sp1_clicked();
	void on_sp2_clicked();
private:
	Ui::DataFileView *ui;
	SoDFReader *m_reader;
};




/************************************************************
									Table Function View
 ************************************************************/
class SORTLIBGUISHARED_EXPORT TblFuncView : public QGroupBox
{
	Q_OBJECT
public:
	explicit TblFuncView(SoTblFunc *father, QWidget *parent = 0);
	~TblFuncView();
	void AddTab(QString caption, QWidget *widg);// Adds custom tabs. Should be used for derived objects
private slots:
	void on_lineEdit_textChanged(const QString &arg1);
	void refr();
	void on_pushButton_clicked();
private:
	Ui::TblFuncView *ui;
	SoTblFunc *master;
};




/***************************************************************
								Custom QTreeWidtesItems
 ***************************************************************/
class SORTLIBGUISHARED_EXPORT TypeRegTreeWidgetItem: public QTreeWidgetItem{
public:
	TypeRegTreeWidgetItem(SoTypeReg *typereg);
	~TypeRegTreeWidgetItem();
	SoTypeReg *TypeReg();
private:
	SoTypeReg *m_typereg;
};
class SORTLIBGUISHARED_EXPORT SObjectTreeWidgetItem: public QTreeWidgetItem{
public:
	SObjectTreeWidgetItem(SObject *so, myTreeWidget *w=NULL, bool recursive=true);
	~SObjectTreeWidgetItem();
	SObject *myobj();
	bool changed(SObject *so);
private:
	SObject *m_object;
};
class SORTLIBGUISHARED_EXPORT ExprTreeWidgetItem: public QTreeWidgetItem{
public:
	ExprTreeWidgetItem(Math_::Expr *expr, QTreeWidget *w=NULL);
	~ExprTreeWidgetItem();
	Math_::Expr *myobj();
	bool changed(Math_::Expr *expr);
private:
	Math_::Expr *m_expr;
};
class SORTLIBGUISHARED_EXPORT PeakTreeWidgetItem: public QTreeWidgetItem{
public:
	PeakTreeWidgetItem(Peak *peak, QTreeWidget *w=NULL);
	~PeakTreeWidgetItem();
	Peak *myobj();
private:
	Peak *m_peak;
};

#endif // SORTPROJECTVIEW_H
