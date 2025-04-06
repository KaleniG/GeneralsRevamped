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
    void* ParseVec4Color(const std::string& val);
    void* ParseVec3Coord(const std::string& val);
    void* ParseVec3Color(const std::string& val);
    void* ParseVec2Coord(const std::string& val);
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

    using ParseMethod = std::function<void* (const std::string&)>;

    class Parser
    {
    public:
      template <typename T, typename... Associations>
      static void AddAssociation(T* reference, ParseMethod method, const std::string& head, Associations... association)
      {
        SAGE_ASSERT(method, "[SYSTEM] Cannot add an association with invalid ParseMethod");
        SAGE_ASSERT(reference, "[SYSTEM] Cannot add an association with invalid Reference");

        std::vector<std::string> associations;
        (associations.push_back(std::string(association)), ...);

        if (!Parser::Get().s_AssociationMap[head].SubAssociation)
          Parser::Get().s_AssociationMap[head].SubAssociation = std::make_unique<AssociationMap>();

        AssocStruct* currentSubAssoc = &Parser::Get().s_AssociationMap[head];

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
#if defined(CONFIG_DEBUG) || defined(CONFIG_RELEASE)
          std::string fullAssoc;
          fullAssoc.append("|" + head + "|");
          for (const std::string& assoc : associations)
            fullAssoc.append(assoc + "|");
          SAGE_WARN("[SYSTEM] Overriding the ParseCouple of the association '{}'", fullAssoc);
#endif
          currentSubAssoc->ParseCouple.reset();
        }
        currentSubAssoc->ParseCouple = std::make_unique<ParseCouple>(method, (void*)reference, sizeof(*reference));
      }
      template <typename T>
      static void AddAssociation(T* reference, ParseMethod method, const std::string& head)
      {
        SAGE_ASSERT(method, "[SYSTEM] Cannot add an association with invalid ParseMethod");
        SAGE_ASSERT(reference, "[SYSTEM] Cannot add an association with invalid Reference");

        if (Parser::Get().s_AssociationMap[head].ParseCouple)
        {
#if defined(CONFIG_DEBUG) || defined(CONFIG_RELEASE)
          std::string fullAssoc;
          fullAssoc.append("|" + head + "|");
          SAGE_WARN("[SYSTEM] Overriding the ParseCouple of the association '{}'", fullAssoc);
#endif
          Parser::Get().s_AssociationMap[head].ParseCouple.reset();
        }
        Parser::Get().s_AssociationMap[head].ParseCouple = std::make_unique<ParseCouple>(method, (void*)reference, sizeof(*reference));
      }
      template <typename... Associations>
      static void AddAssociation(ParseMethod method, const std::string& head, Associations... association)
      {
        SAGE_ASSERT(method, "[SYSTEM] Cannot add an association with invalid ParseMethod");

        std::vector<std::string> associations;
        (associations.push_back(std::string(association)), ...);

        if (!Parser::Get().s_AssociationMap[head].SubAssociation)
          Parser::Get().s_AssociationMap[head].SubAssociation = std::make_unique<AssociationMap>();

        AssocStruct* currentSubAssoc = &Parser::Get().s_AssociationMap[head];

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
#if defined(CONFIG_DEBUG) || defined(CONFIG_RELEASE)
          std::string fullAssoc;
          fullAssoc.append("|" + head + "|");
          for (const std::string& assoc : associations)
            fullAssoc.append(assoc + "|");
          SAGE_WARN("[SYSTEM] Overriding the ParseCouple of the association '{}'", fullAssoc);
#endif
          currentSubAssoc->ParseCouple.reset();
        }
        currentSubAssoc->ParseCouple = std::make_unique<ParseCouple>(method, nullptr, 0);
      }
      static void AddAssociation(ParseMethod method, const std::string& head)
      {
        SAGE_ASSERT(method, "[SYSTEM] Cannot add an association with invalid ParseMethod");

        if (Parser::Get().s_AssociationMap[head].ParseCouple)
        {
#if defined(CONFIG_DEBUG) || defined(CONFIG_RELEASE)
          std::string fullAssoc;
          fullAssoc.append("|" + head + "|");
          SAGE_WARN("[SYSTEM] Overriding the ParseCouple of the association '{}'", fullAssoc);
#endif
          Parser::Get().s_AssociationMap[head].ParseCouple.reset();
        }
        Parser::Get().s_AssociationMap[head].ParseCouple = std::make_unique<ParseCouple>(method, nullptr, 0);
      }
      template <typename... Associations>
      static void DeleteAssociation(Associations... association)
      {
        std::vector<std::string> associations;
        (associations.push_back(std::string(association)), ...);

        AssociationMap* currentHead = &Parser::Get().s_AssociationMap;

        for (int32_t i = 0; i < associations.size() - 1; i++)
        {
          SAGE_ASSERT(currentHead->find(associations[i]) != currentHead->end(), "[SYSTEM] Trying to erase from unknown header '{}'", associations[i]);
          SAGE_ASSERT((*currentHead)[associations[i]].SubAssociation, "[SYSTEM] Trying to erase from uninitialized header '{}'", associations[i]);
          currentHead = (*currentHead)[associations[i]].SubAssociation.get();
        }

        currentHead->erase(associations.back());
        Parser::ShrinkToFit(Parser::Get().s_AssociationMap);
      }
      static void ResetAssociations();
      static void ParseFile(const std::filesystem::path& filepath, bool ignore_unknown_headers = false);
      static void PrintAssociations();

    private:
      static Parser& Get() { static Parser instance; return instance; }

      Parser() = default;
      Parser(const Parser&) = delete;
      Parser& operator=(const Parser&) = delete;
      Parser(Parser&&) = delete;
      Parser& operator=(Parser&&) = delete;

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
}