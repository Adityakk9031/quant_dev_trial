#pragma once
#include <string>
#include <vector>
#include "types.hpp"

void writeMBPToCSV(const std::string& output_path, const std::vector<MBPRecord>& snapshots);
