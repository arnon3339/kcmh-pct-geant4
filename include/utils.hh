#ifndef UTILS_HH
#define UTILS_HH 1

#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>

namespace kcmh
{
std::vector<std::string> split(std::string s, std::string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
      token = s.substr (pos_start, pos_end - pos_start);
      pos_start = pos_end + delim_len;
      res.push_back (token);
  }

  res.push_back (s.substr (pos_start));
  return res;
}

std::string createOutputDirs(const float& energy, const float& angle)
{
  auto angleStrs = split(std::to_string(angle), ".");
  auto energyStrs = split(std::to_string(energy), ".");
  std::ostringstream ossAngle;
  std::ostringstream ossEnergy;
  ossEnergy << std::setw(3) << std::setfill('0') << energyStrs[0];
  ossAngle << std::setw(3) << std::setfill('0') << angleStrs[0];

  std::string angleDir = "";
  std::string energyDir = "";

  if (energyStrs.size() > 1 && !(atof(energyStrs[1].c_str()) == 0.))
  {
    energyDir = ossEnergy.str() + "." + energyStrs[1];
  }
  else energyDir = ossEnergy.str();

  if (angleStrs.size() > 1 && !(atof(angleStrs[1].c_str()) == 0.))
  {
    angleDir = ossAngle.str() + "." + angleStrs[1];
  }
  else angleDir = ossAngle.str();

  auto outputDir =  "./output/" + energyDir + "/projection_" + angleDir;
  auto outputPath =  outputDir + "/run.root";

  try
  {
    std::filesystem::create_directories(outputDir);
    return outputPath;
  }
  catch (std::filesystem::filesystem_error e)
  {
    std::cerr << "Cannot create output path!" << std::endl;
    exit(EXIT_FAILURE);
  }
}
} // namespace kcmh

#endif