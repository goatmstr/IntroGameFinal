#include "Scene.h"

class LevelC : public Scene {
public:
    int ENEMY_COUNT = 3;

    ~LevelC();

    void initialise() override;
    void update(float ticks, float delta_time, int& lives, Entity** enemy) override;
    void render(ShaderProgram* program) override;
};
