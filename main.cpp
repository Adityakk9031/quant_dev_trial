#include <iostream>
#include <vector>
#include <algorithm>
#include "parser.hpp"
#include "order_book.hpp"
#include "mbp_writer.hpp"
#include "types.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_mbo.csv> <output_mbp.csv>\n";
        return 1;
    }

    try {
        auto records = parseMBOCSV(argv[1]);
        std::cout << "Parsed " << records.size() << " MBO records.\n";

        OrderBook book;
        std::vector<MBPRecord> mbp_snapshots;
        size_t SNAPSHOT_INTERVAL = 584;
        size_t record_count = 0;

        for (const auto& record : records) {
            ++record_count;

            if (record_count % SNAPSHOT_INTERVAL == 0) {
                MBPRecord snapshot;
                snapshot.ts_event = record.ts_event;
                snapshot.symbol = record.symbol;

                const auto& bids = book.getBids();
                const auto& asks = book.getAsks();

                size_t bid_limit = std::min<size_t>(10, bids.size());
                size_t ask_limit = std::min<size_t>(10, asks.size());

                auto itb = bids.begin();
                for (size_t i = 0; i < bid_limit && itb != bids.end(); ++i, ++itb)
                    snapshot.bids[itb->first] = itb->second;

                auto ita = asks.begin();
                for (size_t i = 0; i < ask_limit && ita != asks.end(); ++i, ++ita)
                    snapshot.asks[ita->first] = ita->second;

                if (!snapshot.bids.empty() || !snapshot.asks.empty()) {
                    std::cout << "[SNAPSHOT] ts=" << snapshot.ts_event
                              << ", symbol=" << snapshot.symbol
                              << ", bids=" << snapshot.bids.size()
                              << ", asks=" << snapshot.asks.size() << "\n";

                    mbp_snapshots.push_back(snapshot);
                } else {
                    std::cout << "[SNAPSHOT] ts=" << snapshot.ts_event << " is empty, skipping\n";
                }

                MBORecord clear_record = record;
                clear_record.action = Action::Clear;
                book.process(clear_record);
                continue;
            }

            book.process(record);
        }

        std::cout << "Writing " << mbp_snapshots.size() << " MBP snapshots.\n";
        writeMBPToCSV(argv[2], mbp_snapshots);

        for (int i = 0; i < std::min(3, static_cast<int>(mbp_snapshots.size())); ++i) {
            const auto& snap = mbp_snapshots[i];
            std::cout << "[DEBUG] ts=" << snap.ts_event << ", symbol=" << snap.symbol
                      << ", bids=" << snap.bids.size() << ", asks=" << snap.asks.size() << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
