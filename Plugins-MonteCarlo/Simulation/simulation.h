#ifndef SIMULATION_H
#define SIMULATION_H
#include <QtCore/qglobal.h>
#include <SortLib.h>
#include <SortLibGui.h>
#if defined(SIMULATION_LIBRARY)
#  define SIMULATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SIMULATIONSHARED_EXPORT Q_DECL_IMPORT
#endif


#define SOT_Simulation 0x0401
#define SOT_RandomMagnitude 0x0810
#define SOT_GaussMagnitude 0x010810
#define SOT_MixedDistribution 0x020810
#define SOT_DistributedByFunction 0x040810
#define SOT_BinarySumm 0x0303

class SIMULATIONSHARED_EXPORT Simulation:public SoDFReader
{
	Q_OBJECT
public:
	Simulation(SortProject *father);
	Simulation(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str);
	virtual ~Simulation();
	virtual QString DisplayName();
	virtual void *GetForm();
	// Data Event generating
	virtual DataEvent *NextEvent(QDataStream &);
	virtual void ReadHeader(QDataStream &datastr);
	virtual void DataEventIsGot(DataEvent *Event);
	virtual void FinalDataCheck();
	virtual ReaderAsync *_getReaderAsync(QString m_path, SoDFReader *father);
	// Custom simulation parameters
	uint N();
	void SetN(uint val);
	void SetNADC(char val);
	QString Func(char ind);
	void SetFuncs(QStringList val);
private:
	QMutex m_mutex;
	void initfuncs();
	void clearfuncs();
	QStringList functions;
	uint n;
};

class SIMULATIONSHARED_EXPORT SimulationAsync: public ReaderAsync{
	Q_OBJECT
public:
	SimulationAsync(SoDFReader *father, uint N);
	virtual void Open();
protected slots:
	virtual void process();
private:
	QMutex m_mutex;
	uint n;
};


class SIMULATIONSHARED_EXPORT RandomMagnitude: public SoFormula {
	Q_OBJECT
public:
	RandomMagnitude(SortProject *proj);
	RandomMagnitude(QDataStream &str, SortProject *proj);
	virtual void Save(QDataStream &str);
	virtual ~RandomMagnitude();
	virtual QString DisplayName();
	virtual double Value(SoDFReader *fr, DataEvent *event);
	virtual void* GetForm();
	double MaxVal();
protected:
	double maxval;
	double lastval;
	DataEvent *last;
	SoDFReader *lastreader;
};

class SIMULATIONSHARED_EXPORT GaussMagnitude:public RandomMagnitude{
	Q_OBJECT
public:
	GaussMagnitude(SortProject *proj);
	GaussMagnitude(QDataStream &str,SortProject *proj);
	virtual void Save(QDataStream &str);
	virtual ~GaussMagnitude();
	virtual QString DisplayName();
	virtual double Value(SoDFReader *fr, DataEvent *event);
	virtual void *GetForm();
	double Average();
	double Sigma();
	uint Discr();
	void SetAverage(double val);
	void SetSigma(double val);
	void SetDiscr(uint val);
private:
	double average;
	double sigma;
};

class SIMULATIONSHARED_EXPORT SubDistr;

class SIMULATIONSHARED_EXPORT MixDistributions: public RandomMagnitude{
	Q_OBJECT
public:
	MixDistributions(SortProject *proj);
	MixDistributions(QDataStream &str,SortProject *proj);
	virtual void Save(QDataStream &str);
	virtual ~MixDistributions();
	virtual QString DisplayName();
	virtual double Value(SoDFReader *fr, DataEvent *event);
	virtual void *GetForm();
	uint Count();
	double Probability(uint n);
	QString Function(uint n);
	void Remove(uint n);
	void Add(QString name, double probability);
private:
	QList<SubDistr> data;
};

class SIMULATIONSHARED_EXPORT DistributedByFunction:public RandomMagnitude{
	Q_OBJECT
public:
	DistributedByFunction(SoTblFunc *func);
	DistributedByFunction(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str);
	virtual ~DistributedByFunction();
	virtual QString DisplayName();
	virtual double Value(SoDFReader *fr, DataEvent *event);
	virtual void *GetForm();
	virtual SObject *DisplParrent();
private slots:
	void getdata();
private:
	SoTblFunc *m_func;
	double *m_distr;
};

class SIMULATIONSHARED_EXPORT SoBinarySumm:public SoBinaryOperator{
	Q_OBJECT
public:
	SoBinarySumm(SortProject *proj);
	SoBinarySumm(QDataStream &str, SortProject *proj);
	virtual void Save(QDataStream &str);
	virtual ~SoBinarySumm();
	virtual double F(double x, double y, SoDFReader *dr, DataEvent *event);
	virtual void *GetForm();
	int Count();
	QString Summand(int index);
	void Reset();
	void AddSummand(QString name);
private:
	QList<QString> m_oprs;
};

#endif // SIMULATION_H
