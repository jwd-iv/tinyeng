#include "tiny.h"

namespace tiny
{
/*  bool ResourceManager::IsResource(char const* filename) const
  {
    if (filename == NULL || std::string("") == filename)
      return false;

    auto file = Systems::Get<FileService>()->Open(filename);
    if (file.Data() == NULL || !file->Exists())
    {
      Systems::Get<FileService>()->Close(file);
      return false;
    }
    
    bool found = false;
    auto pair = resTypes.find(file->Ext());
    Systems::Get<FileService>()->Close(file);
    return pair != resTypes.end();
  }

  Data::Handle ResourceManager::Load(char const* filename)
  {
    if(!IsResource(filename))
      return Data::Handle();

    auto pair = loaded.find(filename);
    if (pair != loaded.end())
      return pair->second;

    auto file = Systems::Get<FileService>()->Open(filename);
    auto type = resTypes[file->Ext()];
    
    Resource* res = static_cast<Resource*>(type->Factory().New());

    if (res != NULL)
    {
      res->filename = filename;
      res->Load(file); //TODO: handle this function returning false
      loaded[filename] = res->GetID();
    }

    Systems::Get<FileService>()->Close(file);
    return res == NULL ? Data::Handle() : res->GetID();
  }

  bool ResourceManager::Unload(Data::Handle resource)
  {
    Resource* res = resource.To<Resource>();
    if(res == NULL)
      return false;
    else
    {
      bool success = res->Unload();
      loaded.erase(res->filename);
      res->GetType()->Factory().Delete(res);
      return success;
    }
  }

  std::vector<std::string> ParseExt(std::string extensions)
  {
    std::vector<std::string> exts;

    unsigned b = 0;
    unsigned e = 0;

    while (b < extensions.size())
    {
      e = extensions.find_first_of(",./+; ", b);
      if (e > extensions.size())
        e = extensions.size();

      exts.push_back(extensions.substr(b, e - b));
      if ((--exts.end())->empty())
        exts.pop_back();
      b = e + 1;
    }

    return exts;
  }

  void ResourceManager::Initialize()
  {
    std::list<Meta::TypeInfo> resources;

    for (auto const& iter : Meta::GetType<Resource>()->children)
      resources.push_back(iter);

    while (!resources.empty())
    {
      Meta::TypeInfo rtype = resources.front();
      resources.pop_front();

      for (auto const& iter : rtype->children)
        resources.push_back(iter);

      auto res = rtype->Create();
      if (res.Data() != NULL)
        for (auto const& ext : ParseExt(res.To<Resource>()->Extensions()))
          resTypes[ext] = rtype;
    }
  }

  void ResourceManager::Close()
  {
    while (!loaded.empty())
      Unload(loaded.begin()->second);
  }

  void ResourceManager::Update(float dt)
  {
  }*/
}
