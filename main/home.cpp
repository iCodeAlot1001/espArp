#include "globalVar.h"
#include <string>

std::string htmlHome = R"(<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Home</title>
    <link rel="stylesheet" href="style.css">

    </head>
<body>
    <header>
        <h1>add ip of master</h1>
    </header>
    <main>
        <form action="/input" method="POST">
        <input type="text" name="targetSsid" id="inputIP" placeholder="Enter SSID" required>
        <input type="text" name="targetPass" id="routerIP" placeholder="Enter Password optionala">
        <button type="submit">Submit</button>
        </form>
        <br>
        <label for="fileHandle">Upload payload</label>
        <input type="file" name="fileHandle">
        <br>
        <br>
        <a href="/monitor" class="button">Go to Monitor</a>
        <br>
        <a href="/checkPass" class="button2">Check Pass</a>

    </main>

    <footer>
        <p>Romar Hernandez 2025</p>
        <pre>
github: https://github.com/iCodeAlot1001
        </pre>
    </footer>
</body>
</html>)";