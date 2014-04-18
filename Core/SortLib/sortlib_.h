#ifndef SORTLIBGUIINTERFACE_H
#define SORTLIBGUIINTERFACE_H
#include <SortLib.h>
SORTLIBSHARED_EXPORT QString sortlibversion();
SORTLIBSHARED_EXPORT QString aboutsortlibcaption();
SORTLIBSHARED_EXPORT QString aboutsortlibmsg();
SORTLIBSHARED_EXPORT int RegisteredTypesCount();
SORTLIBSHARED_EXPORT SoTypeReg* GetRegisteredType(int index);
#endif // SORTLIBGUIINTERFACE_H
