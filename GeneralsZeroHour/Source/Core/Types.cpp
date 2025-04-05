#include <SAGE.h>

#include "Core/Types.h"

namespace genzh
{
  namespace INI
  {
    void* ParseTerrainLOD(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing TerrainLOD"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string terrainLODToken = val.substr(spacePos);
      terrainLODToken = terrainLODToken.substr(0, terrainLODToken.find_first_of(" =\n\r\t"));

      TerrainLOD* terrainLODValue = nullptr;

      if (terrainLODToken == "NONE")
        terrainLODValue = new TerrainLOD(TerrainLOD::Invalid);
      else if (terrainLODToken == "MIN")
        terrainLODValue = new TerrainLOD(TerrainLOD::Min);
      else if (terrainLODToken == "STRETCH_NO_CLOUDS")
        terrainLODValue = new TerrainLOD(TerrainLOD::StretchNoClouds);
      else if (terrainLODToken == "HALF_CLOUDS")
        terrainLODValue = new TerrainLOD(TerrainLOD::HalfClouds);
      else if (terrainLODToken == "NO_CLOUDS")
        terrainLODValue = new TerrainLOD(TerrainLOD::NoClouds);
      else if (terrainLODToken == "STRETCH_CLOUDS")
        terrainLODValue = new TerrainLOD(TerrainLOD::StretchClouds);
      else if (terrainLODToken == "NO_WATER")
        terrainLODValue = new TerrainLOD(TerrainLOD::NoWater);
      else if (terrainLODToken == "MAX")
        terrainLODValue = new TerrainLOD(TerrainLOD::Max);
      else if (terrainLODToken == "AUTOMATIC")
        terrainLODValue = new TerrainLOD(TerrainLOD::Automatic);
      else if (terrainLODToken == "DISABLE")
        terrainLODValue = new TerrainLOD(TerrainLOD::Disable);

      SAGE_ASSERT(terrainLODValue, "[SYSTEM] No valid TerrainLOD token has been found. Invalid token: '{}'", terrainLODToken);

      return (void*)terrainLODValue;
    }

    void* ParseTimeOfDay(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing TimeOfDay"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string timeOfDayToken = val.substr(spacePos);
      timeOfDayToken = timeOfDayToken.substr(0, timeOfDayToken.find_first_of(" =\n\r\t"));

      TimeOfDay* timeOfDayValue = nullptr;

      if (timeOfDayToken == "NONE")
        timeOfDayValue = new TimeOfDay(TimeOfDay::Invalid);
      else if (timeOfDayToken == "MORNING")
        timeOfDayValue = new TimeOfDay(TimeOfDay::Morning);
      else if (timeOfDayToken == "AFTERNOON")
        timeOfDayValue = new TimeOfDay(TimeOfDay::Afternoon);
      else if (timeOfDayToken == "EVENING")
        timeOfDayValue = new TimeOfDay(TimeOfDay::Evening);
      else if (timeOfDayToken == "NIGHT")
        timeOfDayValue = new TimeOfDay(TimeOfDay::Night);

      SAGE_ASSERT(timeOfDayValue, "[SYSTEM] No valid TimeOfDay token has been found. Invalid token: '{}'", timeOfDayToken);

      return (void*)timeOfDayValue;
    }

    void* ParseWeather(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing Weather"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string weatherToken = val.substr(spacePos);
      weatherToken = weatherToken.substr(0, weatherToken.find_first_of(" =\n\r\t"));

      Weather* weatherValue = nullptr;

      if (weatherToken == "NORMAL")
        weatherValue = new Weather(Weather::Normal);
      else if (weatherToken == "SNOWY")
        weatherValue = new Weather(Weather::Snowy);

      SAGE_ASSERT(weatherValue, "[SYSTEM] No valid Weather token has been found. Invalid token: '{}'", weatherToken);

      return (void*)weatherValue;
    }

