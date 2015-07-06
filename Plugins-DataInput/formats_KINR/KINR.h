// this file is distributed under
// GPL v 3.0 license
#ifndef KINR_H
#define KINR_H
#include <SortLib.h>
#include <SortLibGui.h>

//Data format type constants
#define SOT_KINR 0x0101 //The same signature group for all KINR datafiles
#define SOT_KINR_D00V 0x010101
#define SOT_KINR_D00Z 0x020101
// If you use several data format libraries,make sure,
// that theese number sets(first parameters)
// do not intersect. Also check if every constructor calls
// AddType method with the same number

class SoD00V:public SoDFReader{
	public:
		// this constructor is used for adding new datafile reader to the project
		// Path - is the path to the file which is to be read
		SoD00V(QString Path, SortProject *father);

		// this constructor is used for reading the datafile reader from the project file
		// after the project is saved and opened again
		SoD00V(QDataStream &str,SortProject *father);


		// this virtual method declared in basic SoDFReader class
		// is called when a data event is read from datafile
		virtual DataEvent *NextEvent(QDataStream &datastr)override;
	protected:
		// this method reads datafile header
		virtual void ReadHeader(QDataStream &datastr)override;

		// this method checks if the data stored in the file are correct after
		// the reading is finished. The warning is displayed if something is wrong
		virtual void FinalDataCheck()override;
	private:
		QMutex readmutex;
		QList<int> m_monitors;
		uint m_events_header;
};

class SoD00Z:public SoDFReader{
	public:
		// this constructor is used for adding new datafile reader to the project
		// Path - is the path to the file which is to be read
		SoD00Z(QString Path, SortProject *father);

		// this constructor is used for reading the datafile reader from the project file
		// after the project is saved and opened again
		SoD00Z(QDataStream &str,SortProject *father);

		// this virtual method declared in basic SoDFReader class
		// is called when a data event is read from datafile
		virtual DataEvent *NextEvent(QDataStream &datastr)override;
	protected:
		// this method reads datafile header
		virtual void ReadHeader(QDataStream &datastr)override;

		// this method checks if the data stored in the file are correct after
		// the reading is finished. The warning is displayed if something is wrong
		virtual void FinalDataCheck()override;
	private:
		QMutex readmutex;
		QList<int> m_monitors;
		int recordwidth;
		int blocklength;
		int recsperblock;
		int numblocks;
		int lastblocklength;
		int blocktag;
		int count;
		int cnt;
		int blockcnt;
};


#endif // KINR_H
