const express = require('express');
const fs = require('fs');
const readline = require('readline');

const app = express();
const PORT = 3000;

app.use(express.static(__dirname)); // Serve static files like index.html

// Endpoint to send TSV data as JSON
app.get('/data', (req, res) => {
    const results = [];

    const rl = readline.createInterface({
        input: fs.createReadStream('data.txt'),
        output: process.stdout,
        terminal: false
    });

    rl.on('line', (line) => {
        const [Time, ID, Smoke, Temp] = line.trim().split(/\s+/);
        if (Time !== "Time") { // Skip header row
            results.push({ Time, ID, Smoke, Temp: parseFloat(Temp) });
        }
    });

    rl.on('close', () => {
        res.json(results);
    });
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
