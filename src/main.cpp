#include "stdafx.h"

DECLARE_COMPONENT_VERSION("Miscellaneous tools","0.2","");

VALIDATE_COMPONENT_FILENAME("foo_misc.dll");


//! \since 0.9.5.4
//! Allows you to spawn a popup Media Library Search window with any query string that you specify. \n
//! Usage: library_search_ui::get()->show("querygoeshere");

//class NOVTABLE library_search_ui : public service_base {