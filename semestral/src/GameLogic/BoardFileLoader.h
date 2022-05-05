#ifndef BOARDFILELOADER_H
#define BOARDFILELOADER_H

#include "FileLoader.h"
#include "Board.h"

class BoardFileLoader : public FileLoader {
private:
    bool buildCheckForSpecialCharacter(char c, size_t x, size_t y, Board & toBoard);

    void createBoardOutOfData(std::list<std::string> & lines, Board & toBoard);

    /**
    * @brief Imported file char to type conversion
    *
    * @exception BoardInvalidGrid char is unknown
    *
    * @param c char from file
    * @return Type Tile::Type the inputted char represents
    */
    static Board::Tile::Type dataCharToType(char c);

public:
    BoardFileLoader(const std::string & filePath);

    Board loadBoard();

};

#endif /* BOARD_H */