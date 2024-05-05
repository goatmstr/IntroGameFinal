#include "Scene.h"

class LevelA : public Scene {
public:
    int ENEMY_COUNT = 3;
    
    ~LevelA();
    
    void initialise() override;
    void update(float ticks, float delta_time, int& lives, Entity** enemy) override;
    void render(ShaderProgram *program) override;
};