    void* ParseBodyDamageState(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing BodyDamageState"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string bodyDamageStateToken = val.substr(spacePos);
      bodyDamageStateToken = bodyDamageStateToken.substr(0, bodyDamageStateToken.find_first_of(" =\n\r\t"));

      BodyDamageState* bodyDamageStateValue = nullptr;

      if (bodyDamageStateToken == "PRISTINE")
        bodyDamageStateValue = new BodyDamageState(BodyDamageState::Pristine);
      else if (bodyDamageStateToken == "DAMAGED")
        bodyDamageStateValue = new BodyDamageState(BodyDamageState::Damaged);
      else if (bodyDamageStateToken == "REALLYDAMAGED")
        bodyDamageStateValue = new BodyDamageState(BodyDamageState::ReallyDamaged);
      else if (bodyDamageStateToken == "RUBBLE")
        bodyDamageStateValue = new BodyDamageState(BodyDamageState::Rubble);

      SAGE_ASSERT(bodyDamageStateValue, "[SYSTEM] No valid BodyDamageState token has been found. Invalid token: '{}'", bodyDamageStateToken);

      return (void*)bodyDamageStateValue;
    }

    void* ParseFloatAcceleration(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing a float acceleration"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string floatAccelerationToken = val.substr(spacePos);
      floatAccelerationToken = floatAccelerationToken.substr(0, floatAccelerationToken.find_first_of(" =\n\r\t"));

      float* floatAccelerationValue = new float(std::stof(floatAccelerationToken) * (1.0f / FPS) * (1.0f / FPS)); // (parse acceleration in (dist/sec^2) and convert to (dist/frame^2)) FPS needs to be obtained from a source not a define, maybe should also tansform this conversion into a function in utils namespace @TODO
      return (void*)floatAccelerationValue;
    }

    void* ParseUInt32Duration(const std::string& val)
    {
      size_t spacePos = val.find_first_not_of(" =\r\n\t");
      SAGE_ASSERT(spacePos != std::string::npos, "[SYSTEM] No value read while parsing an unsigned 32 bit integer"); // Hoping nothing breaks here, when no token found 'spacePos' has an extremely high value
      std::string uint32Token = val.substr(spacePos);
      uint32Token = uint32Token.substr(0, uint32Token.find_first_of(" =\n\r\t"));
      SAGE_ASSERT(uint32Token[0] != '-', "[SYSTEM] Encountered a negative value while parsing to an unsigned one");

      uint32_t* uint32Value = new uint32_t(static_cast<uint32_t>(std::ceilf(static_cast<float>(std::stoul(uint32Token)) * (FPS / 1000.0f)))); // (parse suration in milliseconds to frames) FPS needs to be obtained from a source not a define, maybe should also tansform this conversion into a function in utils namespace @TODO
      return (void*)uint32Value;
    }
  }

  void WeaponBonusSet::Add(WeaponBonusCondition condition, WeaponBonusType bonus_type, float bonus)
  {
#if defined(CONFIG_DEBUG) || defined(CONFIG_RELEASE)
    if (m_WeaponBonuses.find(condition) != m_WeaponBonuses.end())
    {
      auto& bonusTypes = m_WeaponBonuses[condition];
      if (bonusTypes.find(bonus_type) != bonusTypes.end())
        SAGE_WARN("[SYSTEM] Overriding an existing WeaponBonusType");
    }
#endif
    m_WeaponBonuses[condition][bonus_type] = bonus;
  }

  std::vector<WeaponBonusDescription> WeaponBonusSet::GetBonuses(WeaponBonusCondition condition)
  {
    SAGE_ASSERT(m_WeaponBonuses.find(condition) != m_WeaponBonuses.end(), "[SYSTEM] Tryng to get WeaponBonusDescriptions from unexisting WeaponBonusCondition")
    std::vector<WeaponBonusDescription> bonusDescriptions;

    for (const auto& bonusDescription : m_WeaponBonuses[condition])
      bonusDescriptions.push_back({ bonusDescription.first, bonusDescription.second });

    return bonusDescriptions;
  }
}