#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

std::vector<MBORecord> parseMBOCSV(const std::string& input_path) {
    std::ifstream file(input_path);
    if (!file.is_open()) throw std::runtime_error("Failed to open input file");

    std::vector<MBORecord> records;
    std::string line;
    std::getline(file, line);  // Skip header

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        MBORecord rec;
        std::string field;

        std::getline(ss, rec.ts_recv, ',');
        std::getline(ss, rec.ts_event, ',');  

        std::getline(ss, field, ','); rec.rtype = std::stoi(field);
        std::getline(ss, field, ','); rec.publisher_id = std::stoull(field);
        std::getline(ss, field, ','); rec.instrument_id = std::stoull(field);
        std::getline(ss, field, ',');  
        char action_char = field.empty() ? 'R' : field[0];
        switch (action_char) {
            case 'A': rec.action = Action::Add; break;
            case 'C': rec.action = Action::Cancel; break;
            case 'F': rec.action = Action::Fill; break;
            case 'M': rec.action = Action::Modify; break;
            case 'R': rec.action = Action::Clear; break;
            default: rec.action = Action::Add;
        }

        std::getline(ss, field, ',');  
        rec.side = (field == "B" ? Side::Bid : Side::Ask);

        std::getline(ss, field, ',');  
        rec.price = field.empty() ? 0 : static_cast<int64_t>(std::stod(field) * 1000);

        std::getline(ss, field, ',');  
        rec.size = field.empty() ? 0 : std::stoul(field);

        std::getline(ss, field, ','); rec.channel_id = std::stoull(field);
        std::getline(ss, field, ','); rec.order_id = std::stoull(field);
        std::getline(ss, field, ','); rec.flags = std::stoul(field);
        std::getline(ss, field, ','); rec.ts_in_delta = std::stoull(field);
        std::getline(ss, field, ','); rec.sequence = std::stoull(field);
        std::getline(ss, rec.symbol, ',');

        records.push_back(rec);
    }

    return records;
}
