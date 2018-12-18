#include "tiny.h"
#include "json\json.h"

struct json_format : public tiny::serializer::format
{
  // Inherited via format
  virtual std::string ext() const;
  virtual riku::variant parse(char const* text) const;
  virtual std::string serialize(riku::variant obj) const;

  rkMetaHook(json_format);
};

struct json_blob : public riku::variant_type
{
  // Inherited via variant_type
  virtual riku::typeinfo type() const;
  virtual void* data();
  virtual void const* data() const;

  virtual bool assign(variant_type const& rhs);
  virtual bool modify(variant_type& rhs) const;

  virtual riku::variant property(riku::string n);
  virtual riku::variant property(riku::string n) const;
  virtual std::vector<riku::string> properties() const;

  Json::Value val;

  rkMetaHook(json_blob);
};
