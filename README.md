# Chess Game
♟️ *This game is currently in progress* ♟️

## 1 | General Info
📚 Library : OpenGL


## 2 | Files Organisation
All **.cpp** files are into the "src" folder. All **.hpp** files are into the "include" folder.

**main** : basic file who renders the game and launch the app. Almost not modified
**App** : the main hub of all other files. Helps the games manages itself and holds the app's parameters
**Debug** : to test functions, code and debug stuff
**Settings** : holds the settings manageable by the user
**Color** : hpp alone, just has an enum color, used very often in the game

**Board** : manage (draw...) the board
**Pawn** : defines the pawns of the game

## 3 | Github Lexicon
[ WIP ] ... == Work in progress
[ SAVE ] ... == Broken code, do not pull. Only serves to save current code before debug
[ DONE ] ... == end of task
[ UPDATE ] ... == improved a task previously marked as DONE (optimisation or debug)
[ CLEANUP ] ... == deletions of useless parts, files organisation, typos

## 4 | Other notes
- Pieces do not have a color, the player does. The pieces belong to the player