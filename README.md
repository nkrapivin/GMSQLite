# GMSQLite

(insert logo here)

An open source SQLite wrapper for GameMaker: Studio.

## Using
- Import the extension package according to your GameMaker: Studio version.
- Verify that gmsqlite_ functions have been added.
- Check the [Wiki](https://github.com/nkrapivin/GMSQLite/wiki) for the documentation.


## Building

### Windows (64-bit for GMS2.3.1 and above, 32-bit for GMS2.3.0 and below)
- Open gmsqlite.sln in Visual Studio 2017 or higher.
- Build the project.
- Replace GMSQLite.dll in the extension files.

### Linux (64-bit, GMS2 and above)
- Make sure you have a C++ and a C compiler installed.
- Run `make`
- Replace libGMSQLite.so in the extension files.

### Linux (32-bit, GM:S 1.4)
- Make sure you have a C++ and a C compiler installed.
- Also make sure you have multilib support installed if you're on a 64-bit system.
- Run `make gms1`
- Replace libGMSQLite-32bit.so in the extension files.

### macOS (64-bit only)
- Not yet! Sorry!



## Credits
- SQLite Consortium for the awesome SQLite library.
- YoYoGames, for making such an awesome engine, where you can't even mark DS Maps as DS Maps...


