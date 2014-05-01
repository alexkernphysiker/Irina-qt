#include "masterwindow.h"
#include "ui_masterwindow.h"
#include <Core/SortLib/sortlib_.h>
#include <SortLibGui.h>
#include <QFileDialog>
#include <generateSource/GenerateSource.h>
using namespace GenerateSource;

QString getConfigFilePath(){
	QDir appDir = QDir(qApp->applicationDirPath());
	return appDir.absoluteFilePath("Irina-qt-devtool.path.config");
}
QString getConfig(){
	QString res ="/";
	{
		QString p=getConfigFilePath();
		QFile file(p);
		file.open(QFile::ReadOnly);
		if(file.isOpen()){
			QTextStream str(&file);
			res="";
			while(!str.atEnd()){
				QString s="";
				str>>s;
				if(res!="")
					res=res+" "+s;
				else
					res=s;
			}
			file.close();
		}
	}
	return res;
}
void saveConfig(QString val){
	QFile file(getConfigFilePath());
	file.open(QFile::WriteOnly);
	if(file.isOpen()){
		QTextStream str(&file);
		str<<val;
		file.close();
	}
}


MasterWindow::MasterWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MasterWindow)
{
	ui->setupUi(this);
	setWindowTitle(aboutsortlibcaption()+" - Developers Tool");
	ui->path_line->setText(getConfig());
}

MasterWindow::~MasterWindow()
{
	saveConfig(ui->path_line->text());
	delete ui;
}

