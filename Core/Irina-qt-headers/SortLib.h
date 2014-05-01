#ifndef SORTPROJECT_H
#define SORTPROJECT_H
#include <QtCore/qglobal.h>

#if defined(SORTLIB_LIBRARY)
#  define SORTLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SORTLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QtCore>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QMutex>



/*********************************************
 * Built-in SObject derivatives type signatures
 *********************************************/
#define SOT_DFReader 0x01							// base datafile reader (abstract class)
#define SOT_ECounter 0x02							// simple event counter
#define SOT_EFilter 0x04								// simple event filter (filters by ND value)
#define SOT_Value 0x08									// Global variable
#define SOT_Formula 0x10								// Function
#define SOT_Dependency 0x20						// Plot data
#define SOT_GroupDF 0x40							// Constant function
#define SOT_Binary 0x03								// Binary operator
#define SOT_Unary 0x05								// Unary operator
// 2nd level sort object types masks
#define SOT_SPAxis 0x0110							// Spectrum axis
#define SOT_ECSP1 0x0102							// 1-d spectrum
#define SOT_ECSP2 0x0202							// 2-d spectrum
#define SOT_EFWin 0x0104								// sp1 window
#define SOT_EFLoc 0x0204								// sp2 locus
#define SOT_SoTblFunc 0x0105							// Table function
#define SOT_DSewed 0x0120							// extended plot data
#define SOT_TblTbl 0x0103								// Simple interpolated binary operator
//3rd level
#define SOT_SPEnergy 0x010110					// Spectrum axis with dE-E calibration
#define SOT_SoTblFuncTxt 0x030105					// Table function loaded from text file
#define SOT_SP1Norm 0x020105						// Normalized 1-d spectrum (table func)
//4th level - the last one
#define SOT_SP1Norm_CoefErr 0x01020105		// Normalized 1-d spectrum with known error of norm. coefficient (table func)
/************************************************
	the sets of types implemented in plugins
	must NOT intersect with this set and with each other
 ************************************************/








/*******************************************
 Interfaces for plugins integration into SortLib
 Additional SObject derived types
 *******************************************/
class SObject; // Base Data analysing object type
class SortProject; // Class which corresponds to Irina-qt project
class SoTypeReg;// SObject type register item. Must be created by Irina-qt plugin which implements new SObject type
SORTLIBSHARED_EXPORT bool RegisterSoType(SoTypeReg* type_register_item);// registers additional type of SObject
SORTLIBSHARED_EXPORT bool UnregisterSoType(uint type_signature);// unregisters additional type of SObject
SORTLIBSHARED_EXPORT bool UnregisterSoType(uint sig_prefix, uint parrentsig);

typedef SObject* (*SOTypeReader)(QDataStream &stream, SortProject *parentproj);// reads SObject of the additional type
typedef SObject* (*SOTypeCreator)(SortProject *parentproj, SObject* selectedobject/*object selected in GUI*/);//creates SObject of the additional type
enum SoTypeCategory{SoCatData,SoCatFilter,SoCatFormula,SoCatOther};// corresponds to menu where object's item is placed

class SORTLIBSHARED_EXPORT SoTypeReg{
public:
	SoTypeReg(QString about_, QString caption_,SoTypeCategory cat_,uint addedsig,uint basesig, SOTypeReader read_, SOTypeCreator create_);
	SoTypeReg(QString about_, QString caption_,SoTypeCategory cat_,uint sig, SOTypeReader read_, SOTypeCreator create_);
	~SoTypeReg();
	uint Signature();// type signature
	QString About();// about message (is displayed in general "about" information)
	QString Caption();// the caption of adding menu item
	SoTypeCategory Category();// corresponds to menu where the item is added
	SObject* readInstance(QDataStream &str, SortProject *father);// reads an instance of object from stream
	SObject* createInstance(SortProject *parentproj, SObject* selectedobject);// creates a new instance of object
private:
	QString m_about;
	QString m_caption;
	uint m_sig;
	SoTypeCategory m_cat;
	SOTypeReader m_read;
	SOTypeCreator m_create;
};








/*******************************************
 Interfaces for plugins integration into SortLib
 Data output extensions
 *******************************************/
