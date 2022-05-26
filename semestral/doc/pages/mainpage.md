# dobesmic's PacMan

## About

dobesmic's PacMan is implementation of classic PacMan game. 
The game has a graphical interface in the terminal that is responsive.

The game has 4 ghost enemies: Blinky, Pinky, Inky, Clyde, each with its own AI, implemented in same way as original game.
It also implements the playing board, along with tiles like wall, space, coin, bonus and power-up and with possibility of teleportation from one side of board to the other.
Ghosts also have two modes: chase and scatter.

Once launched, the game offers to select the difficulty, load the game configuration and then load the map from the file. The game then starts. When the game is finished, either by collecting all the coins or by running out of lives, the score is saved back into the configuration file.

## Configuration file

### Path

Configuration files must be stored in the `./examples/Settings/` folder.
The file must have a `.spac` extension.

### Format

The format of the configuration file must be the same as the following example:

    player_periodspeed>		350 
    ghost_periodspeed>		400 
    scatter_duration>		7000
    chase_duration>			20000 
    frighten_duration>		15000 
    kill_duration>			8000
    bonus_period>			20000 
    ghost_begin_period>		5000

With values as integers in milliseconds, where:

 - `player_periodspeed` is the period between player movement
 - `ghost_periodspeed` is the period between ghost movement
 - `scatter_duration` is the duration of scatter phase
 - `chase_duration` is the duration of chase phase
 - `frighten_duration` is the duration of frighten phase (blue ghosts, after activating power-up)
 - `kill_duration` is the time until a new bonus is created
 - `bonus_period` is the period between when a bonus is generated
 - `ghost_begin_period` is a period between times when the ghosts come out of their house after the game restarts
 
## Map file

### Path

The map file must be stored in the directory `./examples/Maps/` and has to have `.mpac` extension.

The file must be a grid of characters, i.e. each line must have the same number of characters.

Characters are transferred to the game board after loading as follows:

 - `#` - wall
 - ` ` - space
 - `.` - coin
 - `o` - power-up
 - `P` - player spawn
 - `E` - enemy spawn

Where the last two are required and can be placed in map only once. Enemy spawn 
also needs to have two tiles to the left that allow for movement.

Teleport is created automatically when moveable tile is placed at the edge of map.

Example of default map:

    #######################
    #..........#..........#
    #o###.####.#.####.###o#
    #.###.####.#.####.###.#
    #.....................#
    #.###.#.#######.#.###.#
    #.....#....#....#.....#
    #####.#### # ####.#####
    #####.#    E    #.#####
         .  #######  .     
    #####.#         #.#####
    #####.# ####### #.#####
    #..........#..........#
    #.###.####.#.####.###.#
    #o..#......P......#..o#
    ###.#.#.#######.#.#.###
    #.....#....#....#.....#
    #.########.#.########.#
    #.....................#
    #######################

## Display

The app requires colors in terminal to be able to run correctly. Ideal is 256+ colors, but offers fallback to 8 colors. Game won't start if colors are not supported. 