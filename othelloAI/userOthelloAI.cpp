#include <ics46/factory/DynamicFactory.hpp>
#include "userOthelloAI.hpp"
#include <iostream>
#include "OthelloGameState.hpp"
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <iterator>

ICS46_DYNAMIC_FACTORY_REGISTER(OthelloAI, uciid::userOthelloAI, "name goes here");

/**
 * chooseMove
 *
 * chooses a move to place
 * uses a minimax tree and heuristic to choose the move
 *
 * @const OthelloGameState& state: the current game state pointer
 * @return pair<int, int> : x, y of the best move to make
 */
std::pair<int, int> uciid::userOthelloAI::chooseMove(const OthelloGameState& state)
{
  std::vector<int> minimax;
  std::unique_ptr<OthelloGameState> game;
  height = state.board().height();
  width  = state.board().width();
  int depth = 3;
  int max_index;
  int delta = 0;
  //color: 0 = black..  1 = white
  color = (int)state.isWhiteTurn();

  std::vector<std::pair<int,int>> available_moves;
  findAvailableMoves(available_moves, state);

  for(std::vector<std::pair<int,int>>::iterator it = available_moves.begin();
    it != available_moves.end(); ++it)
  {
    if ( isGoldenZone(it->first, it->second) ) {
      return std::pair<int,int>(it->first, it->second);
    }
    if (isDangerZone(it->first, it->second))
    {
      delta += -10;
    }
    game = state.clone();
    game->makeMove(it->first, it->second);
    minimax.push_back(search(game, depth, delta));



  }
  max_index = std::distance(minimax.begin(), std::max_element(minimax.begin(), minimax.end()));
  return available_moves[max_index];
}

/**
 * search
 *
 * searches for a best heuristic to use  
 *
 *
 * //TODO refactor this for the code duplication.
 *
 * @unique_ptr<OthelloGameState> &state: current game state
 * @int depth: the depth of the tree
 * @int delta: delta score
 * @return int : min or max number score
 */
int uciid::userOthelloAI::search(std::unique_ptr<OthelloGameState> const & state, int depth, int delta)
{
  if (state->isGameOver())
  {
    return 0;
  }
  std::vector<int> minimax;
  std::unique_ptr<OthelloGameState> game;
  std::vector<std::pair<int,int>> available_moves;
  findAvailableMoves(available_moves, state);

  if ( depth == 0 ) {
    return evaluation(state, delta);
  } else {
 
    if ( (color == 0 && state->isBlackTurn()) || (color == 1 && state->isWhiteTurn()) ) {
      for(std::vector<std::pair<int,int>>::iterator it = available_moves.begin(); 
        it != available_moves.end(); ++it)
      {
        game = state->clone();
        game->makeMove(it->first, it->second);
        if (isDangerZone(it->first, it->second))
        {
          delta += -10;
        } else if (isEdge(it->first, it->second))
        {
          delta += 30;
        }
        minimax.push_back(search(game, depth - 1, delta));
        //make move on s yeilding a state s'prime
        //search(s'prime, depth - 1);
        //return maximum value

      }
      if ( available_moves.size() == 0 ) 
        return 0;
      return *std::max_element(minimax.begin(), minimax.end());
    } else {
      for(std::vector<std::pair<int,int>>::iterator it = available_moves.begin(); 
        it != available_moves.end(); ++it)
      {
        game = state->clone();
        game->makeMove(it->first, it->second);
        if (isDangerZone(it->first, it->second))
        {
          delta += 10;
        }else if (isEdge(it->first, it->second))
        {
          delta -= 30;
        }
        minimax.push_back(search(game, depth - 1, delta));
        //make move on s yeilding a state s'prime
        //search(s'prime, depth - 1);
        //return minimm value from recursive call
      }

      if ( available_moves.size() == 0)
        return 0;
      return *std::min_element(minimax.begin(), minimax.end());

    }

  }

  
}

/**
 * evaluation
 * general heuristic that returns Opponent - Current Player + some Delta
 * 
 * @unique_ptr<OthelloGameState> state: game state pointer
 * @int delta: delta heuristic
 * 
 * @return int heuristic score
 */
int uciid::userOthelloAI::evaluation(const std::unique_ptr<OthelloGameState>& state, int delta)
{
  int a;
  int b;
  if (color == 0) {
    a = state->blackScore();
    b = state->whiteScore();
  } else {
    a = state->whiteScore();
    b = state->blackScore();
  }

  return a - b + delta;

}

/**
 * ownsCorners
 * DEPRECATED
 * its not even in the code
 */
int uciid::userOthelloAI::ownsCorners(const std::unique_ptr<OthelloGameState>& state)
{
  return 1;
}

void uciid::userOthelloAI::findAvailableMoves(std::vector<std::pair<int,int>>& available_moves, const OthelloGameState& state)
{
  for(int i = 0; i < width; i++)
  {
    for (int j = 0; j < height; j++)
    {
      if ( state.isValidMove(i, j)){
        available_moves.push_back(std::make_pair(i,j));
      }
    }
  }
}

/**
 * findAvailableMoves
 *
 * finds available moves to make
 *
 * @vector<pair<int,int>>& available_moves: available moves
 * @unique_ptr<OthelloGameState> & state: current game state
 *
 * @return void
 */
void uciid::userOthelloAI::findAvailableMoves(std::vector<std::pair<int,int>>& available_moves, std::unique_ptr<OthelloGameState> const & state)
{
  for(int i = 0; i < width-1; i++)
  {
    for (int j = 0; j < height-1; j++)
    {
      if ( state->isValidMove(i, j)){
        available_moves.push_back(std::make_pair(i,j));
      }
    }
  }
}

/**
 * isEdge
 * checks to see if current position is an edge on the board
 *
 * @int x: x pos
 * @int y: y pos
 *
 * @return bool
 */
bool uciid::userOthelloAI::isEdge(int x, int y)
{
  return ((x == 0) || (y == 0) || (y == height - 1) || (x == width - 1));
}

/**
 * isGoldenZone
 *
 * checks if it the current is a corner, if so take it
 * main part of the heuristic is that the more corners you own generally
 * the better your score will be
 * 
 */
bool uciid::userOthelloAI::isGoldenZone(int x, int y)
{
  return ((x == 0 && y == 0) || (x == (width-1) && y == height - 1) || 
       (x == 0 && y == height - 1) || (x == width - 1 && y == 0));
}

/**
 * isDangerZone
 * the danger zones are any position that can possibly lead the opponent a 
 * good position, i.e. corners, or edges
 */
bool uciid::userOthelloAI::isDangerZone(int x, int y)
{
  return checkClose(x,y,width,height) || checkFar(x,y,width,height);
}

/**
 * checkClose
 * misnomer
 * //TODO rename to an verb that does what it does.
 *
 * checks top right corner edges
 */
bool uciid::userOthelloAI::checkClose(int a, int b, int len1, int len2)
{
  return((b == 1 && a != 0 && a != len1 - 1) || (a == 1 && b != 0 && b != len2 - 1));
}

/**
 * checkFar
 * misnomer
 * //TODO rename to an verb that does what it does.
 *
 * checks bottom left corner edges
 */
bool uciid::userOthelloAI::checkFar(int a, int b, int len1, int len2)
{
  return((b == len1 - 2 && a != len1 - 1 && a != 0) || (a == len2 - 2 && b != len2 - 1 && b != 0));
}