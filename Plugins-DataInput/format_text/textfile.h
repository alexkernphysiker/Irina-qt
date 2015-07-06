// this file is distributed under
// GPL v 3.0 license
#ifndef TEXTFILE_H
#define TEXTFILE_H
#include <SortLib.h>
#include <SortLibGui.h>

//Data format type constants
#define SOT_TEXT 0x0201
// If you use several data format libraries,make sure,
// that theese number sets(first parameters)
// do not intersect. Also check if every constructor calls
// AddType method with the same number




//
// This class implements simple datafile reader for Irina-qt
// that reads multicolumn text files interpreting each line as a data event
//
class TxtData:public SoDFReader{
	public:
		// this constructor is used for adding new datafile reader to the project
		// Path - is the path to the file which is to be read
		TxtData(QString Path, SortProject *father);

		// this constructor is used for reading the datafile reader from the project file
		// after the project is saved and opened again
		TxtData(QDataStream &str,SortProject *father);

		// this virtual method declared in basic SoDFReader class
		// is called when a data event is read from datafile
		virtual DataEvent *NextEvent(QDataStream &datastr)override;
};

#endif
