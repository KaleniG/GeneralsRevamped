#include <pch.h>

#include <algorithm>
#include <optional>
#include <iostream>
#include <vector>

#include "Systems/GameData.h"

namespace genzh
{
  namespace INI
  {
    void* ParseGamma(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a float"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string gammaToken = val.substr(spacePos);
      gammaToken = gammaToken.substr(0, gammaToken.find_first_of(" =\n\r\t"));

      float* gammaValue = new float(std::stof(gammaToken));

      if (*gammaValue <= 0)
        *gammaValue = 0.6f;
      else if (*gammaValue < 50.0f)
        *gammaValue = 1.0f - 0.4f * (50.0f - *gammaValue) / 50.0f;
      else if (*gammaValue > 50)
        *gammaValue = 1.0f + 1.0f * (*gammaValue - 50.0f) / 50.0f;
      else
        *gammaValue = 1.0f;

      return (void*)gammaValue;
    }

    void* ParseResolution(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing resolution vector of two signed 32 bit integers"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string resolutionToken = val.substr(spacePos);

      glm::ivec2* resolutionValue = new glm::ivec2();

      spacePos = resolutionToken.find_first_of(" \r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No 'X' resolution token found while parsing resolution vector of two signed 32 bit integers");
      std::string xToken = resolutionToken.substr(0, spacePos);
      resolutionValue->x = std::stoi(xToken);
      resolutionToken = resolutionToken.substr(spacePos);

      spacePos = resolutionToken.find_first_of(" \r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No 'Y' resolution token found while parsing resolution vector of two signed 32 bit integers");
      std::string yToken = resolutionToken.substr(spacePos);
      resolutionValue->y = std::stoi(yToken);

      return (void*)resolutionValue;
    }

    void* ParseScrollFactor(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a scroll factor that is an signed 32 bit integer"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string scrollFactorToken = val.substr(spacePos);
      scrollFactorToken = scrollFactorToken.substr(0, scrollFactorToken.find_first_of(" =\n\r\t"));

      int32_t scrollFactorValue = std::clamp(std::stoi(scrollFactorToken), 0, 100);

      float* scrollFactorFloatValue = new float(static_cast<float>(scrollFactorValue) / 100.0f);

      return (void*)scrollFactorFloatValue;
    }

    std::string SerializeGamma(void* val)
    {
      SAGE_ASSERT(val, "[SYSTEM] Invalid pointer to a gamma float value");

      std::string gammaString;
      gammaString.append(" = ");

      float* gammaValue = static_cast<float*>(val);

      int32_t serializeGammaValue = 50;

      if (*gammaValue < 1.0f)
        serializeGammaValue = static_cast<int>(50 - ((1.0f - *gammaValue) * 50.0f / 0.4f));
      else if (*gammaValue > 1.0f)
        serializeGammaValue = static_cast<int>(50 + ((*gammaValue - 1.0f) * 50.0f));

      serializeGammaValue = std::clamp(serializeGammaValue, 0, 100);

      gammaString.append(std::to_string(serializeGammaValue));

      return gammaString;
    }

    std::string SerializeResolution(void* val)
    {
      SAGE_ASSERT(val, "[SYSTEM] Invalid pointer to a resolution vector of two 32 bit integers");

      std::string resolutionString;
      resolutionString.append(" = ");

      glm::ivec2* resolutionValue = static_cast<glm::ivec2*>(val);

      resolutionString.append(std::to_string(resolutionValue->x));
      resolutionString.append(" ");
      resolutionString.append(std::to_string(resolutionValue->y));

      return resolutionString;
    }

    std::string SerializeScrollFactor(void* val)
    {
      SAGE_ASSERT(val, "[SYSTEM] Invalid pointer to an scroll factor float value");

      std::string scrollFactorString;
      scrollFactorString.append(" = ");

      float* scrollFactorValue = static_cast<float*>(val);
      int32_t scrollFactorInt32Value = static_cast<int32_t>(std::clamp(*scrollFactorValue, 0.0f, 100.0f) * 100.0f);

      scrollFactorString.append(std::to_string(scrollFactorInt32Value));

      return scrollFactorString;
    }
  }

