#include <pch.h>

#include "Engine/INIParser.h"
#include "Engine/FileSystem.h"

namespace sage
{
  void INIParser::ResetAssociations()
  {
    INIParser::Get().s_AssociationMap.clear();
  }

  void INIParser::ParseFile(const std::filesystem::path& filepath)
  {
    RAMFile file = FileSystem::OpenRAMFile(filepath);
    std::vector<std::string> headTokens;
    AssociationMap* currentHead = &INIParser::Get().s_AssociationMap;

    while (!file.IsEndOfFile())
    {
      std::string line;
      if (file.GetLine(line) == false) 
        break;

      // Deleting everything that is after the ';' character, since it is a comment
      {
        size_t pos = line.find(";");
        if (pos != std::string::npos)
          line.erase(pos);
      }

      size_t spacePos = line.find_first_not_of(" =\n\r\t");
      if (spacePos == std::string::npos)
        continue;

      std::string firstToken = line.substr(spacePos);
      size_t endFirstToken = firstToken.find_first_of(" =\n\r\t");
      firstToken = firstToken.substr(0, endFirstToken);
      if (firstToken == "End" || firstToken == "END")
      {
        SAGE_ASSERT(!headTokens.empty(), "[SYSTEM] Read an 'End' header without a starting header in file '{}', position '{}'", file.GetFilepath().string(), file.GetPosition());
        headTokens.pop_back();
        currentHead = &INIParser::Get().s_AssociationMap;
        for (const std::string& token : headTokens)
          currentHead = (*currentHead)[token].SubAssociation.get();
        continue;
      }

      std::string restOfString = line.substr(spacePos + endFirstToken);
      if (restOfString.find_first_not_of(" =\n\r\t") == std::string::npos || restOfString.empty())
        restOfString.clear();

      SAGE_ASSERT(currentHead->find(firstToken) != currentHead->end(), "[SYSTEM] Read and unknown header '{}' from file '{}', position '{}'", firstToken, file.GetFilepath().string(), file.GetPosition());
      if (currentHead->find(firstToken) != currentHead->end())
      {
        if ((*currentHead)[firstToken].ParseCouple)
        {
          ParseCouple& couple = *(*currentHead)[firstToken].ParseCouple.get();
          void* resultVal = couple.Method(restOfString);
          std::memcpy(couple.Reference, resultVal, couple.ReferenceSize);
          delete resultVal;
        }
        if ((*currentHead)[firstToken].SubAssociation)
        {
          headTokens.push_back(firstToken);
          currentHead = (*currentHead)[firstToken].SubAssociation.get();
        }
      }
    }
    SAGE_ASSERT(headTokens.empty(), "[SYSTEM] Not all headers ending with 'End' header in file '{}'", file.GetFilepath().string());
  }

  void INIParser::PrintAssociations()
  {
    for (const auto& headEntry : Get().s_AssociationMap)
    {
      const auto& head = headEntry.first;
      const auto& assocStruct = headEntry.second;

      if (assocStruct.SubAssociation)
      {
        SAGE_INFO("[TEST] |{}| has subassec", head);
        INIParser::CheckSubAssociation(*assocStruct.SubAssociation, head);
      }
      else
      {
        SAGE_ERROR("[TEST] |{}| has no subassec", head);
      }

      if (assocStruct.ParseCouple)
      {
        SAGE_INFO("[TEST] |{}| has parsecouple", head);
      }
      else
      {
        SAGE_ERROR("[TEST] |{}| has no parsecouple", head);
      }
    }
  }

  void INIParser::CheckSubAssociation(const AssociationMap& subassoc_map, const std::string& parent_head)
  {
    for (const auto& subEntry : subassoc_map)
    {
      const auto& subAssoc = subEntry.first;
      const auto& assocStruct = subEntry.second;

      if (assocStruct.SubAssociation)
      {
        SAGE_INFO("[TEST] |{}|{}| has subassec", parent_head, subAssoc);
        CheckSubAssociation(*assocStruct.SubAssociation, parent_head + "|" + subAssoc);
      }
      else
      {
        SAGE_ERROR("[TEST] |{}|{}| has no subassec", parent_head, subAssoc);
      }

      if (assocStruct.ParseCouple)
      {
        SAGE_INFO("[TEST] |{}|{}| has parsecouple", parent_head, subAssoc);
      }
      else
      {
        SAGE_ERROR("[TEST] |{}|{}| has no parsecouple", parent_head, subAssoc);
      }
    }
  }

  bool INIParser::ShrinkToFit(AssociationMap& subassoc_map)
  {
    bool anyChanges = false;

    if (subassoc_map.empty())
      return true;

    for (auto it = subassoc_map.begin(); it != subassoc_map.end();)
    {
      auto& assoc = it->first;
      auto& assocStruct = it->second;

      if (assocStruct.SubAssociation)
      {
        if (INIParser::ShrinkToFit(*assocStruct.SubAssociation))
        {
          if (assocStruct.SubAssociation->empty())
          {
            assocStruct.SubAssociation.reset();
            anyChanges = true;
          }
        }
      }

      if (!assocStruct.SubAssociation && !assocStruct.ParseCouple)
      {
        it = subassoc_map.erase(it);
        anyChanges = true;
      }
      else
        it++;
    }

    return anyChanges;
  }
}
