#ifndef _GLOBAL_CONFIG_H
#define _GLOBAL_CONFIG_H

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class GlobalConfig {
public:
  static GlobalConfig &getInstance() {
    static GlobalConfig instance;
    return instance;
  }

  template <typename T> T getConfig(const std::string &key) {
    auto it = config.find(key);
    if (it != config.end()) {
      std::istringstream is(it->second);
      T value;
      if (!(is >> value)) {
        throw std::runtime_error("Invalid conversion");
      }
      return value;
    }
    throw std::runtime_error("Key not found");
  }

private:
  std::map<std::string, std::string> config;

  GlobalConfig() {
    std::ifstream configFile("config.txt");
    if (configFile.is_open()) {
      std::string line;
      while (getline(configFile, line)) {
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
          std::string value;
          if (std::getline(is_line, value)) {
            config[key] = value;
          }
        }
      }
      configFile.close();
    } else {
      // Creating default config file
      std::ofstream newConfigFile("config.txt");
      if (newConfigFile.is_open()) {
 std::vector<std::string> defaultConfig = {
  "MapWidth=100",
  "MapHeight=100",
  "BoardRectLeft=0",
  "BoardRectTop=0",
  "BoardRectBottom=0.75",
  "BoardRectRight=0.75",
  "MessageDisplayRectLeft=0",
  "MessageDisplayRectTop=0.75",
  "MessageDisplayRectBottom=1",
  "MessageDisplayRectRight=1",
  "StatsRectLeft=0.75",
  "StatsRectTop=0",
  "StatsRectBottom=1",
  "StatsRectRight=1",
  "PlayerHealth=300",
  "PlayerDamage=100",
  "MonsterUpdateSpeed=360",
  "GoblinsCount=50",
  "GoblinHealth=100",
  "GoblinDamage=30",
  "OrcsCount=20",
  "OrcHealth=200",
  "OrcDamage=30",
  "TrollsCount=10",
  "TrollHealth=300",
  "TrollDamage=50",
  "DragonsCount=10",
  "DragonHealth=400",
  "DragonDamage=100",
  "MessageQueueSize=20",
  "EmptySymbol=32",
  "WallSymbol=#",
  "PlayerSymbol=@",
  "GoblinSymbol=g",
  "OrcSymbol=o",
  "DragonSymbol=D",
  "TrollSymbol=T",
  "StartSymbol=S",
  "EndSymbol=❎",
  "TreasureSymbol=*",
  "BonusValue=50",
  "BonusExpirationCounter=100"
};


        for (const auto &entry : defaultConfig) {
          newConfigFile << entry << "\n";
        }

        newConfigFile.close();
        std::cout << "Default config file created" << std::endl;
      } else {
        std::cerr << "Unable to create config file" << std::endl;
      }
    }
  }

  GlobalConfig(GlobalConfig const &) = delete;
  void operator=(GlobalConfig const &) = delete;
};


#endif 
