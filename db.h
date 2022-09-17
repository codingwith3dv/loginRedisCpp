#include <memory>
#include <sw/redis++/redis++.h>

void openConnection(const std::string& url);
sw::redis::Redis* getDB();
