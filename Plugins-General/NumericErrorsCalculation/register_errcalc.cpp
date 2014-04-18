#include "numericerrorscalculation.h"
#include "thiswidget.h"


SObject * adderrorcalc(SortProject *pr, SObject *){
	return (new NumericErrorsCalculation(pr));
}
SObject *loaderrorcalc(QDataStream &str,SortProject *father){
	return new NumericErrorsCalculation(str,father);
}

#include "register_errcalc.h"
bool ErrCalcLoader::Register(){
	return RegisterSoType(new SoTypeReg(
	"Numeric error calculation/1.5","Error calculation",SoCatFormula
	,4,SOT_Formula,&loaderrorcalc, &adderrorcalc));
}
bool ErrCalcLoader::Unregister(){
	return UnregisterSoType(4,SOT_Formula);
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(errcalc, ErrCalcLoader)
QT_END_NAMESPACE
