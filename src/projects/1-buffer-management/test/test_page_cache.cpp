#include "../page_cache_adapter.hpp"
#include "dependencies/sqlite/sqlite3.h"
#include <iostream>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main() {
  int rc;
  char *zErrMsg = 0;

  rc = sqlite3_config(SQLITE_CONFIG_PCACHE2, &bufferManagerMethods);
  std::cout << rc << std::endl;

  rc = sqlite3_initialize();
  std::cout << rc << std::endl;

  sqlite3 *db;

  rc = sqlite3_open("test.sqlite", &db);
  std::cout << rc << std::endl;

  rc = sqlite3_exec(db, "DROP TABLE IF EXISTS T", callback, 0, &zErrMsg);
  rc = sqlite3_exec(db, "CREATE TABLE T (a INT)", callback, 0, &zErrMsg);
  rc = sqlite3_exec(db, "INSERT INTO T VALUES (1), (2), (3)", callback, 0,
                    &zErrMsg);

  std::cout << rc << std::endl;

  return 0;
}