class DataEvent;// Used for data transfer between Irina-qt core and data output extension
class SORTLIBSHARED_EXPORT IDataOutput: public QObject{
	Q_OBJECT
public:
	virtual QString About()=0;//about message
public slots:
	// these slots are used for output data plotting
	virtual void sl_out_1d(QString)=0;// starting of 1-d distribution data transfer
	virtual void sl_out_2d(QString)=0;// starting of 2-d distribution data transfer
	virtual void sl_out_dataitem(DataEvent*)=0;// transfering a piece of data
	virtual void sl_out_end()=0;// data transfer is finished
	virtual void sl_out_config()=0;// open data output configuration dialog (optional)
	virtual void sl_out_gui_close()=0;// close all open data output windows (before closing Irina-qt)
signals:
	void error(QString);// report about errors
};
/*		How data is transfered to data output extension:
 *
 *		Plotting 1-d data:
 *		sl_out_1d(<distribution name>);
 *		sl_out_dataitem(DataEvent<x,y,dx,dy>);// first data point
 *		sl_out_dataitem(DataEvent<x,y,dx,dy>);// second data point
 *		...........
 *		sl_out_dataitem(DataEvent<x,y,dx,dy>);// last data point
 *		sl_out_end();// end data transfer
 *
 *		Plotting 2-d data:
 *		sl_out_2d(<distribution name>);
 *		sl_out_dataitem(DataEvent<x_min, x_channel_width,x_max>);
 *		sl_out_dataitem(DataEvent<y_min, y_channel_width,y_max>);
 *		sl_out_dataitem(DataEvent<count>);// start first row
 *		...........
 *		sl_out_dataitem(DataEvent<count>);
 *		sl_out_end();// end first row
 *		sl_out_dataitem(DataEvent<count>);// start second row
 *		...........
 *		sl_out_dataitem(DataEvent<count>);
 *		sl_out_end();// end second row
 *		...........
 *		sl_out_dataitem(DataEvent<count>);// start last row
 *		...........
 *		sl_out_dataitem(DataEvent<count>);
 *		sl_out_end();// end last row
 *		sl_out_end();// end data transfer
 */










/********************************************
						Irina-qt project
 ********************************************/
class SoDFReader; // defines datafile. Each datafile corresponds to separated measurement and can have it's own variables
class DataEvent; // defines data event. Each event has it's own variables that correspond to measured magnitudes
class SORTLIBSHARED_EXPORT SortProject: public QObject {
	Q_OBJECT
public:
	SortProject(QString path, bool replace=false);// constructor requires the path to project file
	void Load();//reads project file and creates sort objects
	void Save();//saves project into file (path should be given in constructor)
	void SetDataOutput(IDataOutput* out); // registers data output extension
	QString DataOutput(); // Obtains currently registered data output extension
	~SortProject();
	SObject *at(int N);//get sort object at given index
	int Count();//sort objects count
	int Number(SObject *obj);//gets number of given sort object
	void Add(SObject *obj);//adds new sort object (this one in constructor should get this SortProject's adress)
	void Remove(int N);//removes sort object from project and delets it (sort object deletes dependent objects itself)
	void Remove(SObject *so);//removes sort object from project and delets it
	void Insert(SObject *obj,int N=0);//insert sort object at given position
	void Clear();//removes all sort objects
	SObject *Find(uint type, QString name, Qt::CaseSensitivity sence=Qt::CaseSensitive, bool just=false);// Finds object of special type by name. If just=false then searches also derived objects
	bool GetVar(QString name,double *val, SoDFReader *dr, DataEvent *event);// gets variable or function value by name.(thread safe)
	double GetUnary(QString name, double x, SoDFReader *dr, DataEvent *event); // calculates the value of unary operator accessed by the name (thread safe)
	double GetBinary(QString name, double x, double y, SoDFReader *dr, DataEvent *event); // calculates the value of binary operator accessed by the name (thread safe)
	QString Path();//project file path
	void Refresh();//re-read all datafiles and rebuild all spectra
	bool IsBusy(); // returns true is reading datafiles is running
protected slots:
	void pr_changed(SObject *so);//is called when some object was added or deleted. Checks if signals should be sent and sends them
	void obj_changed(SObject *so);//some object has been changed
	void catcherror(QString msg);//for error output adds time and sends signal further
	void sl_out_1d(QString);// signals about starting of 1-d distribution plotting(passes from exported SObject to IDataOutput slot)
	void sl_out_2d(QString);// signals about starting of 2-d distribution plotting(passes from exported SObject to IDataOutput slot)
	void sl_out_dataitem(DataEvent*);// transfering a piece of data for plotting(passes from exported SObject to IDataOutput slot)
	void sl_out_end();// data terminating signal (passes from exported SObject to IDataOutput slot)
public slots:
	void sl_out_config();// open data output configuration dialog signal (passes from SortLibGui to IDataOutput slot)
	void sl_gui_close();// for closing all connected windows before terminating (passes from SortLibGui to plugins which require)
private slots:
	void refr();//is called when needs to be refreshed
	void setbusy(bool val);// is called when datafile reading starts or stops
	void setstatusmsg(QString msg);// is called when program is busy for reporting in status message
signals:
	void error(QString);//for error output
	void project_changed(SObject*);// for refreshing project tree in SortLibGui
	void object_changed(SObject*);//for refreshing sort objects' properties in SortLibGui
	void refreshed();// for notifying when is refreshed
	void variable_getting(QString name, double *val, SoDFReader *dr, DataEvent *event);//is called when a variable is getting it's value. connected slots can distort value got. it's usefull for example for numeric derivative calculations
	void busy(bool);// indicates the changes of "busy" state
	void statusmsg(QString);// indicates status message change
	void out_1d(QString);//(for IDataOutput slot)
	void out_2d(QString);//(for IDataOutput slot)
	void out_dataitem(DataEvent*);//(for IDataOutput slot)
	void out_end();//(for IDataOutput slot)
	void out_config();//(for IDataOutput slot)
	void gui_close();// for closing all connected windows before terminating
private:
	QList<SoDFReader*> lst;
	char m_chcnt;
	QString m_path;
	SObject **m_data;
	int m_count;
	bool m_sendsignals;//flag, that is used to stop sending signals while creating or disposing
	bool isbusy;
	bool blockerrors;
	uint lastfindtype;
	QString lastfindname;
	SObject* lastfindobj;
	QString m_dataoutmsg;
};
/*************************************
 *  reading and writing the project file
 *  these functions may be used in classes
 *  derived from SObject
 *************************************/
