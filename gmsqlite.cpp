// gmsqlite.cpp : Defines the exported functions for the DLL.
//

#include <vector>
#include <string>
#include <cmath>
#include "gmsqlite.hpp"
#include "sqlite3.h"

#define DB_FUNC_START                 \
	size_t index = scs(_index);       \
	if (index > Databases.size() - 1) \
		return Error;

#define DB_FUNC_START_STRING          \
	size_t index = scs(_index);       \
	if (index > Databases.size() - 1) \
		return "";



// GM stuff.
gm_createevent CreateAsynEventWithDSMap = nullptr;
gm_createmap CreateDsMap = nullptr;
gm_adddouble DsMapAddDouble = nullptr;
gm_addstring DsMapAddString = nullptr;

// also GM but gmsqlite stuff.
uintptr_t LastAsyncId = 0;
std::string LastErrorMessage{ };

std::vector<sqlite3*> Databases{ };

bool CheckHandles() {
	return (CreateAsynEventWithDSMap != nullptr && CreateDsMap != nullptr && DsMapAddDouble != nullptr && DsMapAddString != nullptr);
}

static int gmsqlite_callback(void* _myAsyncId, int argc, char* argv[], char* azColName[]) {
	auto myAsyncId = reinterpret_cast<uintptr_t>(_myAsyncId);

	int myMap = CreateDsMap(0);
	DsMapAddString(myMap, "event_type", "gmsqlite_exec_result");
	DsMapAddDouble(myMap, "gmsqlite_exec_argc", scd(argc));
	DsMapAddDouble(myMap, "gmsqlite_exec_id", scd(myAsyncId));

	// TODO: a better way?
	int insideMap = CreateDsMap(0);
	DsMapAddDouble(myMap, "gmsqlite_exec_result", scd(insideMap));
	for (int i = 0; i < argc; i++) {
		DsMapAddString(insideMap, azColName[i], (argv[i] != nullptr) ? argv[i] : "gmsqlite_<undefined>");
	}

	CreateAsynEventWithDSMap(myMap, EVENT_OTHER_SOCIAL);

	return SQLITE_OK;
}

GM_DOUBLE RegisterCallbacks(void* createasynevent, void* createmap, void* adddouble, void* addstring) {
	CreateAsynEventWithDSMap = reinterpret_cast<gm_createevent>(createasynevent);
	CreateDsMap = reinterpret_cast<gm_createmap>(createmap);
	DsMapAddDouble = reinterpret_cast<gm_adddouble>(adddouble);
	DsMapAddString = reinterpret_cast<gm_addstring>(addstring);
	return scd(CheckHandles());
}

GM_DOUBLE gmsqlite_init() {
	return scd(-sqlite3_initialize());
}

GM_DOUBLE gmsqlite_quit() {
	LastErrorMessage.clear();
	LastErrorMessage.shrink_to_fit();

	// try to close all current db connections.
	for (auto& db : Databases) {
		sqlite3_close(db);
	}

	return scd(-sqlite3_shutdown());
}

GM_STRING gmsqlite_check() {
	return "GMSQLite";
}

GM_DOUBLE gmsqlite_config(double _conf, double _prop) {
	return scd(-sqlite3_config(sci(_conf), sci(_prop)));
}

GM_DOUBLE gmsqlite_open(const char *filename) {
	Databases.push_back(nullptr);
	int err = sqlite3_open(filename, &Databases.back());
	if (err != SQLITE_OK) {
		sqlite3_close(Databases.back());
		return -err;
	}
	else {
		return scd(Databases.size() - 1);
	}
}

GM_DOUBLE gmsqlite_close(double _index) {
	DB_FUNC_START;

	double ret = scd(-sqlite3_close(Databases[index]));
	if (index == Databases.size() - 1) { // this is the last item, can pop it.
		Databases.pop_back();
	}

	return ret;
}

GM_STRING gmsqlite_errmsg(double _index) {
	DB_FUNC_START_STRING;
	return sqlite3_errmsg(Databases[index]);
}

GM_STRING gmsqlite_exec_errmsg() {
	return LastErrorMessage.c_str();
}

GM_DOUBLE gmsqlite_exec(double _index, const char *_sql) {
	DB_FUNC_START;

	// error msg setup.
	char *zErrMsg = nullptr;

	// call the query.
	int err = sqlite3_exec(Databases[index], _sql, gmsqlite_callback, reinterpret_cast<void *>(LastAsyncId), &zErrMsg);

	// return the error code or the async id.
	if (err != SQLITE_OK) {
		LastErrorMessage = zErrMsg;
		sqlite3_free(zErrMsg);
		return -err;
	}
	else {
		LastAsyncId++;
		if (LastAsyncId == UINTPTR_MAX) LastAsyncId = 0; // reset to 0 if out of range.
		return scd(LastAsyncId - 1);
	}
}
