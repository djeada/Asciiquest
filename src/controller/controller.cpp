#include "controller.h"


Controller::Controller() {
            isRunning = false;
            currentGameState = GameState::MAINE_MENU;
        }

        void Controller::run() {
            isRunning = true;
            

            while(isRunning) {
                switch(currentGameState) {

                    
                    case GameState::MAINE_MENU:
                            handleMainMenu();
                            break;
                    case GameState::GAMEPLAY:
                            handleMainMenu();
                            break;
                    case GameState::PAUSE_MENU:
                            handlePauseMenu();
                            break;
                    case GameState::GAME_OVER:
                            handleGameOver();
                            break;
                    default:
                            break;

                }
            }
        }
        
        void Controller::handleGamePlay() {
                handleInput();
                renderer.drawBoard(model.grid);
               // renderer.drawStats(model.getPlayerStats());
                renderer.drawFightInfo(model.fightInfo);
        
            if (model.isGameOver()) {
                currentGameState = GameState::GAME_OVER;

            }
        }

        void Controller::handleGameOver(){}
        void Controller::handlePauseMenu(){}
        void Controller::handleMainMenu(){}

        void Controller::handleInput() {
            int ch = getch();



            isRunning = false;
        }


