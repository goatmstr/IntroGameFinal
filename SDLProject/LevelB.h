#include "Scene.h"

class LevelB : public Scene {
public:
    int ENEMY_COUNT = 3;
    
    ~LevelB();
    
    void initialise() override;
    void update(float ticks, float delta_time, int& lives, Entity** enemy) override;
    void render(ShaderProgram *program) override;
};
