#include "db.h"
#include "json.h"

#include <iostream>
#include <random>

#include <sw/redis++/redis++.h>

using engine = std::mt19937;

static std::random_device rd;
static engine gen(rd());
static std::uniform_int_distribution<> distrib(1000000, 9999999);

struct UserInfo {
  int followers;
  int following;
  
  static constexpr auto properties = std::make_tuple(
    property(&UserInfo::followers, "followers"),
    property(&UserInfo::following, "following")
  );
};

struct User {
  std::string userName;
  uint32_t id;
  UserInfo info;

  static constexpr auto properties = std::make_tuple(
    property(&User::userName, "userName"),
    property(&User::id, "id"),
    property(&User::info, "info", PropertyType::OBJECT)
  );
};

void signUpUser(std::string userName, std::string password) {
  auto existingUserName = getDB()->hget("users", userName);
  if(existingUserName.has_value()) {
    std::cout << "User " << userName <<
                 " already exists!\n";
    return;
  }
  uint32_t id = distrib(gen);
  std::string key(userName);
  key += ":";
  key += std::to_string(id);
  User user { userName, id, { 300, 3 } };
  writeJson(user, key, getDB());
  getDB()->hset("users", userName, std::to_string(id));
  std::cout << "User signed in successfully\n";
}

int main() {
  openConnection("tcp://127.0.0.1:6379");

  // accept input
  std::string userName, password;
  std::cout << "Enter username: ";
  std::cin >> userName;
  std::cout << "Enter password: ";
  std::cin >> password;

  signUpUser(userName, password);

  return 0;
}
