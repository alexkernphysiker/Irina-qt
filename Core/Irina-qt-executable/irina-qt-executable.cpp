// this file is distributed under
// GPL v 3.0 license
#include <QObject>
#include <QApplication>
#include <QFileDialog>
#include <QString>
#include <QList>
#include <QPluginLoader>
// Irina-qt core
#include <Core/SortLib/sortlib_.h>
#include <SortLibGui.h>
// plugin interface class declaration
#include <plugin_interface.h>
#include <plugin_output_interface.h>
//this executable classes
#include <irinaqterrorlog.h>
#include "outputextensionselector.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//Ask user about working directory path
	QString dir = QFileDialog::getExistingDirectory(NULL,
			aboutsortlibcaption()+" ; "+PLUGIN_INTERFACE+" ; "+OUTPUT_PLUGIN_INTERFACE,
			#ifdef WINBUILD
			"/",
			#else
			"~/",
			#endif
			QFileDialog::ShowDirsOnly	| QFileDialog::DontResolveSymlinks);
	if((dir!=NULL)&(dir!="")){

		//Search plugins
		QStringList plotters;
		QList<QObject*> plotterplugins;
		QList<QObject*> obj_plugins;
		QDir pluginsDir = QDir(qApp->applicationDirPath());
		foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
			QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
			QObject *plugin = loader.instance();
			if (plugin) {
				LoaderInterface *pluginloader = qobject_cast<LoaderInterface*>(plugin);
				if (pluginloader) {
					obj_plugins<<plugin;
				}else{
					// store the names of all plotter extensions available
					OutputPluginLoaderInterface *pluginloader = qobject_cast<OutputPluginLoaderInterface*>(plugin);
					if (pluginloader) {
						plotters<<fileName;
						plotterplugins<<plugin;
					}else{
						delete plugin;
					}
				}
			}
		}
		// Select plotter extension
		QObject* plotter=NULL;
		IDataOutput* dataoutput=NULL;
		if(plotters.length()==0){
			MsgBox(QString("There are no data output extensions. You will not be able to export the results of calculations.\nRequired plugin interface '")+OUTPUT_PLUGIN_INTERFACE+"'");
		}else{
			if(plotters.length()>1){
				OutputExtensionSelector *dial=new OutputExtensionSelector(plotters);
				dial->exec();
				int ind=dial->SelectedIndex();
				delete dial;
				for(int i=0; i<plotters.length();i++){
					QObject *plugin=plotterplugins.at(i);
					if(i==ind){
						OutputPluginLoaderInterface *pluginloader = qobject_cast<OutputPluginLoaderInterface*>(plugin);
						if (pluginloader) {
							plotter=plugin;
							dataoutput=pluginloader->DataOutput();
						}
					}else{
						delete plugin;
					}
				}
			}else{
				QObject *plugin=plotterplugins.at(0);
				OutputPluginLoaderInterface *pluginloader = qobject_cast<OutputPluginLoaderInterface*>(plugin);
				if (pluginloader) {
					plotter=plugin;
					dataoutput=pluginloader->DataOutput();
				}
			}
			plotterplugins.clear();
			plotters.clear();
		}

		//register plugins
		int wrong=0;
		for(int i=0; i<obj_plugins.length();i++){
			QObject *plugin=obj_plugins.at(i);
			LoaderInterface *pluginloader = qobject_cast<LoaderInterface*>(plugin);
			if (pluginloader) {
				if(!pluginloader->Register()){
					wrong++;
				}
			}else{
				obj_plugins.removeAt(i);
				delete plugin;
				i--;
			}
		}
		if(wrong>0){
			MsgBox(QString::number(wrong)+" plugin(s) could not be completely registered.\nInterface version: '"+PLUGIN_INTERFACE+"'");
		}

		//change current directory into it and create project (SortLib) using project file located there if exhists
		QDir::setCurrent(dir);
		SortProject project("data.sorting");
		if(dataoutput!=NULL)
			project.SetDataOutput(dataoutput);
		//Create main form instance (SortLibGui)
		SortProjectView *form=new SortProjectView(&project);
		form->setWindowTitle(aboutsortlibcaption()+" ; "+PLUGIN_INTERFACE+" ; "+OUTPUT_PLUGIN_INTERFACE);
		//Create an instance of error logging form (ErrorLog)
		ErrorLog *logform= ErrorLog::GetInstance();
		//connect error log form slots to signals that manage it's behaviour
		QObject::connect(&project,SIGNAL(error(QString)),logform,SLOT(error(QString)));
		QObject::connect(&project,SIGNAL(gui_close()),logform,SLOT(close()));
		//load project, show main window and run application
		project.Load();
		form->show();
		int res= a.exec();

		//unregister plugins
		wrong = 0;
		for(int i=0; i<obj_plugins.length();i++){
			QObject *plugin=obj_plugins.at(i);
			LoaderInterface *pluginloader = qobject_cast<LoaderInterface*>(plugin);
			if (pluginloader) {
				if(!pluginloader->Unregister()){
					wrong++;
				}
			}
			delete plugin;
		}
		if(wrong>0){MsgBox(QString::number(wrong)+" plugin(s) had problems with unregistering.\nInterface version: '"+PLUGIN_INTERFACE+"'");}
		delete plotter;
		return res;
	}else return 0;
}
