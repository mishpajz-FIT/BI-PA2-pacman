/****************************************************************
 * @file AppState.h
 * @author Michal Dobes
 * @brief App state
 * @date 2022-05-25
 *
 * @copyright Copyright (c) 2022
 *
 *****************************************************************/

#ifndef APPSTATE_H
#define APPSTATE_H

/**
 * @brief States in which the app may find itself
 *
 *
 */
enum class AppState {
    programContinue,
    programExit,
    game,
    mainmenu
};

#endif /* APPSTATE_H */