#include "core/json_format.h"

riku::variant translate_json(Json::Value const& val)
{
  switch (val.type())
  {
  case Json::objectValue: {
    json_blob b;
    b.val = val;
    return b;
  }

  case Json::arrayValue: {
    riku::array ret;
    for (auto const& iter : val)
      ret.push_back(translate_json(iter));
    return ret;
  }

  case Json::stringValue: {
    if (tiny::systems::get<tiny::filesystem>()->exists(val.asCString()))
      return tiny::systems::get<tiny::serializer>()->parse(val.asCString());
    else
      return val.asString();
  }

  case Json::realValue:
    return (float) val.asDouble();

  case Json::intValue:
    return val.asInt();

  case Json::uintValue:
    return val.asUInt();

  case Json::booleanValue:
    return val.asBool();

  default:
    return riku::variant();
  }
}

riku::typeinfo json_blob::type() const { return meta(); }
void* json_blob::data() { return this; }
void const* json_blob::data() const { return this; }

bool json_blob::assign(variant_type const& rhs)
{
  //TODO: fill in val with rhs properties
  return false;
}

bool json_blob::assignto(variant_type& rhs) const
{
  if(rhs.type() == NULL || rhs.data() == NULL)
    return false;

  bool did_stuff = false;

  auto ret = rhs(std::string("deserialize"), riku::array::create(this));
  if (ret.is<bool>())
    did_stuff |= ret.as<bool>();

  if(!did_stuff)
    for (auto const& member : val.getMemberNames())
      did_stuff |= property(member).assignto(rhs[member]);

  return did_stuff;
}

riku::variant json_blob::property(riku::string n)
{
  return ((json_blob const*) this)->property(n);
}

riku::variant json_blob::property(riku::string n) const
{
  if(val.type() != Json::objectValue)
    return riku::variant();

  return translate_json(val.get(n, Json::Value()));
}

std::vector<riku::string> json_blob::properties() const
{
  if (val.type() == Json::objectValue)
    return val.getMemberNames();

  return std::vector<riku::string>();
}

std::string json_format::ext() const
{
  return "json";
}

riku::variant json_format::parse(char const* text) const
{
  Json::Value v;
  Json::Reader r;
  r.parse(text, text + strlen(text), v);
  return translate_json(v);
}

std::string json_format::serialize(riku::variant obj) const
{
  return std::string();
}
