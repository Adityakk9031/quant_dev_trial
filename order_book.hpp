#pragma once
#include <map>
#include <unordered_map>
#include "types.hpp"

class OrderBook {
public:
    void process(const MBORecord& record);
    const std::map<int64_t, uint32_t, std::greater<>>& getBids() const;
    const std::map<int64_t, uint32_t>& getAsks() const;

private:
    std::map<int64_t, uint32_t, std::greater<>> bids;
    std::map<int64_t, uint32_t> asks;
    std::unordered_map<uint64_t, MBORecord> order_map;
};