  void* GameData::ParseWeaponBonus(const std::string& val)
  {
    size_t spacePos = val.find_first_not_of(" =\r\n\t");
    SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing WeaponBonus"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
    std::string weaponBonusDescriptionToken = val.substr(spacePos);
    weaponBonusDescriptionToken = weaponBonusDescriptionToken.substr(0, weaponBonusDescriptionToken.find_last_of(" =\n\r\t"));

    spacePos = weaponBonusDescriptionToken.find_first_of(" ");
    std::string weaponBonusConditionToken = weaponBonusDescriptionToken.substr(0, spacePos);
    weaponBonusDescriptionToken = weaponBonusDescriptionToken.substr(spacePos);

    spacePos = weaponBonusDescriptionToken.find_first_not_of(" ");
    std::string weaponBonusTypeToken = weaponBonusDescriptionToken.substr(spacePos);
    weaponBonusDescriptionToken = weaponBonusDescriptionToken.substr(spacePos);
    spacePos = weaponBonusDescriptionToken.find_first_of(" ");
    weaponBonusTypeToken = weaponBonusTypeToken.substr(0, spacePos);
    weaponBonusDescriptionToken = weaponBonusDescriptionToken.substr(spacePos);

    spacePos = weaponBonusDescriptionToken.find_first_not_of(" ");
    SAGE_ASSERT(weaponBonusDescriptionToken.find_first_of("%") != std::string::npos, "[SYSTEM] Trying to parse a percentage value but the '%' character is missing");
    std::string weaponBonusToken = weaponBonusDescriptionToken.substr(spacePos);
    weaponBonusToken = weaponBonusToken.substr(0, weaponBonusToken.find("%"));

    WeaponBonusSet& set = GameData::Get().s_WeaponBonusSet;

    std::optional<WeaponBonusCondition> weaponBonusConditionValue;

    if (weaponBonusConditionToken == "GARRISONED")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::Garrisoned);
    else if (weaponBonusConditionToken == "HORDE")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::Horde);
    else if (weaponBonusConditionToken == "CONTINUOUS_FIRE_MEAN")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::ContinuousFireMean);
    else if (weaponBonusConditionToken == "CONTINUOUS_FIRE_FAST")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::ContinuousFireFast);
    else if (weaponBonusConditionToken == "NATIONALISM")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::Nationalism);
    else if (weaponBonusConditionToken == "FANATICISM") // the shit about fanaticism that there is not even a trace of it in the codebase, but there is in the .ini file, this means or the coeebase is outdated or that i'm shitless @TODO
      weaponBonusConditionValue.emplace(WeaponBonusCondition::Fanaticism);
    else if (weaponBonusConditionToken == "PLAYER_UPGRADE")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::PlayerUpgrade);
    else if (weaponBonusConditionToken == "DRONE_SPOTTING")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::DroneSpotting);
    else if (weaponBonusConditionToken == "ENTHUSIASTIC")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::Enthusiastic);
    else if (weaponBonusConditionToken == "TARGET_FAERIE_FIRE") // what the hell is it, no trace in the codebase
      weaponBonusConditionValue.emplace(WeaponBonusCondition::TargetFaerieFire); // had to add a new enum just for it
    else if (weaponBonusConditionToken == "VETERAN")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::Veteran);
    else if (weaponBonusConditionToken == "ELITE")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::Elite);
    else if (weaponBonusConditionToken == "HERO")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::Hero);
    else if (weaponBonusConditionToken == "BATTLEPLAN_BOMBARDMENT")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::BattleplanBombardment);
    else if (weaponBonusConditionToken == "BATTLEPLAN_HOLDTHELINE")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::BattleblanHoldTheLine);
    else if (weaponBonusConditionToken == "BATTLEPLAN_SEARCHANDDESTROY")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::BattleplaySearchAndDestroy);
    else if (weaponBonusConditionToken == "SUBLIMINAL")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::SubliminalMessaging);
    else if (weaponBonusConditionToken == "FRENZY_ONE") // The real one, like some other, absent from codebase, same for the other two below
      weaponBonusConditionValue.emplace(WeaponBonusCondition::FrenzyOne);
    else if (weaponBonusConditionToken == "FRENZY_TWO") //
      weaponBonusConditionValue.emplace(WeaponBonusCondition::FrenzyTwo);
    else if (weaponBonusConditionToken == "FRENZY_THREE") //
      weaponBonusConditionValue.emplace(WeaponBonusCondition::FrenzyThree);
    else if (weaponBonusConditionToken == "SOLO_HUMAN_EASY")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::SoloHumanEasy);
    else if (weaponBonusConditionToken == "SOLO_HUMAN_NORMAL")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::SoloHumanNormal);
    else if (weaponBonusConditionToken == "SOLO_HUMAN_HARD")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::SoloHumanHard);
    else if (weaponBonusConditionToken == "SOLO_AI_EASY")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::SoloAIEasy);
    else if (weaponBonusConditionToken == "SOLO_AI_NORMAL")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::SoloAINormal);
    else if (weaponBonusConditionToken == "SOLO_AI_HARD")
      weaponBonusConditionValue.emplace(WeaponBonusCondition::SoloAiHard);

    SAGE_ASSERT(weaponBonusConditionValue.has_value(), "[SYSTEM] WeaponBonusCondition is invalid. Invalid value: '{}'", weaponBonusConditionToken);

    std::optional<WeaponBonusType> weaponBonusTypeValue;

    if (weaponBonusTypeToken == "DAMAGE")
      weaponBonusTypeValue.emplace(WeaponBonusType::Damage);
    else if (weaponBonusTypeToken == "RADIUS")
      weaponBonusTypeValue.emplace(WeaponBonusType::Radius);
    else if (weaponBonusTypeToken == "RANGE")
      weaponBonusTypeValue.emplace(WeaponBonusType::Range);
    else if (weaponBonusTypeToken == "RATE_OF_FIRE")
      weaponBonusTypeValue.emplace(WeaponBonusType::RateOfFire);
    else if (weaponBonusTypeToken == "PRE_ATTACK")
      weaponBonusTypeValue.emplace(WeaponBonusType::PreAttack);

    SAGE_ASSERT(weaponBonusTypeValue.has_value(), "[SYSTEM] WeaponBonusType is invalid. Invalid value: '{}'", weaponBonusTypeToken);

    float weaponBonusValue = std::stof(weaponBonusToken) / 100.0f;

    set.Add(weaponBonusConditionValue.value(), weaponBonusTypeValue.value(), weaponBonusValue);

    return (void*)&set;
  }

  void* GameData::ParsePublicBone(const std::string& val)
  {
    size_t spacePos = val.find_first_not_of(" =\r\n\t");
    SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a PublicBone"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
    std::string publicBoneToken = val.substr(spacePos);
    publicBoneToken = publicBoneToken.substr(0, publicBoneToken.find_last_of(" \n\r\t"));

    auto& bones = GameData::Get().s_StandardPublicBones;
    bones.push_back(publicBoneToken);

    return (void*)&bones;
  }

  GameData::GameData()
  {
    for (int32_t i = 0; i < MAX_GLOBAL_LIGHTS; i++)
    {
      s_TerrainLightPos->z = -1.0f;
      for (int32_t j = 0; j < TIME_OF_DAY_COUNT; j++)
      {
        s_TerrainLighting[j][i].LightPosition.z = -1.0f;
        s_TerrainObjectsLighting[j][i].LightPosition.z = -1.0f;
      }
    }
  }
}