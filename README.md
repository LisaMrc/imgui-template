# Chess Game
♟️ *This game is currently in progress* ♟️

## 1 | General Info
📚 Library : OpenGL


## 2 | Files Organisation
All **.cpp** files are into the "src" folder. All **.hpp** files are into the "include" folder.
All #include for external libraries and hpp files are at the beginning of the .cpp files.

**main** : basic file who renders the game and launch the app. Almost not modified
**App** : the main hub of all other files. Helps the games manages itself and holds the app's parameters
**Debug** : to test functions, code and debug stuff
**Settings** : holds the settings manageable by the user

**Board** : manage (draw...) the board
**Pawn** : defines the pawns of the game