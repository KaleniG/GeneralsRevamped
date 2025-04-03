#include <pch.h>

#include "Engine/INIParser.h"
#include "Engine/FileSystem.h"

namespace sage
{
  namespace INI
  {
    void* ParseBool(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a bool"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string boolToken = val.substr(spacePos);
      boolToken = boolToken.substr(0, boolToken.find_first_of(" =\n\r\t"));

      bool* boolValue = nullptr;
      if (boolToken == "yes" || boolToken == "Yes")
        boolValue = new bool(true);
      else if (boolToken == "no" || boolToken == "No")
        boolValue = new bool(false);
      SAGE_ASSERT(boolValue, "[SYSTEM] Invalid bool parsing token '{}'", boolToken);
      return (void*)boolValue;
    }

    void* ParseFloat(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a float"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string floatToken = val.substr(spacePos);
      floatToken = floatToken.substr(0, floatToken.find_first_of(" =\n\r\t"));

      float* floatValue = new float(std::stof(floatToken));
      return (void*)floatValue;
    }

    void* ParseInt64(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a signed 64 bit integer"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string int64Token = val.substr(spacePos);
      int64Token = int64Token.substr(0, int64Token.find_first_of(" =\n\r\t"));

      int64_t* int64Value = new int64_t(std::stoll(int64Token));
      return (void*)int64Value;
    }

    void* ParseInt32(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a signed 32 bit integer"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string int32Token = val.substr(spacePos);
      int32Token = int32Token.substr(0, int32Token.find_first_of(" =\n\r\t"));

      int32_t* int32Value = new int32_t(std::stoi(int32Token));
      return (void*)int32Value;
    }

    void* ParseInt16(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a signed 16 bit integer"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string int16Token = val.substr(spacePos);
      int16Token = int16Token.substr(0, int16Token.find_first_of(" =\n\r\t"));

      int16_t* int16Value = new int16_t(static_cast<int16_t>(std::stoi(int16Token)));
      return (void*)int16Value;
    }

    void* ParseInt8(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a signed 8 bit integer"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string int8Token = val.substr(spacePos);
      int8Token = int8Token.substr(0, int8Token.find_first_of(" =\n\r\t"));

      int8_t* int8Value = new int8_t(static_cast<int8_t>(std::stoi(int8Token)));
      return (void*)int8Value;
    }

    void* ParseUInt64(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing an unsigned 64 bit integer"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string uint64Token = val.substr(spacePos);
      uint64Token = uint64Token.substr(0, uint64Token.find_first_of(" =\n\r\t"));
      SAGE_ASSERT(uint64Token[0] != '-', "[SYSTEM] Encountered a negative value while parsing to an unsigned one");

      uint64_t* uint64Value = new uint64_t(std::stoull(uint64Token));
      return (void*)uint64Value;
    }

    void* ParseUInt32(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing an unsigned 32 bit integer"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string uint32Token = val.substr(spacePos);
      uint32Token = uint32Token.substr(0, uint32Token.find_first_of(" =\n\r\t"));
      SAGE_ASSERT(uint32Token[0] != '-', "[SYSTEM] Encountered a negative value while parsing to an unsigned one");

      uint32_t* uint32Value = new uint32_t(static_cast<uint32_t>(std::stoul(uint32Token)));
      return (void*)uint32Value;
    }

    void* ParseUInt16(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing an unsigned 16 bit integer"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string uint16Token = val.substr(spacePos);
      uint16Token = uint16Token.substr(0, uint16Token.find_first_of(" =\n\r\t"));
      SAGE_ASSERT(uint16Token[0] != '-', "[SYSTEM] Encountered a negative value while parsing to an unsigned one");

      uint16_t* uint16Value = new uint16_t(static_cast<uint16_t>(std::stoul(uint16Token)));
      return (void*)uint16Value;
    }

    void* ParseUInt8(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing an unsigned 8 bit integer"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string uint8Token = val.substr(spacePos);
      uint8Token = uint8Token.substr(0, uint8Token.find_first_of(" =\n\r\t"));
      SAGE_ASSERT(uint8Token[0] != '-', "[SYSTEM] Encountered a negative value while parsing to an unsigned one");

      uint8_t* uint8Value = new uint8_t(static_cast<uint8_t>(std::stoul(uint8Token)));
      return (void*)uint8Value;
    }

    void* ParseString(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a string"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string stringToken = val.substr(spacePos);
      stringToken = stringToken.substr(0, stringToken.find_first_of(" =\n\r\t"));

      std::string* stringValue = new std::string(stringToken);
      return (void*)stringValue;
    }
  }

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
