#pragma once

#include <string>
#include <map>
#include <cstdint>
#include <vector>
#include <functional>  


enum class Side { Bid, Ask };


enum class Action { Add, Cancel, Fill, Modify, Clear };


struct MBORecord {
    std::string ts_event;  
    std::string ts_recv;   
    std::string symbol;
    Side side = Side::Bid;
    Action action = Action::Add;
    int64_t price = 0;
    uint32_t size = 0;
    uint64_t publisher_id = 0;
    uint64_t instrument_id = 0;
    uint64_t channel_id = 0;
    uint64_t order_id = 0;
    uint32_t flags = 0;
    uint64_t ts_in_delta = 0;
    uint64_t sequence = 0;
    uint8_t rtype = 0;
};


struct MBPRecord {
    std::string ts_event; 
    std::string symbol;
    std::map<int64_t, uint32_t, std::greater<>> bids; 
    std::map<int64_t, uint32_t> asks;                 
};
