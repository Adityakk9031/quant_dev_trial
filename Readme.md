<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>MBP Snapshot Generator</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 40px;
      line-height: 1.6;
      background-color: #f9f9f9;
    }
    h1, h2, h3 {
      color: #2c3e50;
    }
    code {
      background-color: #eee;
      padding: 2px 4px;
      border-radius: 4px;
      font-family: monospace;
    }
    pre {
      background-color: #efefef;
      padding: 10px;
      overflow-x: auto;
      border-left: 4px solid #3498db;
    }
    ul {
      margin-top: 0;
    }
  </style>
</head>
<body>

<h1>Market-by-Price Snapshot Generator from Market-by-Order Data</h1>

<h2>📌 Overview</h2>
<p>
  This project reconstructs <strong>Market-by-Price (MBP-10)</strong> snapshots from <strong>Market-by-Order (MBO)</strong> data.
  It maintains an in-memory order book and generates snapshots of the top 10 levels on both bid and ask sides at specific intervals.
</p>
<p>
  This task was completed as part of a technical evaluation for a <strong>Quantitative Developer</strong> position.
</p>

<h2>📁 Project Structure</h2>
<pre><code>.
├── main.cpp               # Program entry point
├── parser.hpp / .cpp      # MBO CSV parser
├── order_book.hpp / .cpp  # Order book state management
├── mbp_writer.hpp / .cpp  # MBP snapshot writer
├── types.hpp              # Enums and type definitions
├── mbo.csv                # Input: MBO records
├── mbp.csv                # Output: MBP snapshots
└── README.html            # Project documentation
</code></pre>

<h2>⚙️ Build & Run Instructions</h2>

<h3>🔧 Prerequisites</h3>
<ul>
  <li>C++17 or higher</li>
  <li>Standard build toolchain (e.g., <code>g++</code>, <code>make</code>, or <code>CMake</code>)</li>
</ul>

<h3>🛠️ Build (Using g++)</h3>
<pre><code>g++ -std=c++17 main.cpp parser.cpp order_book.cpp mbp_writer.cpp -o mbp_reconstructor</code></pre>

<h3>🚀 Run</h3>
<pre><code>./mbp_reconstructor mbo.csv mbp.csv</code></pre>

<h2>📥 Input Format (<code>mbo.csv</code>)</h2>
<pre><code>ts_event,symbol,side,price,size
2025-07-17T13:46:10.904672934Z,ARL,Ask,2,644856
...</code></pre>
<ul>
  <li><strong>ts_event</strong>: Timestamp of the event</li>
  <li><strong>symbol</strong>: Instrument symbol</li>
  <li><strong>side</strong>: Either <code>Bid</code> or <code>Ask</code></li>
  <li><strong>price</strong>: Order price (as integer or decimal)</li>
  <li><strong>size</strong>: Quantity at the price level</li>
</ul>

<h2>📤 Output Format (<code>mbp.csv</code>)</h2>
<pre><code>ts_event,symbol,side,price,size
2025-07-17T13:46:10.904672934Z,ARL,Bid,13,110
2025-07-17T13:46:10.904672934Z,ARL,Ask,13,220
...</code></pre>
<ul>
  <li>Each snapshot includes up to the <strong>top 10 bids and 10 asks</strong></li>
  <li>Exactly <strong>10 snapshots</strong> are taken, evenly spaced through the MBO data</li>
  <li>Prices are scaled and formatted with appropriate precision</li>
</ul>

<h2>💡 Implementation Highlights</h2>
<ul>
  <li>Price and size levels are tracked using <code>std::map</code>:
    <ul>
      <li><code>std::greater&lt;&gt;</code> for descending <strong>bids</strong></li>
      <li>Default ascending order for <strong>asks</strong></li>
    </ul>
  </li>
  <li>Each MBO event updates the internal book state via actions like Add, Modify, Fill, Cancel, and Clear</li>
  <li>Every 1/10th of the total records, an MBP snapshot is taken (e.g., every 584 records for 5840 rows)</li>
  <li>Book state is cleared after each snapshot to ensure independence</li>
</ul>

<h2>📌 Assumptions</h2>
<ul>
  <li>Only the first 10 unique timestamps are considered for MBP snapshot output</li>
  <li>CSV input only includes 5 fields, extra MBO fields are stubbed internally</li>
  <li>Snapshots are evenly spaced across the dataset</li>
</ul>

<h2>✅ Output Validation</h2>
<ul>
  <li>Exactly 10 MBP snapshots are written</li>
  <li>Each snapshot contains ≤10 bids and ≤10 asks</li>
  <li>Prices and sizes in the output match the internal book state accurately at each snapshot timestamp</li>
</ul>

<h2>🚀 Performance Consideration</h2>
<p>
  While <code>std::map</code> provides ordered keys, for real-time or high-frequency applications, you may consider:
</p>
<ul>
  <li>Using <code>std::unordered_map</code> with a secondary sorted container</li>
  <li>Preallocating structures for predictable symbol sets</li>
  <li>Multi-threading or SIMD optimizations</li>
</ul>

<h2>👨‍💻 Author</h2>
<p>
  <strong>Aditya Kumar Singh</strong><br>
  Technical Assignment – Quantitative Developer (Blockhouse)
</p>

</body>
</html>
