#include "logic.hpp"

bool checkCollision (SDL_Rect A, SDL_Rect B) {
    return (A.x + A.w >= B.x && B.x + B.w >= A.x &&
            A.y + A.h >= B.y && B.y + B.h >= A.y);
}
