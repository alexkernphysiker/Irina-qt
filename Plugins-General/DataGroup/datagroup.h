#ifndef TEXTFILE_H
#define TEXTFILE_H
#include <SortLib.h>
#include <QList>

//Data format type constants
#define SOT_DataGroup 0x0301

enum DataGroupType{DGSumm, DGAverage};
class DataGroup:public SoDFReader{
	Q_OBJECT
public:
		DataGroup(SortProject *father);
		DataGroup(QDataStream &str,SortProject *father);
		virtual ~DataGroup();
		virtual void Save(QDataStream &str)override;
		virtual QString DisplayName()override;
		virtual DataEvent *NextEvent(QDataStream &datastr)override;
		virtual void *GetForm()override;
		int GetDFReaderCount();
		SObject* GetDFReader(int index);
		void RemDFReader(int index);
		void AddDFReader(SoTypeReg* factory);
		DataGroupType GetType();
		void SetType(DataGroupType type);
public slots:
		virtual void Read()override;
private slots:
		void AfterRead();
		void CallDataEventGot(DataEvent *);
		void setstatusmsg(QString);
		void errorreport(QString);
private:
		void __connect();
		void __disconnect();
		QList<SoDFReader*> m_list;
		DataGroupType m_dgtype;
};

#endif
