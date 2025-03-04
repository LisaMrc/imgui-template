# Chess Game
♟️ *This game is currently in progress. Hopefully will be finished by may !* ♟️

## 1 | General Info
🧑‍💻 Developers : Anass Doublal, Jean Le Chaudelec, Lisa Muracciole
📚 Library : OpenGL
🖥️ Developed for : Windows, MacOS and Linux

## 2 | Files Organisation
All **.cpp** files are into the "src" folder. All **.hpp** files are into the "include" folder.

#### App related / core  files
**main** : basic file who renders the game and launch the app. Almost not modified
**App** : the main hub of all other files. Helps the game launch and update
**Debug** : to test code and debug stuff
**Math** : holds math-related and random functions

#### User related files
**Player** : holds player info and data
**Settings** : holds the settings manageable by the user

#### Pieces related files
**Board** : manages (draws) the board
**Pieces** : defines the pieces of the game and their settings

## 3 | Github Lexicon
[ WIP ] ... == Work in progress
[ SAVE ] ... == Broken code, do not pull. Only serves to save current code before debug
[ DONE ] ... == end of task
[ UPDATE ] ... == improved a task previously marked as DONE (optimisation or debug)
[ CLEANUP ] ... == deletions of useless parts, files organisation, typos
[ MERGE ] ... = merge
[ NEEDS TESTING ] ... = written function that needs to be tested

## 4 | Other notes


## 5 | Naming conventions
**Classes** are in PascalCase
**Functions and booleans** are in camelCase
**Files** all start with capital letter

## 5 | Random variables
1. Event : switches player
2. Event : leaf falls on board in an empty spot
3. Event : ORDI/AI player 2
4. Random sounds 
5. Random background music
6. Fish swimming trajectory at the sides of the board
7. Random color (you are playing as white or black and the machine sets it for you)
8. Random animals at the pond