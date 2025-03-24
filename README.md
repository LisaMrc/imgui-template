# Welcome to Zen Chess 🍃
♟️ *This game is currently in progress. Hopefully will be finished by may !* ♟️

## 1 | General Info
🧑‍💻 **Developers** : Anass Doublal, Jean Le Chaudelec, Lisa Muracciole
⌨️ **Language** : C++
📚 **Libraries** : OpenGL, Quick Imgui, GFLW, tinyOBJ, miniaudio
🖥️ **Developed for** : Windows, MacOS and Linux

## 2 | Files Organisation
All **.cpp** files are into the "src" folder. All **.hpp** files are into the "include" folder.

#### App related / core  files
**main** : basic file who renders the game and launch the app. Almost not modified
**App** : the main hub of all other files. Helps the game launch and update
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
**Functions, variables and booleans** are in camelCase
**Files** all start with capital letter, except main.cpp
Functions that are used for **debugging** are named debug_functionName

## 6 | Probability laws
1️⃣ Random Background Music (Uniform law) 🎵
2️⃣ Player Switch in the Middle of the Game (Bernoulli law) 🔄
3️⃣ Random Zen Sounds (Geometric law) 🌿
4️⃣ Scene Setting (Multinomial law) 🌤️
5️⃣ Leaf Falls on the Board (Poisson law) 🍃
6️⃣ Drifting Breeze (Hyper geometric law) 🎐
7️⃣ Meditative Promotion (Binomial law) ⏳
8️⃣ AI player 2 🐟