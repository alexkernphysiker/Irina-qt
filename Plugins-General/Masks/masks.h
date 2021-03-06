// this file is distributed under
// GPL v 3.0 license
#ifndef MASKS_H
#define MASKS_H
#include <SortLib.h>
#include <SortLibGui.h>

#define SOT_ExtendedMask 0x0404

class ExtendedMask: public SoEFilter{
public:
	ExtendedMask(SortProject *father);
	ExtendedMask(QDataStream &str, SortProject *father);
	virtual ~ExtendedMask();
	virtual void Save(QDataStream &str)override;

	virtual bool In(SoDFReader *dr, DataEvent *event)override;
	virtual QString DisplayName()override;
	virtual void *GetForm()override;

	QString Func1();
	void SetFunc1(QString name);
	QString Func2();
	void SetFunc2(QString name);
	QString Predicate();
	void SetPredicate(QString value);
private:
	bool getpredicate(double opr1, double opr2, bool errorreport=false);
	QString opr1;
	QString opr2;
	QString predicate;
};

#endif // MASKS_H
