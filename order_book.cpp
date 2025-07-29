#include "order_book.hpp"
#include <algorithm>
#include <iostream>

void OrderBook::process(const MBORecord& record) {
    std::cout << "[ORDER APPLY] action=" << static_cast<int>(record.action)
              << " side=" << (record.side == Side::Bid ? "B" : "A")
              << " price=" << record.price << " size=" << record.size << "\n";

    switch (record.action) {
        case Action::Add:
            order_map[record.order_id] = record;
            if (record.side == Side::Bid) bids[record.price] += record.size;
            else asks[record.price] += record.size;
            break;

        case Action::Cancel:
        case Action::Fill: {
            auto it = order_map.find(record.order_id);
            if (it != order_map.end()) {
                const auto& prev = it->second;
                if (prev.side == Side::Bid) {
                    uint32_t reduction = std::min(bids[prev.price], record.size);
                    bids[prev.price] -= reduction;
                    if (bids[prev.price] == 0) bids.erase(prev.price);
                } else {
                    uint32_t reduction = std::min(asks[prev.price], record.size);
                    asks[prev.price] -= reduction;
                    if (asks[prev.price] == 0) asks.erase(prev.price);
                }
                if (record.action == Action::Cancel || record.size >= prev.size)
                    order_map.erase(it);
            }
            break;
        }

        case Action::Modify: {
            auto it = order_map.find(record.order_id);
            if (it != order_map.end()) {
                const auto& prev = it->second;
                if (prev.side == Side::Bid) {
                    bids[prev.price] -= prev.size;
                    if (bids[prev.price] == 0) bids.erase(prev.price);
                } else {
                    asks[prev.price] -= prev.size;
                    if (asks[prev.price] == 0) asks.erase(prev.price);
                }
                order_map[record.order_id] = record;
                if (record.side == Side::Bid) bids[record.price] += record.size;
                else asks[record.price] += record.size;
            }
            break;
        }

        case Action::Clear:
            order_map.clear();
            bids.clear();
            asks.clear();
            break;

        default:
            std::cerr << "Unknown action for order_id " << record.order_id << ", skipping.\n";
            break;
    }
}

const std::map<int64_t, uint32_t, std::greater<>>& OrderBook::getBids() const {
    return bids;
}

const std::map<int64_t, uint32_t>& OrderBook::getAsks() const {
    return asks;
}
