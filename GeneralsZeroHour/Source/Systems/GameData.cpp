#include <pch.h>

#include <optional>
#include <iostream>
#include <vector>

#include "Systems/GameData.h"

namespace genzh
{
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

    return nullptr;
  }

  void* GameData::ParsePublicBone(const std::string& val)
  {
    size_t spacePos = val.find_first_not_of(" =\r\n\t");
    SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a PublicBone"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
    std::string publicBoneToken = val.substr(spacePos);
    publicBoneToken = publicBoneToken.substr(0, publicBoneToken.find_last_of(" \n\r\t"));

    auto& bones = GameData::Get().s_StandardPublicBones;
    bones.push_back(publicBoneToken);

    return nullptr;
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