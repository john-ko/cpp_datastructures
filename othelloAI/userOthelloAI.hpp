#include "OthelloAI.hpp"
#include <vector>
#include <utility>
namespace kojy1
{
    class userOthelloAI : public OthelloAI
    {
    public:
      bool isDangerZone(int,int);
      bool isGoldenZone(int,int);
      int search(std::unique_ptr<OthelloGameState> const& state, int, int);
        virtual std::pair<int, int> chooseMove(const OthelloGameState& state);
        bool checkClose(int, int, int, int);
        bool checkFar(int, int, int, int);
        void findAvailableMoves(std::vector<std::pair<int,int>>&, const OthelloGameState&);
        void findAvailableMoves(std::vector<std::pair<int,int>>&, const std::unique_ptr<OthelloGameState>& state);
        int evaluation(const std::unique_ptr<OthelloGameState>& state, int);
        int additionalEvaluation(int, int);
        int ownsCorners(const std::unique_ptr<OthelloGameState>& state);
        bool isEdge(int, int);
    private:
      int height;
      int width;
        int color;

    };
}