#include "mbp_writer.hpp"
#include <fstream>
#include <stdexcept>

void writeMBPToCSV(const std::string& output_path, const std::vector<MBPRecord>& snapshots) {
    std::ofstream out(output_path);
    if (!out.is_open()) throw std::runtime_error("Failed to open output file");

    out << "ts_event,symbol,side,price,size\n";
    for (const auto& snap : snapshots) {
        for (const auto& bid : snap.bids) {
            out << snap.ts_event << "," << snap.symbol << ",Bid," << (bid.first / 1000.0) << "," << bid.second << "\n";
        }
        for (const auto& ask : snap.asks) {
            out << snap.ts_event << "," << snap.symbol << ",Ask," << (ask.first / 1000.0) << "," << ask.second << "\n";
        }
    }
}
