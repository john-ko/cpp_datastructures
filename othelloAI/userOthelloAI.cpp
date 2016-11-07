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

int uciid::userOthelloAI::evaluation(const std::unique_ptr<OthelloGameState>& state, int delta)
{
  if ( color == 0 ){

    return state->blackScore() - state->whiteScore() + delta;
  } else {
    return state->whiteScore() - state->blackScore() + delta;
  }

  return 1;
}

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

bool uciid::userOthelloAI::isEdge(int x, int y)
{
  return ((x == 0) || (y == 0) || (y == height - 1) || (x == width - 1));
}

bool uciid::userOthelloAI::isGoldenZone(int x, int y)
{
  return ((x == 0 && y == 0) || (x == (width-1) && y == height - 1) || 
       (x == 0 && y == height - 1) || (x == width - 1 && y == 0));
}

bool uciid::userOthelloAI::isDangerZone(int x, int y)
{
  return checkClose(x,y,width,height) || checkFar(x,y,width,height);
}

bool uciid::userOthelloAI::checkClose(int a, int b, int len1, int len2)
{
  return((b == 1 && a != 0 && a != len1 - 1) || (a == 1 && b != 0 && b != len2 - 1));
}

bool uciid::userOthelloAI::checkFar(int a, int b, int len1, int len2)
{
  return((b == len1 - 2 && a != len1 - 1 && a != 0) || (a == len2 - 2 && b != len2 - 1 && b != 0));
}