// Temporary Header
#pragma once

#include <unordered_map>
#include <type_traits>
#include <cstdint>
#include <string>

#include <glm/glm.hpp>

#define MAX_WATER_GRID_SETTINGS 4
#define TIME_OF_DAY_COUNT 4
#define MAX_GLOBAL_LIGHTS 3
#define PLAYERTYPE_COUNT 2
#define DIFFICULTY_COUNT 3
#define LEVEL_COUNT 3 /*-1, the regualr level isn't counted just in this case bruh*/ 
#define FPS 60.0f

namespace genzh
{
  namespace INI
  {
    void* ParseTerrainLOD(const std::string& val);
    void* ParseTimeOfDay(const std::string& val);
    void* ParseWeather(const std::string& val);
    void* ParseBodyDamageState(const std::string& val);
    void* ParseFloatAcceleration(const std::string& val);
    void* ParseUInt32Duration(const std::string& val);
  }

  enum class TerrainLOD : uint32_t
  {
    Invalid,
    Min,
    StretchNoClouds,
    HalfClouds,
    NoClouds,
    StretchClouds,
    NoWater,
    Max,
    Automatic,
    Disable
  };

  enum class TimeOfDay
  {
    Invalid,
    Morning,
    Afternoon,
    Evening,
    Night
  };

  enum class Weather
  {
    Normal,
    Snowy
  };

  struct TerrainLighting
  {
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 LightPosition;
  };

  enum class BodyDamageState
  {
    Pristine,
    Damaged,
    ReallyDamaged,
    Rubble
  };

  enum class WeaponBonusType
  {
    Damage,
    Radius,
    Range,
    RateOfFire,
    PreAttack
  };

  enum class WeaponBonusCondition
  {
    Garrisoned,
    Horde,
    ContinuousFireMean,
    ContinuousFireFast,
    Nationalism,
    Fanaticism,
    PlayerUpgrade,
    DroneSpotting,
    Enthusiastic,
    TargetFaerieFire,
    Veteran,
    Elite,
    Hero,
    BattleplanBombardment,
    BattleblanHoldTheLine,
    BattleplaySearchAndDestroy,
    SubliminalMessaging,
    FrenzyOne,
    FrenzyTwo,
    FrenzyThree,
    SoloHumanEasy,
    SoloHumanNormal,
    SoloHumanHard,
    SoloAIEasy,
    SoloAINormal,
    SoloAiHard
  };

  struct WeaponBonusDescription
  {
    WeaponBonusType Type;
    float Bonus;
  };

  class WeaponBonusSet
  {
  public:
    WeaponBonusSet() = default;
    void Add(WeaponBonusCondition condition, WeaponBonusType bonus_type, float bonus);
    std::vector<WeaponBonusDescription> GetBonuses(WeaponBonusCondition condition);

  private:
    std::unordered_map<WeaponBonusCondition, std::unordered_map<WeaponBonusType, float>> m_WeaponBonuses; // for better case should be an array since the number of WeaponBonuseTypes are known (ex: std::array<WeaponBonusCondition, std::unordered_map<WeaponBonusType, float>, 21>), but keeping it flexible just for the sake of it
  };

  enum class AIDebugOptions
  {
    None,
    Paths,
    Terrain,
    Vells,
    GroundPaths
  };

  enum class FirewallBehavoirFlags : uint32_t
  {
    Unknown = 0,
    Simple = 1 << 0,
    DumbMangling = 1 << 1,
    SmartMangling = 1 << 2,
    NetgearBug = 1 << 3,
    SimplePortAllocation = 1 << 4,
    RelativePortAllocation = 1 << 5,
    DestinationPortDelta = 1 << 6
  };

  inline constexpr uint32_t operator|(FirewallBehavoirFlags a, FirewallBehavoirFlags b)
  {
    return static_cast<uint32_t>(a) | static_cast<uint32_t>(b);
  }

  inline constexpr uint32_t operator&(FirewallBehavoirFlags a, FirewallBehavoirFlags b)
  {
    return static_cast<uint32_t>(a) & static_cast<uint32_t>(b);
  }

  inline constexpr uint32_t operator~(FirewallBehavoirFlags a)
  {
    return ~static_cast<uint32_t>(a);
  }

  inline constexpr uint32_t operator|(uint32_t flags, FirewallBehavoirFlags flag)
  {
    return flags | static_cast<uint32_t>(flag);
  }

  inline constexpr uint32_t operator&(uint32_t flags, FirewallBehavoirFlags flag)
  {
    return flags & static_cast<uint32_t>(flag);
  }

  inline constexpr uint32_t& operator|=(uint32_t& flags, FirewallBehavoirFlags flag)
  {
    flags |= static_cast<uint32_t>(flag);
    return flags;
  }

  inline constexpr uint32_t& operator&=(uint32_t& flags, FirewallBehavoirFlags flag)
  {
    flags &= static_cast<uint32_t>(flag);
    return flags;
  }

  inline constexpr bool HasFlag(uint32_t flags, FirewallBehavoirFlags flag)
  {
    return (flags & flag) != 0;
  }
}