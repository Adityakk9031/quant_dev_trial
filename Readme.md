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

<h2>Overview</h2>
<p>
  This project reconstructs <strong>Market-by-Price (MBP-10)</strong> snapshots from <strong>Market-by-Order (MBO)</strong> data.
  It maintains an in-memory order book and emits snapshots of the top 10 levels on both bid and ask sides.
</p>
<p>
  This task was completed as part of a technical assignment for a <strong>Quantitative Developer</strong> role.
</p>

<h2>📁 Project Structure</h2>
<pre><code>.
├── main.cpp               # Program entry point
├── parser.hpp/cpp         # MBO CSV reader and parser
├── order_book.hpp/cpp     # In-memory order book logic
├── mbp_writer.hpp/cpp     # MBP snapshot writer
├── types.hpp              # Structs and enums
├── mbo.csv                # Input: Market-by-Order CSV
├── mbp.csv                # Output: Reconstructed MBP snapshots
└── README.html            # Project documentation
</code></pre>

<h2>⚙️ Build & Run Instructions</h2>

<h3>Prerequisites</h3>
<ul>
  <li>C++17 or higher</li>
  <li>Standard build tools (g++, make, or CMake)</li>
</ul>

<h3>Build using g++</h3>
<pre><code>g++ -std=c++17 main.cpp parser.cpp order_book.cpp mbp_writer.cpp -o mbp_builder</code></pre>

<h3>Run</h3>
<pre><code>./mbp_builder mbo.csv mbp.csv</code></pre>

<h2>📥 Input Format (<code>mbo.csv</code>)</h2>
<pre><code>ts_event,symbol,side,price,size
2025-07-17T13:46:10.904672934Z,ARL,Ask,2,644856
...</code></pre>
<ul>
  <li><strong>ts_event</strong>: Event timestamp</li>
  <li><strong>symbol</strong>: Instrument symbol</li>
  <li><strong>side</strong>: Bid or Ask</li>
  <li><strong>price</strong>: Price level</li>
  <li><strong>size</strong>: Quantity at that level</li>
</ul>

<h2>📤 Output Format (<code>mbp.csv</code>)</h2>
<pre><code>ts_event,symbol,side,price,size
2025-07-17T13:46:10.904672934Z,ARL,Bid,13,110
2025-07-17T13:46:10.904672934Z,ARL,Ask,13,220
...</code></pre>
<ul>
  <li>Each snapshot contains the <strong>top 10 bids and asks</strong></li>
  <li>Only <strong>10 unique timestamps</strong> are considered for snapshot output</li>
</ul>

<h2>💡 Implementation Insights</h2>
<ul>
  <li>Book state is stored using <code>std::map</code>:
    <ul>
      <li><code>std::greater&lt;&gt;</code> for descending bid prices</li>
      <li>default ascending sort for asks</li>
    </ul>
  </li>
  <li>Snapshots are taken on each new <code>ts_event</code></li>
  <li>All actions are currently treated as <code>Action::Add</code> to match simplified CSV format</li>
</ul>

<h2>📌 Assumptions</h2>
<ul>
  <li>All actions are assumed to be new orders (<code>Add</code>)</li>
  <li>Only 5 fields are present in the CSV (others are stubbed)</li>
  <li>Snapshot limit: 10 unique timestamps only</li>
</ul>

<h2>✅ Output Validation</h2>
<ul>
  <li>Final output contains exactly 10 MBP snapshots</li>
  <li>Each MBP snapshot has ≤10 bids and asks</li>
  <li>Output prices and sizes match book state at each <code>ts_event</code></li>
</ul>

<h2>🚀 Performance Note</h2>
<p>
  For high-frequency usage, consider replacing <code>std::map</code> with a flat container or specialized structure.
  The modular design allows drop-in optimization.
</p>

<h2>🧠 Author</h2>
<p>
  <strong>Aditya Kumar Singh</strong><br>
  Quantitative Developer Trial Project
</p>

</body>
</html>
