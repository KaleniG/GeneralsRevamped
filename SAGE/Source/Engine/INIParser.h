#pragma once

#include <unordered_map>
#include <filesystem>
#include <functional>
#include <string>
#include <memory>

#include "Core/File.h"

namespace sage
{
  namespace INI
  {
    void* ParseNothing(const std::string& val);
    void* ParseBool(const std::string& val);
    void* ParseFloat(const std::string& val);
    void* ParseFloatAngle(const std::string& val);
    void* ParseFloatPercent(const std::string& val);
    void* ParseInt64(const std::string& val);
    void* ParseInt32(const std::string& val);
    void* ParseInt16(const std::string& val);
    void* ParseInt8(const std::string& val);
    void* ParseUInt64(const std::string& val);
    void* ParseUInt32(const std::string& val);
    void* ParseUInt16(const std::string& val);
    void* ParseUInt8(const std::string& val);
    void* ParseString(const std::string& val);
    void* ParseQuotedString(const std::string& val);
  }
  
  using ParseMethod = std::function<void*(const std::string&)>;

  class INIParser
  {
  public:
    template <typename T, typename... Associations>
    static void AddAssociation(ParseMethod method, T* reference, const std::string& head, Associations... association)
    {
      SAGE_ASSERT(method != nullptr, "[SYSTEM] Cannot add an association with invalid ParseMethod");

      std::vector<std::string> associations;
      (associations.push_back(std::string(association)), ...);

      if (!INIParser::Get().s_AssociationMap[head].SubAssociation)
        INIParser::Get().s_AssociationMap[head].SubAssociation = std::make_unique<AssociationMap>();

      AssocStruct* currentSubAssoc = &INIParser::Get().s_AssociationMap[head];

      for (int32_t i = 0; i < associations.size(); i++)
      {
        const std::string& assoc = associations[i];

        if (!currentSubAssoc->SubAssociation)
          currentSubAssoc->SubAssociation = std::make_unique<AssociationMap>();

        currentSubAssoc = &(*currentSubAssoc->SubAssociation)[assoc];
      }

      const std::string& lastAssoc = associations.back();
      if (currentSubAssoc->ParseCouple)
      {
        std::string fullAssoc;
        fullAssoc.append("|" + head + "|");
        for (const std::string& assoc : associations)
          fullAssoc.append(assoc + "|");
        SAGE_WARN("[SYSTEM] Overriding the ParseCouple of the association '{}'", fullAssoc);
        currentSubAssoc->ParseCouple.reset();
      }
      currentSubAssoc->ParseCouple = std::make_unique<ParseCouple>(method, (reference) ? (void*)reference : nullptr, (reference) ? sizeof(*reference) : 0);
    }
    template <typename T>
    static void AddAssociation(ParseMethod method, T* reference, const std::string& head)
    {
      SAGE_ASSERT(method != nullptr, "[SYSTEM] Cannot add an association with invalid ParseMethod");

      if (INIParser::Get().s_AssociationMap[head].ParseCouple)
      {
        std::string fullAssoc;
        fullAssoc.append("|" + head + "|");
        SAGE_WARN("[SYSTEM] Overriding the ParseCouple of the association '{}'", fullAssoc);
        INIParser::Get().s_AssociationMap[head].ParseCouple.reset();
      }
      INIParser::Get().s_AssociationMap[head].ParseCouple = std::make_unique<ParseCouple>(method, (reference) ? (void*)reference : nullptr, (reference) ? sizeof(*reference) : 0);
    }
    template <typename... Associations>
    static void DeleteAssociation(Associations... association)
    {
      std::vector<std::string> associations;
      (associations.push_back(std::string(association)), ...);

      AssociationMap* currentHead = &INIParser::Get().s_AssociationMap;

      for (int32_t i = 0; i < associations.size() - 1; i++)
      {
        SAGE_ASSERT(currentHead->find(associations[i]) != currentHead->end(), "[SYSTEM] Trying to erase from unknown header '{}'", associations[i]);
        SAGE_ASSERT((*currentHead)[associations[i]].SubAssociation, "[SYSTEM] Trying to erase from uninitialized header '{}'", associations[i]);
        currentHead = (*currentHead)[associations[i]].SubAssociation.get();
      }

      currentHead->erase(associations.back());
      INIParser::ShrinkToFit(INIParser::Get().s_AssociationMap);
    }
    static void ResetAssociations();
    static void ParseFile(const std::filesystem::path& filepath, bool ignore_unknown_headers = false);

    static void PrintAssociations();

  private:
    static INIParser& Get() { static INIParser instance; return instance; }

    INIParser() = default;
    INIParser(const INIParser&) = delete;
    INIParser& operator=(const INIParser&) = delete;
    INIParser(INIParser&&) = delete;
    INIParser& operator=(INIParser&&) = delete;

  private:
    struct AssocStruct;
    using AssociationMap = std::unordered_map<std::string, AssocStruct>;

    struct ParseCouple
    {
      ParseMethod Method;
      void* Reference = nullptr;
      int32_t ReferenceSize = 0;
    };

    struct AssocStruct
    {
      std::unique_ptr<ParseCouple> ParseCouple;
      std::unique_ptr<AssociationMap> SubAssociation;
    };

  private:
    AssociationMap s_AssociationMap;

  private:
    static void CheckSubAssociation(const AssociationMap& subassoc_map, const std::string& parent_head);
    static bool ShrinkToFit(AssociationMap& subassoc_map);
  };
}