void MasterWindow::on_browse_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(NULL,
			aboutsortlibcaption(),ui->path_line->text(),
			QFileDialog::ShowDirsOnly	| QFileDialog::DontResolveSymlinks);
	if((dir!=NULL)&(dir!="")){
		ui->path_line->setText(dir);
	}
}
void GenProjectFile(QString projName, QString targetname, QDir projDir, QList<TranslationUnit*> contains, QStringList uifiles=QStringList()){
	QDir destdir = QDir(qApp->applicationDirPath());
	GeneratedFile projectfile(projName+".pro");
	projectfile.SetDir(projDir)->
			AddLine("#Irina-qt installation path")->
			AddLine(QString("DESTDIR = ")+destdir.path())->
			AddLine("DEPENDPATH += $$DESTDIR")->
			AddLine("win32: DLLDESTDIR = $$DESTDIR")->
			Add((new CppNested("CONFIG (debug, debug|release) ",true))->
				AddLine("OBJECTS_DIR=$$DESTDIR/debug")
				)->
			Add((new CppNested("else",true))->
				AddLine("OBJECTS_DIR=$$DESTDIR/release")
				)->
			AddLine("LIBS += -L$$DEPENDPATH -lSortLib")->
			//AddLine("LIBS += -L$$DEPENDPATH -lSortLibGui")->
			Add((new CppNested("unix: ",true))->
				AddLine("QMAKE_LFLAGS += -Wl,--rpath=$$PWD/$$DESTDIR")->
				AddLine("QMAKE_LFLAGS_RPATH=")
				)->
			AddLine("#Path to the headers (in linux can differ)")->
			AddLine("INCLUDEPATH +=$$DESTDIR/include/Irina-qt")->
			AddLine("INCLUDEPATH +=$$DESTDIR/include/MathLibs")->
			AddLine("#Standard C++11 if supported")->
			AddLine("QMAKE_CXXFLAGS+=-std=c++11")->
			AddLine("#Standard C++11 if not supported")->
			AddLine("#DEFINES += override=")->
			AddLine("#Configure project")->
			AddLine("QT+= core gui")->
			AddLine("CONFIG += plugin")->
			AddLine("TEMPLATE = lib")->
			AddLine(QString("TARGET   = $$qtLibraryTarget(")+targetname+")")->
			AddLine("SOURCES += \\");
	for(int i=0; i<contains.count();++i){
		projectfile.AddLine(contains.at(i)->SourceName()+" \\");
	}
    projectfile.AddLine(" ");
	projectfile.AddLine("HEADERS += \\");
	for(int i=0; i<contains.count();++i){
		projectfile.AddLine(contains.at(i)->HeaderName()+" \\");
	}
	projectfile.AddLine(" ");
	if(uifiles.count()>0){
		projectfile.AddLine("FORMS += \\");
		for(int i=0; i<uifiles.count();i++){
			projectfile.AddLine(uifiles.at(i)+" \\");
		}
		projectfile.AddLine(" ");
	}
	projectfile.Save();
}
void GenRegPlugin(TranslationUnit* registerClass, ClassUnit* myclass, QString category, QString targetname, QString typeids, IInstruction* addinstruction){
	QString loaderclassname=targetname+"_PluginLoaderInterface";
	ClassUnit* regclass=new ClassUnit(loaderclassname,"public QObject, public LoaderInterface");
	registerClass->AddInclude("<plugin_interface.h>",IncludeInHeader)->
			AddInstruction((new CppNested(QString("SObject*")+" myload(QDataStream &str,SortProject *father)",true))->
						   AddLine("return new "+myclass->Name()+"(str,father);"))->
			AddInstruction((new CppNested(QString("SObject*")+" myadd(SortProject *father, SObject*)",true))->
						   Add(addinstruction)
						)->
			AddUnit(regclass)->
			AddInstruction((new AbstractNested)->
						   AddLine("QT_BEGIN_NAMESPACE")->
						   AddLine(QString("Q_EXPORT_PLUGIN2(")+targetname+", "+loaderclassname+")")->
						   AddLine("QT_END_NAMESPACE")
						   );
	regclass->AddLine("Q_OBJECT")->AddLine("Q_INTERFACES(LoaderInterface)");
	regclass->Public()->Method("virtual","bool","Register"," ",
							   (new AbstractNested)->
							   AddLine(QString("return RegisterSoType(new SoTypeReg(\"")+
									   myclass->Name()+"/0.1\",\""+
									   myclass->Name()+"\","+
									   category+","+typeids+",&myload,&myadd));")
							)->Method("virtual","bool","Unregister"," ",
									  (new AbstractNested)->
									  AddLine("return UnregisterSoType("+typeids+");")
									  );
}
TranslationUnit* GenerateForm(TranslationUnit* mainunit, ClassUnit* viewedclass, QStringList *ui_output){
	QString viewerclassname=viewedclass->Name()+"View";
	ClassUnit* formclass=new ClassUnit(viewerclassname,"QGroupBox",pubmode);
	TranslationUnit* formcode=new TranslationUnit(viewerclassname.toLower());
	formcode->SetDir(mainunit->Dir());
	formcode->AddInclude("<QGroupBox>",IncludeInHeader)->
			AddInclude(QString("\"")+mainunit->HeaderName()+"\"",IncludeInHeader)->
			AddInclude(QString("\"ui_")+formcode->HeaderName()+"\"")->
			AddInstruction(
				(new CppNested("namespace Ui ",true))->AddLine(QString("class ")+viewerclassname+";")
				,true)->AddUnit(formclass);
	formclass->AddLine("Q_OBJECT");
	formclass->Public()->AddLine("explicit");
	formclass->Constructor(viewedclass->Name()+"* father","), ui(new Ui::"+viewerclassname,
						   new Instruction("ui->setupUi(this);m_father=father;")   )->
			Destructor(true,new Instruction("delete ui;"))->Private()->
			AddLine("Ui::"+viewerclassname+" *ui;")->
			AddLine(viewedclass->Name()+"* m_father;");
	mainunit->AddInclude(QString("\"")+formcode->HeaderName()+"\"");
	viewedclass->Method("virtual","void*","GetForm"," ",new Instruction(QString("return (void*)new "+viewerclassname+"(this);")));
	GeneratedFile uicode(formcode->Name()+".ui");
	uicode.SetDir(mainunit->Dir());
	uicode.AddLine("<?xml version='1.0'?>")->Add(
				(new XmlNested("ui","version=\"4.0\""))->
				AddLine(QString("<class>")+viewerclassname+"</class>")->
				Add((new XmlNested("widget","class=\"QGroupBox\" name=\""+viewerclassname+"\""))->
					Add((new XmlNested("property","name=\"geometry\""))->
						Add((new XmlNested("rect"))->
							AddLine("<x>0</x>")->AddLine("<y>0</y>")->
							AddLine("<width>320</width>")->AddLine("<height>350</height>")
							)
						)
					)
				);
	uicode.Save();
	*ui_output<<uicode.Name();
	return formcode;
}
void GenDataPlugin(QString projName, QDir projDir){
	QString loaderclassname=projName+"_register";
	QString targetname=projName.toLower();
	TranslationUnit mainProjClass(projName);
	ClassUnit* myclass=new ClassUnit(projName,"SoDFReader",pubmode);
	mainProjClass.SetDir(projDir)->AddInclude("<SortLib.h>",IncludeInHeader)->
			AddInclude("<QMutexLocker>")->
			AddDefine("MYTYPESIGN 0xff // Change it to your data format signature it should not exhist in other plugins",true)->
			AddUnit(myclass);
	myclass->AddLine("Q_OBJECT");
	myclass->Public()->Constructor("QString Path, SortProject *father","Path,father",new Instruction("AddType(MYTYPESIGN);"))->
			Constructor("QDataStream &str,SortProject *father","str,father",new Instruction("AddType(MYTYPESIGN);"))->
			Destructor(true,NULL)	->
			Method("virtual", "DataEvent*","NextEvent","QDataStream &datastr",
						(new AbstractNested())->
						AddLine("QMutexLocker locker(&readmutex);")->
						AddLine("DataEvent *res=new DataEvent(NADC());")->
						AddLine("//ToDo: provide code that reads event data from 'datastr'")->
						AddLine("//After reading stream must be seek at the beginning of the next event or at the end")->
						AddLine("return res;")
				)->Protected()->
			Method("virtual","void","ReadHeader","QDataStream &datastr",
				   (new AbstractNested())->
				   AddLine("QMutexLocker locker(&readmutex);")->
				   AddLine("//ToDo: provide code that reads header data from datastr")->
				   AddLine("//After reading the header stream must be seek at the begining of event data")
				)->
			Method("virtual","void","FinalDataCheck"," ",
				   (new AbstractNested())->
				   AddLine("QMutexLocker locker(&readmutex);")->
				   AddLine("SoDFReader::FinalDataCheck();")->
				   AddLine("//ToDo: provide code that checks data correctness after reading all data events is finished")->
				   AddLine("//Use Set_Warning() and Set_Error() to report possible errors")
				   )->Private()->AddLine("QMutex readmutex;");
	TranslationUnit registerClass(loaderclassname);
	registerClass.SetDir(projDir)->
			AddInclude("<QFileDialog>")->
			AddInclude(QString("\"")+projName+".h\"");
	GenRegPlugin(&registerClass, myclass,"SoCatData",targetname,"MYTYPESIGN, SOT_DFReader",(new AbstractNested)->
				 AddLine("QString str=QFileDialog::getOpenFileName(NULL,\"Choose file\",QDir::currentPath(),\"*.*\",NULL,	QFileDialog::DontResolveSymlinks);")->
				 AddLine("str=QDir::current().relativeFilePath(str);")->
				 AddLine("if((str!=NULL)&(str!=\"\"))")->
				 AddLine("return new "+myclass->Name()+"(str,father);")
				);
	GenProjectFile(projName,targetname,projDir, QList<TranslationUnit*>()<<&mainProjClass<<&registerClass);
}
void GenFuncPlugin(QString projName, QDir projDir){
	QString loaderclassname=projName+"_register";
	QString targetname=projName.toLower();
	TranslationUnit mainProjClass(projName);
	ClassUnit* myclass=new ClassUnit(projName,"SoFormula",pubmode);
	mainProjClass.SetDir(projDir)->AddInclude("<SortLib.h>",IncludeInHeader)->
			AddDefine("MYTYPESIGN 0xff // Change it to needed type signature it should not exhist in other plugins",true)->
			AddUnit(myclass);
	myclass->AddLine("Q_OBJECT");
	myclass->Public()->Constructor("SortProject *father","father",
								   (new AbstractNested)->AddLine("AddType(MYTYPESIGN);")->
								   AddLine("//ToDo: Custom initialization")
								   )->
			Constructor("QDataStream &str,SortProject *father","str,father",
					  (new AbstractNested)->AddLine("AddType(MYTYPESIGN);")->
						AddLine("//ToDo: read from str additional information")
						)->Destructor(true,NULL)->
			Method("virtual","void","Save","QDataStream &str",
				   (new AbstractNested)->
				   AddLine(myclass->ParentName()+"::Save(str);")->
				   AddLine("//ToDo: write additional data to str")
				   )->Method("virtual","QString","DisplayName"," ", new Instruction("return \"FUNC::MyFunction\"+Name();"));
	myclass->Method("virtual","double","Value","SoDFReader *fr, DataEvent *event",
					new Instruction("return 0; //ToDo: provide custom calculations"));

	QStringList uis;
	TranslationUnit* form=GenerateForm(&mainProjClass,myclass,&uis);
	TranslationUnit registerClass(loaderclassname);
	registerClass.SetDir(projDir)->
			AddInclude(QString("\"")+projName+".h\"");
	GenRegPlugin(&registerClass, myclass,"SoCatFormula",targetname,"MYTYPESIGN, SOT_Formula",
				 new Instruction("return new "+myclass->Name()+"(father);")
				);
	GenProjectFile(projName,targetname,projDir, QList<TranslationUnit*>()<<&mainProjClass<<&registerClass<<form,uis);
	delete form;
}
void GenFilterPlugin(QString projName, QDir projDir){
	QString loaderclassname=projName+"_register";
	QString targetname=projName.toLower();
	TranslationUnit mainProjClass(projName);
	ClassUnit* myclass=new ClassUnit(projName,"SoEFilter",pubmode);
	mainProjClass.SetDir(projDir)->AddInclude("<SortLib.h>",IncludeInHeader)->
			AddDefine("MYTYPESIGN 0xff // Change it to needed type signature it should not exhist in other plugins",true)->
			AddUnit(myclass);
	myclass->AddLine("Q_OBJECT");
	myclass->Public()->Constructor("SortProject *father","father",
								   (new AbstractNested)->AddLine("AddType(MYTYPESIGN);")->
								   AddLine("//ToDo: Custom initialization")
								   )->
			Constructor("QDataStream &str,SortProject *father","str,father",
					  (new AbstractNested)->AddLine("AddType(MYTYPESIGN);")->
						AddLine("//ToDo: read from str additional information")
						)->Destructor(true,NULL)->
			Method("virtual","void","Save","QDataStream &str",
				   (new AbstractNested)->
				   AddLine(myclass->ParentName()+"::Save(str);")->
				   AddLine("//ToDo: write additional data to str")
				   )->Method("virtual","QString","DisplayName"," ", new Instruction("return \"FILTER::MyFilter\"+Name();"));
	myclass->Method("virtual","bool","In","SoDFReader *fr, DataEvent *event",
					new Instruction("return true; //ToDo: provide custom calculations"));

	QStringList uis;
	TranslationUnit* form=GenerateForm(&mainProjClass,myclass,&uis);
	TranslationUnit registerClass(loaderclassname);
	registerClass.SetDir(projDir)->
			AddInclude(QString("\"")+projName+".h\"");
	GenRegPlugin(&registerClass, myclass,"SoCatFilter",targetname,"MYTYPESIGN, SOT_EFilter",
				 new Instruction("return new "+myclass->Name()+"(father);")
				);
	GenProjectFile(projName,targetname,projDir, QList<TranslationUnit*>()<<&mainProjClass<<&registerClass<<form,uis);
	delete form;
}

