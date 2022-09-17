#include <sw/redis++/redis++.h>

#include <string>
#include <iostream>
#include <tuple>

enum class PropertyType {
  SCALAR,
  OBJECT,
  LIST
};

template<typename Class, typename T>
struct PropertyImpl {
  constexpr PropertyImpl(
    T Class::*member,
    const char* name,
    PropertyType type
  )
    : member(member), name(name), type(type) {}

  using dataType = T;

  T Class::*member;
  const char* name;
  PropertyType type;
};

template<typename Class, typename T>
constexpr auto property(
  T Class::*member,
  const char* name,
  PropertyType type = PropertyType::SCALAR
) {
  return PropertyImpl<Class, T>(member, name, type);
}

template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f) {
    (static_cast<void>(f(std::integral_constant<T, S>{})), ...);
}

template<typename T>
std::string toString(T value) { return value; }

template<>
inline std::string toString(uint32_t value) { return std::to_string(value); }
template<>
inline std::string toString(int value) { return std::to_string(value); }

template<typename T>
void object(const T& obj, std::string path, sw::redis::Redis* redis) {
  constexpr size_t size1 = std::tuple_size<decltype(T::properties)>::value;

  for_sequence(std::make_index_sequence<size1>{}, [&](auto i) {
    constexpr auto property = std::get<i>(T::properties);
    
    using dataType = typename decltype(property)::dataType;
    
    if constexpr (property.type != PropertyType::OBJECT) {
      std::string value = toString<dataType>(obj.*(property.member));
      redis->hset(path,
        property.name,
        value
      );
    } else {
      std::string newPath = path;
      newPath += ":";
      newPath += property.name;
      object(obj.*(property.member), newPath, redis);
    }
  });
}

template<typename T>
void writeJson(const T& obj, std::string path, sw::redis::Redis* redis) {
  object(obj, path, redis);
}
