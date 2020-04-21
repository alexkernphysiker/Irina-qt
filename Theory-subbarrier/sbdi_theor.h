// this file is distributed under
// GPL v 3.0 license
#ifndef SBDI_THEOR_H
#define SBDI_THEOR_H
#include <Core/Irina-qt-headers/SortLib.h>
#include <Core/Irina-qt-headers/SortLibGui.h>

#define SOT_SBDI 0x040105

class SBDI:public SoTblFunc{
	Q_OBJECT
public:
	SBDI(SortProject *father);
	SBDI(QDataStream &str,SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~SBDI();
	virtual QString DisplayName()override;

};

class ElaTheor:public SBDI {
	Q_OBJECT
public:
	ElaTheor(SortProject *father);
	ElaTheor(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~ElaTheor();
	virtual QString DisplayName()override;
	virtual void *GetForm()override;
protected:
	virtual void recalc()override;
public:
	bool ArgIsEnergy();
	void SetArgIsEnergy(bool flag);
	uint Zt();
	void SetZt(uint z);
	double Min();
	void SetMin(double val);
	double Max();
	void SetMax(double val);
	double Step();
	void SetStep(double val);
	double SecondVar();
	void SetSecondVar(double val);
private:
	bool m_en;
	uint zt;
	double min;
	double max;
	double step;
	double secvar;
};

#endif // SBDI_THEOR_H
