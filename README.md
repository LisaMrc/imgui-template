# Chess Game
♟️ *This game is currently in progress* ♟️

## 1 | General Info
📚 Library : OpenGL


## 2 | Files Organisation
All **.cpp** files are into the "src" folder. All **.hpp** files are into the "include" folder.

#### App related / core  files
**main** : basic file who renders the game and launch the app. Almost not modified
**App** : the main hub of all other files. Helps the game launch and update
**Debug** : to test code and debug stuff

#### User related files
**Player** : holds player info and data
**Settings** : holds the settings manageable by the user

#### Pieces related files
**Board** : manages (draws) the board
**Pawn** : defines the pawns of the game


## 3 | Github Lexicon
[ WIP ] ... == Work in progress
[ SAVE ] ... == Broken code, do not pull. Only serves to save current code before debug
[ DONE ] ... == end of task
[ UPDATE ] ... == improved a task previously marked as DONE (optimisation or debug)
[ CLEANUP ] ... == deletions of useless parts, files organisation, typos
[ MERGE ] ... = merge


## 4 | Other notes
- Pieces do not have a color, the player does. The pieces belong to the player
- 

## 5 | Naming conventions
**Classes** are in PascalCase
**Functions and booleans** are in camelCase
**Files** all start with capital letter