void GenOtherPlugin(QString projName, QDir projDir){
	QString loaderclassname=projName+"_register";
	QString targetname=projName.toLower();
	TranslationUnit mainProjClass(projName);
	ClassUnit* myclass=new ClassUnit(projName,"SObject",pubmode);
	mainProjClass.SetDir(projDir)->AddInclude("<SortLib.h>",IncludeInHeader)->
			AddDefine("MYTYPESIGN 0xff // Change it to needed type signature it should not exhist in other plugins",true)->
			AddUnit(myclass);
	myclass->AddLine("Q_OBJECT");
	myclass->Public()->Constructor("SortProject *father","father",
								   (new AbstractNested)->AddLine("AddType(MYTYPESIGN);")->
								   AddLine("//ToDo: Custom initialization")
								   )->
			Constructor("QDataStream &str,SortProject *father","str,father",
					  (new AbstractNested)->AddLine("AddType(MYTYPESIGN);")->
						AddLine("//ToDo: read from str additional information")
						)->Destructor(true,NULL)->
			Method("virtual","void","Save","QDataStream &str",
				   (new AbstractNested)->
				   AddLine(myclass->ParentName()+"::Save(str);")->
				   AddLine("//ToDo: write additional data to str")
				   )->Method("virtual","QString","DisplayName"," ", new Instruction("return \"MyObject\"+Name();"));
	myclass->AddLine("//===================");
	myclass->AddLine("//ToDo: add other methods here");
	myclass->AddLine("//===================");
	QStringList uis;
	TranslationUnit* form=GenerateForm(&mainProjClass,myclass,&uis);
	TranslationUnit registerClass(loaderclassname);
	registerClass.SetDir(projDir)->
			AddInclude(QString("\"")+projName+".h\"");
	GenRegPlugin(&registerClass, myclass,"SoCatOther",targetname,"MYTYPESIGN",
				 new Instruction("return new "+myclass->Name()+"(father);")
				);
	GenProjectFile(projName,targetname,projDir, QList<TranslationUnit*>()<<&mainProjClass<<&registerClass<<form,uis);
	delete form;
}