SORTLIBSHARED_EXPORT QString readstr(QDataStream &str);// reading ASCIIZ string from binary file
SORTLIBSHARED_EXPORT void writestr(QString s, QDataStream &str);// writting ASCIIZ string to binary file
SORTLIBSHARED_EXPORT SObject *ReadObj(QDataStream &str, SortProject *father);// reading SObject (or derived) from file (type is autodetected)









/**************************************************
					Base sort object class
**************************************************/
class SORTLIBSHARED_EXPORT SObject:public QObject{
	Q_OBJECT
protected://constructors are protected because no SObject instances are used. You should use it just as a parent class
	SObject(SortProject *father);//for adding
	SObject(QDataStream &str, SortProject *father);//for reading from project file stream
public:
	virtual ~SObject();
	virtual void Save(QDataStream &str);//saving to project file stream
	virtual void LoadCache(QDataStream &str);// load cached calculation results
	virtual void SaveCache(QDataStream &str);// save calculation results to cache
	SortProject *Owner();//get SortProject class, which this object belongs to
	bool Is(uint type);//check if belongs to given type or derived one
	bool IsJust(uint type);//check if belongs to given type
	void Set_Name(QString name);//set custom name, that's given by user
	QString Name();//get custom name
	virtual QString DisplayName();//get's name which is displayed in project tree
	virtual void BeforeDelete();//method, that's called before an object is deleted for deleting dependencies from Sort Project
	virtual void *GetForm();//override this method if you want your object to be displayed customly in the main Irina-qt window
	virtual SObject *DisplParrent();//override this method if you want the object to be displayed not in the root of project tree but as a subitem of other object this one depends on
signals:
	void changed(SObject*); //for notifying if has been changed
	void error(QString msg);//error notifying
	void out_1d(QString);//for data output
	void out_2d(QString);//for data output
	void out_dataitem(DataEvent*);//for data output
	void out_end();//for data output
	void busy(bool);// change Owner() busy state
	void deleting(); // is called before deleting is connected to dependent objects
	void deleting(SObject*); // is called before deleting is connected to dependent objects
protected slots:
	void remove(); // occurs deleting when some object current one is depending on is deleted
protected:
	uint SType();// get this object's type signature (used in Is and IsJust methods)
	void AddType(uchar subtype);//is used in EACH constructor to add current type prefix to parrents type signature
private:
	uint m_type;
	SortProject *m_master;
	QString m_name;
};








/********************************************************************
									DataFile reading
 ********************************************************************/
