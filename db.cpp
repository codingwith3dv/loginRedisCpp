#include "db.h"
#include <sw/redis++/redis++.h>

static std::unique_ptr<sw::redis::Redis> db;

void openConnection(const std::string& url) {
  db = std::make_unique<sw::redis::Redis>(url);
}

sw::redis::Redis* getDB() {
  return db.get();
}