void GenOutPlugin(QString , QDir ){
}

void MasterWindow::on_create_clicked()
{
	if(ui->name->text()==""){MsgBox("Please input the name of your project");return;}
	if(ui->path_line->text()==""){MsgBox("Please input the path of your project's destination");return;}
	QString projName=ui->name->text();
	QDir projDir(ui->path_line->text());
	if(projDir.mkdir(projName)){
		QString projPath = projDir.absoluteFilePath(projName);
		projDir=QDir(projPath);
		if(ui->choose_data->isChecked()){
			GenDataPlugin(projName,projDir);
		}else{
			if(ui->choose_func->isChecked()){
				GenFuncPlugin(projName,projDir);
			}else{
				if(ui->choose_out->isChecked()){
					GenOutPlugin(projName,projDir);
				}else{
					if(ui->choose_other->isChecked()){
						GenOtherPlugin(projName,projDir);
					}else{
						if(ui->choose_filter->isChecked()){
							GenFilterPlugin(projName,projDir);
						}
					}
				}
			}
		}
		MsgBox("Please check project files at the specified location");
		close();
	}else{
		MsgBox("Cannot create project directory");
	}
}

void MasterWindow::on_choose_data_toggled(bool checked)
{
	if(checked)ui->name->setText("MyDataFormat");
}

void MasterWindow::on_choose_func_toggled(bool checked)
{
	if(checked)ui->name->setText("MyFunction");
}

void MasterWindow::on_choose_filter_toggled(bool checked)
{
	if(checked)ui->name->setText("MyEventFilter");
}

void MasterWindow::on_choose_out_toggled(bool checked)
{
	if(checked)ui->name->setText("MyDataOutputExtension");
}

void MasterWindow::on_choose_other_toggled(bool checked)
{
	if(checked)ui->name->setText("MyObject");
}