class SoDFReader;// DataFile reader class (abstract)
enum DFState{DF_OK,DF_Warning,DF_Error};// datafile state. Is actual after attempt to read
class SORTLIBSHARED_EXPORT  DataEvent{ // Single data event
public:
	DataEvent(int N);//constructor requires ADC count
	virtual ~DataEvent();
	void Freeze();//is called after creating by SoDFReader before giving it to other objects. Blocks Set_* methods
	virtual void Set_ADC(int N, double val);//sets value for ADC at given number
	virtual double ADC(int N);//gets value for ADC at given number
	virtual void Set_ND(int val);//set Detector Number code (if used)
	virtual int ND();//get Detector Number code (if used)
	bool Is_Valid();//checks if this event is valid
	void Set_Invalid();//mark event as invalid
	bool GetVar(QString name, double *val);//get variable value by name (Should be thread safe)
private:
	int m_nadc;	double *m_adc;
	int m_nd;	bool m_valid;bool m_freezed;
	QMutex m_mutex;
};
class SORTLIBSHARED_EXPORT ReaderAsync:public QObject{///provides multithreading functions for data reading
	Q_OBJECT
protected:
	ReaderAsync(SoDFReader *father);
public:
	ReaderAsync(QString m_path, SoDFReader *father); // constructor requires DataFile reader that creates this object
	virtual ~ReaderAsync();
	virtual void Open();//Opens file and checks if there're errors
	void Start();// Run file reading process in another thread
	bool Error();/// Error flag
	QString ErrMsg();// Error message if there's an error
	SoDFReader *Owner(); // Parent DataFile reader
	QDataStream *Stream(); // Binary data stream which corresponds to the data source
signals:
	void FileOpened(QDataStream *str);//is sent when data reading begins (objects, that analyse these data, should clear old ones)
	void DataEventGot(DataEvent *);//is sent for each data event got from the file
	void DataReadingFinnished();//is sent when reading is finnished (analysing objects should refresh the results)
	void statusmsg(QString);
	void finnished();
protected slots:
	virtual void process();
protected:
	SoDFReader *master;
private:
	QFile *file;	QDataStream *datastr;
	bool valid;	QString msg;
};
class SORTLIBSHARED_EXPORT SoDFReader:public SObject{
	Q_OBJECT
protected:
	SoDFReader(QString Path, SortProject *father);
public:
	SoDFReader(QDataStream &str,SortProject *father);
	virtual ~SoDFReader();
	virtual void BeforeDelete()override;
	virtual void LoadCache(QDataStream &str)override;
	virtual void SaveCache(QDataStream &str)override;
	QString Path();//gets datafile path
	QString Description();//gets description (filled from datafile. depends on data format implementation)
	virtual QString DisplayName()override;
	virtual void Save(QDataStream &str)override;
	DFState State();//get datafile state (if there are warnings or errors occured while reading)
	int NADC();// ADC count
	QStringList ConstantNames();//get list of constant names
	double Constant(QString name);//get constant value by name (constants are filled while reading datafile.depends on data format implementation)
	double Variable(QString name);//get constant value by name (constants are filled while reading datafile.depends on data format implementation)
	QString VarName(int N);//get variable name by number (variables are filled either by user or by objects that analyse the data read from this file)
	void RemVar(int N);//remove variable at given number
	int VarCnt();//variables count
	void ApplyVariable(QString name, double val);//set value to a variable by name. If there's no such variable, adds it
	bool GetVar(QString name, double *val);//get variable/constant value by name. returns true if found (Should be thread safe)
	virtual DataEvent *NextEvent(QDataStream &datastr)=0;// gets next event from datafile stream.
	virtual bool CanReadAsync();//Override this and return false in non-async data readers
public slots:
	virtual void Read();//read data from file and send events to dependent objects, which analyse them. Overriding is NOT recomended. Be carefull if it's needed.
signals:
	void ReadingStarted();//is sent when data reading begins (objects, that analyse, should delete old data)
	void DataEventGot(DataEvent *);//is sent for each data event got from the file
	void DataReadingFinnished();//is sent when reading is finnished (analysing objects should refresh results added as variables here)
	void statusmsg(QString);
protected:
	void Set_OK();//sets state to OK
	void Set_Warning();//sets state to Warning
	void Set_Error();//sets state to Error.
	void Set_NADC(int val);//sets ADC count
	void Set_description(QString descr);//Sets description
	void ApplyConstant(QString name, double val);//Adds or sets constant by name
	virtual void ReadHeader(QDataStream &datastr);//reads file header. Should be overriden for custom data format implementation
	virtual void DataEventIsGot(DataEvent *Event);//
	virtual void FinalDataCheck();//is called when reading was finnished. Should be overriden for custom data format implementation
	virtual ReaderAsync *_getReaderAsync(QString m_path, SoDFReader *father);// you can inherit this method and return other ReaderAsync, which will for example provide MonteCarlo simulations instead of reading file
	double eventsread();// how many events have been read successfully
	double eventsbroken(); // how many events are broken
	void reg_event(bool ok);
	void clear_ev_cnt();
	void clear_descr();
	void clear_constants();
protected slots:
	void ReportFinished();
private slots: //Slots for multithreading they are connected to the signals of ReaderAsync and pass them further to analyzing objects
	void onFileOpened(QDataStream *str);
	void GetDataEvent(DataEvent *event);
	void setstatusmsg(QString msg);
private:
	QMutex m_mutex;
	static QString eventsfromfile;
	static QString brokeneventsfromfile;
	DFState m_state;
	QString m_path;
	QString m_descr;
	int m_nadc;
	double m_n_events;
	double m_br_events;
	class LocalVariable{//for constants and variables
	public:
		LocalVariable(QString name, double value);
		QString Name();
		double Value();
	private:
		QString m_name;
		double m_value;
	};
	QList<LocalVariable> m_constants;
	QList<LocalVariable> m_variables;
	//ToDo: add variables
};






/****************************************************
							Event Filters
 ****************************************************/
