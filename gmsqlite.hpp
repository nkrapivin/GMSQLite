#pragma once
#ifndef _GMSQLITE_H_
#define _GMSQLITE_H_

#if !defined( _MSC_VER)
#define GMSQLITE_API __attribute__((visibility("default")))
#else
#define GMSQLITE_API __declspec(dllexport)
#endif


#define GM_DOUBLE extern "C" GMSQLITE_API double       
#define GM_STRING extern "C" GMSQLITE_API const char * 

// handy cast macroses.
#define scd(x) (static_cast<double>(x))
#define scs(x) (static_cast<size_t>(std::floor(x)))
#define sci(x) (static_cast<signed int>(std::floor(x)))

// length of the last error msg array.
#define LAST_ERR_MSG_LEN (8192)

// invalid db index error code.
#define Error (-1000.0)

// these are called automatically by GM.
// if u call them manually, THE PUG will be vewy angery.
GM_DOUBLE RegisterCallbacks(void* createasynevent, void* createmap, void* adddouble, void* addstring);
GM_DOUBLE gmsqlite_init();
GM_DOUBLE gmsqlite_quit();

// stuff for the user.
GM_DOUBLE gmsqlite_config(double _conf, double _prop);
GM_DOUBLE gmsqlite_open(const char *filename);
GM_DOUBLE gmsqlite_close(double _index);
GM_STRING gmsqlite_errmsg(double _index); // for gmsqlite_open error messages.
GM_DOUBLE gmsqlite_exec(double _index, const char *_sql); // _sql -> SQL query.
GM_STRING gmsqlite_exec_errmsg(); // for gmsqlite_exec error messages.
GM_STRING gmsqlite_check();

// sqlite callback.
static int gmsqlite_callback(void* _myAsyncId, int argc, char* argv[], char* azColName[]);

// GM callback function types.
typedef int(*gm_createmap)(int _num, ...);
typedef bool(*gm_adddouble)(int _index, const char *_pKey, double value);
typedef bool(*gm_addstring)(int _index, const char *_pKey, const char *_pVal);
typedef void(*gm_createevent)(int dsmapindex, int event_index);

// GM "Async - Social" event subtype.
const int EVENT_OTHER_SOCIAL = 70;

#endif /* _GMSQLITE_H_ */

