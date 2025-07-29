#pragma once
#include <string>
#include <vector>
#include "types.hpp"

std::vector<MBORecord> parseMBOCSV(const std::string& input_path);