class SORTLIBSHARED_EXPORT SoEFilter:public SObject{// Simple filter filters by ND
	Q_OBJECT
public:
	SoEFilter(SortProject *father);
	SoEFilter(QDataStream &str, SortProject *father);
	virtual ~SoEFilter();
	virtual void BeforeDelete()override;
	virtual void Save(QDataStream &str)override;
	virtual bool In(SoDFReader *, DataEvent *event);//check if this event is accepted by filter
	virtual QString DisplayName()override;
	int ND();//get ND value
	void SetND(int val);//set ND value
private:
	int m_nd;
};
class SoSPAxis;
class SORTLIBSHARED_EXPORT SoEFWin:public SoEFilter{// SP1 Window
	Q_OBJECT
public:
	SoEFWin(SoSPAxis *axis);//spectrum axis configuration is given as a constructor parameter and isn't changed any more
	SoEFWin(QDataStream &str,SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SoEFWin();
	virtual QString DisplayName()override;
	virtual bool In(SoDFReader *, DataEvent *event)override;// though it's inherited from SoEFilter it doesn't take ND value into account
	SoSPAxis *Axis();//get spectrum axis which this window belongs to
	bool In(int ch);//check if given channel number is accepted by this window
	void SetIn(int ch,bool val);//set if given channel number is accepted by this window
	virtual SObject *DisplParrent()override;
private:
	SoSPAxis *m_axis;
	bool *m_data;
private slots:
	void AxisChanged();//is connected to spectrum axis's changed signal
};
class SORTLIBSHARED_EXPORT SoEFLoc: public SoEFilter{// SP2 Locus
	Q_OBJECT
public:
	SoEFLoc(SortProject *father);
	SoEFLoc(QDataStream &str,SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SoEFLoc();
	virtual QString DisplayName()override;
	virtual bool In(SoDFReader *, DataEvent *event)override;// though it's inherited from SoEFilter it doesn't take ND value into account
	//get and set axes
	SoSPAxis *XAxis();
	void SetXAxis(SoSPAxis *ax);
	SoSPAxis *YAxis();
	void SetYAxis(SoSPAxis *ax);
	//get and set if given point (x,y) on corresponding 2D spectrum is accepted with this filter
	bool In(int x, int y);
	void SetIn(int x, int y, bool val);
private:
	SoSPAxis *m_xax;
	SoSPAxis *m_yax;
	bool *m_data;
private slots:
	void AxisChanged();
};






/****************************************************
							Event Counters
 ****************************************************/
class SORTLIBSHARED_EXPORT SoECounter:public SObject{// Simple event counter
	Q_OBJECT
public:
	SoECounter(SoDFReader *source);//takes datafile as constructor parameter and doesn't change it
	SoECounter(QDataStream &str, SortProject *father);
	virtual ~SoECounter();
	virtual void Save(QDataStream &str)override;
	virtual void LoadCache(QDataStream &str)override;
	virtual void SaveCache(QDataStream &str)override;
	SoDFReader *Source();//gets datafile instance it belongs to
	int EventsCount();//get accepted events count
	SoEFilter *Filter(int N);//get one of connected event filters by number
	void AddFilter(SoEFilter *filter);//connect an event filter
	int FilterInd(SoEFilter *filter);//get's filter's index if it's connected here -1 means not connected
	void DelFilterAt(int N);//disconnects filter by index
	int FilterCnt();//connected filters count
	virtual QString DisplayName();
	virtual SObject *DisplParrent();
protected:
	virtual void DEInit();//is called before datafile reading
	virtual bool DEAccepted(DataEvent *event);//is called for checking if data event is accepted by all filters and
	virtual void DEFinal();//is called after datafile reading
signals:
	void Refreshed(SoECounter *);//notifies that datafile reading has been finished
private:
	void Connect_to();//is called when datafile is set (in constructor) connects it's signals for these slots for accepting data events read
	SoDFReader *m_reader;
	int m_count;
	uint m_filt_cnt;
	SoEFilter **m_filters;
private slots:
	void status(QString);
	void ResetData();// is called when datasource starts reading to reset all previous data
	void DataEventAccepted(DataEvent *event);// analyses single data event coming from data source
	void AllDataAccepted();// is called when all data has been analysed for refreshing the data displayed
};
class Peak;
class SORTLIBSHARED_EXPORT SoECSP1:public SoECounter{// 1-d spectrum
	Q_OBJECT
public:
	SoECSP1(SoDFReader *reader);
	SoECSP1(QDataStream &str,SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual void LoadCache(QDataStream &str)override;
	virtual void SaveCache(QDataStream &str)override;
	virtual ~SoECSP1();
	virtual QString DisplayName()override;
	void SetAxis(SoSPAxis *axis);// Sets the axis that will be used for this spectrum building
	SoSPAxis *Axis();// gets currently set axis
	int ChCnt(int channel); // gets events count in the channel
	int PeakCnt(); // gets the count of peaks placed on this spectrum
	void AddPeak(QString name); // adds new peak on this spectrum
	Peak *getPeak(int N); // gets peak by index
	void RemPeak(Peak *peak); // removes peak by index
	void Export(); // Exports data to currently used data output extension
protected:
	virtual void DEInit()override;
	virtual bool DEAccepted(DataEvent *event)override;
	virtual void DEFinal()override;
private:
	SoSPAxis *m_axis;
	uint *m_data;
	QList<Peak*> m_peaks;
private slots:
	void AxisChanged();
};
class SORTLIBSHARED_EXPORT Peak:public QObject{// Peak placed on 1-d spectrum
	Q_OBJECT
public:
	Peak(QString name, SoECSP1 *sp1);// constructor requires sp1, which the peak is connected to
	Peak(QDataStream &str,SoECSP1 *sp1);
	void Save(QDataStream &str);
	virtual ~Peak();
	void SetM1(int m);// sets left marker position
	void SetM2(int m);// sets right marker position
	int M1();// gets left marker position
	int M2(); // gets right marker position
	double S(); // gets peak's area
	double dS(); // gets the statistical error of peaks area
	void SetName(QString name); // sets name
	QString Name(); // gets name
private:
	SoECSP1 *m_sp1;
	int m1;
	int m2;
	QString m_name;
};
class SORTLIBSHARED_EXPORT SoECSP2: public SoECounter{//2-d spectrum
	Q_OBJECT
public:
	SoECSP2(SoDFReader *reader);
	SoECSP2(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual void LoadCache(QDataStream &str)override;
	virtual void SaveCache(QDataStream &str)override;
	virtual ~SoECSP2();
	virtual QString DisplayName()override;
	void SetXAxis(SoSPAxis *ax); // set axis for X direction
	void SetYAxis(SoSPAxis *ax); // set axis for Y direction
	SoSPAxis *XAxis(); // get axis currently used for X direction
	SoSPAxis *YAxis(); // get axis currently used for Y direction
	int Cnt(int xch,int ych); // get events count in a channel
	void Export(); // Exports data to currently used data output extension
protected:
	void AddCnt(int xch,int ych); // adds an event to a channel
protected:
	virtual void DEInit()override;
	virtual bool DEAccepted(DataEvent *event)override;
	virtual void DEFinal()override;
private:
	SoSPAxis *m_xax;
	SoSPAxis *m_yax;
	uint *m_data;
private slots:
	void AxisChanged();
};







/*************************************************
						Global variable
 *************************************************/
class SORTLIBSHARED_EXPORT SoValue:public SObject{
	Q_OBJECT
public:
	SoValue(SortProject *father);
	SoValue(QDataStream &str,SortProject *father);
	virtual ~SoValue();
	virtual void Save(QDataStream &str)override;
	virtual QString DisplayName()override;
	virtual void SetValue(double val);// Sets value
	double Value();// gets value
private:
	double m_value;
};








/**************************************************
						Functions, Spectra axes
 **************************************************/
namespace Math_{class Expr;}//From MAthLibs repository
class SoDFReader;// DataFile reader corresponds to a single measurement and can have it's variables that are the parameters of function
class DataEvent;// Data Event has also it's own variables that can be function's parameters too when it's spectrum axis
class SORTLIBSHARED_EXPORT SoFormula:public SObject{// Function
	Q_OBJECT
public:
	SoFormula(SortProject *father);
	SoFormula(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SoFormula();
	virtual QString DisplayName()override;
	void Set_Expression(Math_::Expr *expr);// Sets the expression
	Math_::Expr *Expression();// Gets currently set expression
	void Del_Expression();// Deletes the expression
	virtual double Value(SoDFReader *fr, DataEvent *event);// Calculates the function's value. Must be thread safe
signals:
	void changedexpr(Math_::Expr *);
protected:
	void SetValueGetParams(SoDFReader *fr, DataEvent *event); // Must be called before using the expression in calculations
protected slots:
	void GetValueForExpr(double &retval, char type, QString name, double x1, double x2); // is used by the expression for obtaining variables and operators
private	slots:
	void catcherror(QString msg);// is connected to expression's error signal and passes it further
	void acceptchanged(Math_::Expr *sender);// is connected to expressions 'changed' signal and calls 'changedexpr'
private:
	Math_::Expr *m_expr;
	bool m_useexpr;
	SoDFReader *vg_fr;
	DataEvent *vg_event;
};
class SORTLIBSHARED_EXPORT SoSPAxis:public SoFormula{// spectrum axis
	Q_OBJECT
public:
	SoSPAxis(SortProject *father);
	SoSPAxis(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SoSPAxis();
	virtual QString DisplayName()override;
	virtual void BeforeDelete()override;
	void SetMin(double val);// Set minimum value of the function displayed on the axis
	void SetMax(double val); // Set maximum value
	void SetChannelW(double val);// Set channel width
	void SetMinMaxChW(double min, double max, double chw);// Set all three values at the same time
	double Min();// Get minimum value
	double Max();// Get maximum value
	double ChannelW();// Get channel width
	double GetMagnitude(int ch);// Get function value corresponding to given channel number
	int ChannelCount();// Get channels count between minimum and maximum
	int getChannel(SoDFReader *fr, DataEvent *event); // get channel number by function arguments
signals:
	void AxisChanged();
private:
	double m_channelw;
	double m_min;
	double m_max;
};
class SORTLIBSHARED_EXPORT SoSPEnergy: public SoSPAxis{//Spectrum axis with dE-E energy calibration
	Q_OBJECT
public:
	SoSPEnergy(SortProject *father);
	SoSPEnergy(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SoSPEnergy();
	virtual QString DisplayName()override;
	void SetXAxis(SoSPAxis *ax);// Sets X-axis for dE-E spectrum this axix is calibrated with
	void SetYAxis(SoSPAxis *ax);// Sets Y-axis for dE-E spectrum this axix is calibrated with
	SoSPAxis *XAxis();// Gets X-axis for dE-E spectrum this axix is calibrated with
	SoSPAxis *YAxis();// Gets Y-axis for dE-E spectrum this axix is calibrated with
	int MarkerCnt();// Gets the count of markers used (is constant. Now there are 3 markers for linear calibration)
	void SetMarker(int N,QPoint p);// Sets marker position on source dE-E spectrum
	void SetMagn(int N, double val);// Sets energy value corresponding to marker position
	QPoint Marker(int N);// Gets marker position by index
	double Magn(int N); // Gets energy value by index
private:
	void buildexpr(); // builds Math_::Expr for calculating the energy.
	SoSPAxis *m_x;
	SoSPAxis *m_y;
	QList<QPoint> m_points;
	QList<double> m_magn;
};











/****************************************************
					Function on group
*****************************************************/
namespace Math_{class Expr;}//From MathLibs repository
class SORTLIBSHARED_EXPORT SoGroupDF:public SObject{
	Q_OBJECT
public:
	SoGroupDF(SortProject *father);
	SoGroupDF(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SoGroupDF();
	virtual QString DisplayName()override;
	void SetExpression(QString str);// Sets the identifier of function dependent on datafile used in calculation
	QString Expression();// Gets the identifier of function dependent on datafile used in calculation
	void AddDF(SoDFReader *source); // Adds a datafile for being processed
	void RemDF(SoDFReader *source); // Excludes a datafile from processing list
	SoDFReader *GetDF(int N); // Gets datafile by index in processing list
	int DFCnt(); // Gets datafiles count in processing list
	void SetSumm(); // Sets the result returned by this function to the summ of values of the identifier set for processed datafiles
	void SetSummSq(); // Sets the result returned by this function to the summ of squares of values of the identifier set for processed datafiles
	void SetAverage();// Sets this function to return average value
	void SetDispersion();// sets this function to return the dispersion
	QString FormulaType(); // gets the type of the result returned for displaying it in GUI
	double Value(); // Gets the value returned by this function
private:
	QString m_expr;
	char m_functype;
	QList<SoDFReader*> m_analyse;
};









/********************************************************
									Plot Data
********************************************************/
class SORTLIBSHARED_EXPORT SoDependency:public SObject{// plot data
	Q_OBJECT
public:
	SoDependency(SortProject *father);
	SoDependency(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SoDependency();
	virtual QString DisplayName()override;
	void SetArgument(QString str);// Sets the identifier for X-value
	void SetValue(QString str);// Sets the identifier for Y-value
	void SetXError(QString str);// Sets the identifier for X-error
	void SetYError(QString str);// Sets the identifier for Y-error
	QString Argument();// Gets the identifier for X-value
	QString Value();// Gets the identifier for Y-value
	QString XError();// Gets the identifier for X-error
	QString YError();// Gets the identifier for Y-error
	void AddDF(SoDFReader *source); // adds datafile to processing list
	void RemDF(SoDFReader *source); // excludes datafile from processing list
	SoDFReader *GetDF(int N); // gets datafile by index in processing list
	int DFCnt();// gets processed datafiles count
	virtual void Export(); // Export data to currently used data output extension (can be overriden in derivative objects)
	void ExportTo();// Export data to currently used data output extension (default)
private slots:
	void exprerr(QString msg);
	void dr_deleting(SObject*);
private:
	QList<SoDFReader*> m_analyse;
	QString m_argument;
	QString m_value;
	QString m_xerror;
	QString m_yerror;
};
class SORTLIBSHARED_EXPORT SoDSewed:public SoDependency{// extended plot data
	Q_OBJECT
public:
	SoDSewed(SortProject *father);
	SoDSewed(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SoDSewed();
	virtual QString DisplayName()override;
	void SetSewed(QString str);// Attach other plot data by name
	QString Sewed();// Get the name of attached plot data
	virtual void Export()override; // Export data of this data set and attached one
private:
	QString m_sewwith;
};








/**********************************************************
						Unary operators
 **********************************************************/
class SoDFReader;// DataFile reader corresponds to a single measurement and can have it's variables that are the parameters of function
class DataEvent;// Data Event has also it's own variables that can be function's parameters too when it's spectrum axis
class SORTLIBSHARED_EXPORT SoUnaryOperator: public SObject{//abstract unary operator
	Q_OBJECT
public:
	SoUnaryOperator(SortProject *proj);
	SoUnaryOperator(QDataStream &str, SortProject *proj);
	virtual ~SoUnaryOperator();
	virtual QString DisplayName()override;
	virtual double F(double x, SoDFReader *, DataEvent *)=0;// is called in SortProject::GetUnary
};
class SORTLIBSHARED_EXPORT SoTblFunc:public SoUnaryOperator{// abstract table function
	Q_OBJECT
protected:
	SoTblFunc(SortProject *father);
public:
	SoTblFunc(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SoTblFunc();
	virtual QString DisplayName()override;
	virtual double F(double x, SoDFReader *, DataEvent *)override;
	void Export();// Export data to currently used data output extension
	int Count();// Get items count in the table
	void GetItem(int n, double &x, double &y, double &dy);// get an item from table by index. x,y and dy are filled with the values
protected:
	void clear_values();// clear the table before refreshing
	void add_item(double x, double y, double dy);// add item to the table
protected slots:
	virtual void recalc();// recalculates or refreshes the table this ipmlementation prepares for interpolation and should be called in inherited one
private:
	QList<double> X;
	QList<double> Y;
	QList<double> Y_;
	QList<double> dY;
};
class SORTLIBSHARED_EXPORT SoTblFuncTxt:public SoTblFunc{// table function read from text file
	Q_OBJECT
public:
	SoTblFuncTxt(SortProject *father);
	SoTblFuncTxt(QDataStream &str,SortProject *father, bool old=false);
	virtual void Save(QDataStream &str)override;
	virtual ~SoTblFuncTxt();
	virtual QString DisplayName()override;
	void SetPath(QString path);// Set text file path
	QString Path();// get text file path
	void SetColumn(uint ind, int col);// Set column number for index (0 - x; 1 - y; 2 - y-error); if column number is set to -1, it's filled with zeros
	int Column(uint ind);// Get column number for index (0 - x; 1 - y; 2 - y-error)
protected:
	virtual void recalc()override;// read the text file and fill the table
private:
	QString m_path;
	int columns[3];
};
class SORTLIBSHARED_EXPORT SP1Norm:public SoTblFunc{// normalised spectrum
	Q_OBJECT
public:
	SP1Norm(SoECSP1 *sp1);// requires sp1 it will depend on
	SP1Norm(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SP1Norm();
	virtual QString DisplayName()override;
	virtual SObject *DisplParrent()override;// is displayed as an sp1 subitem
	SoECSP1 *Spectrum();// gets the sp1 this object depends on
	void SetNormFunc(QString name);// Sets the identifier of normalizing function
	QString NormFunc();// Gets the identifier of normalizing function
public slots:
	void change();// is called when the sp1 changes
protected:
	virtual void recalc()override;
	QString m_func;
private:
	SoECSP1 *m_sp1;
};
class SORTLIBSHARED_EXPORT SP1Norm_CoefErr: public SP1Norm{//normalized spectrum with known normalise function error
	Q_OBJECT
public:
	SP1Norm_CoefErr(SoECSP1 *sp1);
	SP1Norm_CoefErr(QDataStream &str,SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SP1Norm_CoefErr();
	void SetNormFuncErr(QString name);// Sets the identifier of normalise coefficient error function
	QString NormFuncErr();// Gets the identifier of normalise coefficient error function
	void SetNormFunc(QString name,QString nameerr);// Sets the both identifiers: normalise function and normalise error function
protected:
	virtual void recalc()override;
private:
	QString m_func_err;
};










/**********************************************************
						Binary operator
 **********************************************************/
class SoDFReader;// DataFile reader corresponds to a single measurement and can have it's variables that are the parameters of function
class DataEvent;// Data Event has also it's own variables that can be function's parameters too when it's spectrum axis
class SORTLIBSHARED_EXPORT SoBinaryOperator: public SObject{//Abstract binary operator
	Q_OBJECT
public:
	SoBinaryOperator(SortProject *proj);
	SoBinaryOperator(QDataStream &str, SortProject *proj);
	virtual ~SoBinaryOperator();
	virtual QString DisplayName()override;
	virtual double F(double, double, SoDFReader *, DataEvent *)=0;// is called in SortProject::GetBinary
};
class SORTLIBSHARED_EXPORT SoTableOfTables:public SoBinaryOperator{// simple interpolated binary operator
	Q_OBJECT
public:
	SoTableOfTables(SortProject *proj);
	SoTableOfTables(QDataStream &str,SortProject *proj);
	virtual void Save(QDataStream &str)override;
	virtual ~SoTableOfTables();
	virtual double F(double x, double y, SoDFReader *dr, DataEvent *event)override;
	void Add(double y, QString name);// add y-value and unary operator name pair
	QString GetUnaryName(int index);// get unary operator name by the index
	double GetY(int index);// get y-value by the index
	void Remove(int index);// revove the pair by index
	int Count();// get pairs count
private:
	QList<double> m_yvalues;
	QList<QString> m_unarynames;
};

#endif // SORTPROJECT_H
