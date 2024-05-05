#include "LevelA.h"
#include "Utility.h"

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 8

unsigned int LEVELA_DATA[] =
{
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

LevelA::~LevelA()
{
    delete [] m_state.enemies;
    delete    m_state.player;
    delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelA::initialise()
{
    m_state.next_scene_id = -1;
    
    GLuint map_texture_id = Utility::load_texture("assets/tileset.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 4, 1);
    
    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
    // Existing
    m_state.player = new Entity(20, 11);
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(1.0f, -2.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->m_speed = 2.5f;
    m_state.player->set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("assets/steve.png");
    Weapon* stone_sword = new Weapon("Stone Sword", 3, 80, 15);
    m_state.player->add_weapon(stone_sword);
    Weapon* stone_axe = new Weapon("Stone Axe", 4, 70, 12);
    m_state.player->add_weapon(stone_axe);

    
    // Walking
    m_state.player->m_walking[m_state.player->LEFT]  = new int[4] { 1, 5, 9,  13 };
    m_state.player->m_walking[m_state.player->RIGHT] = new int[4] { 3, 7, 11, 15 };
    m_state.player->m_walking[m_state.player->UP]    = new int[4] { 2, 6, 10, 14 };
    m_state.player->m_walking[m_state.player->DOWN]  = new int[4] { 0, 4, 8,  12 };

    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];  // start George looking left
    m_state.player->m_animation_frames = 4;
    m_state.player->m_animation_index  = 0;
    m_state.player->m_animation_time   = 0.0f;
    m_state.player->m_animation_cols   = 4;
    m_state.player->m_animation_rows   = 4;
    m_state.player->set_height(0.7f);
    m_state.player->set_width(0.7f);
    
    // Jumping
    m_state.player->m_jumping_power = 5.0f;
    
    /**
     Enemies' stuff */
    GLuint enemy_texture_id = Utility::load_texture("assets/zombie.png");
    Weapon* fist = new Weapon("fist", 2, 80, 100);
    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(WALKER);
    m_state.enemies[0].set_ai_state(WALKING);
    m_state.enemies[0].m_texture_id = enemy_texture_id;
    m_state.enemies[0].set_position(glm::vec3(5.0f, -3.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].m_speed = 0.5f;
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.enemies[0].add_weapon(fist);
    m_state.enemies[0].set_hp(8);
    m_state.enemies[0].set_speed(10);


    m_state.enemies[1].set_entity_type(ENEMY);
    m_state.enemies[1].set_ai_type(WALKER);
    m_state.enemies[1].set_ai_state(WALKING);
    m_state.enemies[1].m_texture_id = enemy_texture_id;
    m_state.enemies[1].set_position(glm::vec3(15.0f, -3.0f, 0.0f));
    m_state.enemies[1].set_movement(glm::vec3(0.0f));
    m_state.enemies[1].m_speed = 0.5f;
    m_state.enemies[1].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.enemies[1].add_weapon(fist);
    m_state.enemies[0].set_hp(8);
    m_state.enemies[0].set_speed(10);

    m_state.enemies[2].set_entity_type(ENEMY);
    m_state.enemies[2].set_ai_type(WALKER);
    m_state.enemies[2].set_ai_state(WALKING);
    m_state.enemies[2].m_texture_id = enemy_texture_id;
    m_state.enemies[2].set_position(glm::vec3(20.0f, -3.0f, 0.0f));
    m_state.enemies[2].set_movement(glm::vec3(0.0f));
    m_state.enemies[2].m_speed = 0.5f;
    m_state.enemies[2].set_acceleration(glm::vec3(0.0f, -9.81f, 0.0f));
    m_state.enemies[2].add_weapon(fist);
    m_state.enemies[0].set_hp(8);
    m_state.enemies[0].set_speed(10);
    
    
    /**
     BGM and SFX
     */
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    
    m_state.bgm = Mix_LoadMUS("assets/SubwooferLullaby.wav");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(50.0f);

    m_state.jump_sfx = Mix_LoadWAV("assets/hit.wav");
}

void LevelA::update(float ticks, float delta_time, int& lives, Entity** battle_enemy)
{
    m_state.player->update(ticks, delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, lives);
    for (int i = 0; i < ENEMY_COUNT; i++) m_state.enemies[i].update(ticks, delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, lives);
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (m_state.enemies[i].get_battle_status()) {
            *battle_enemy = &m_state.enemies[i];
            break;
        }
    }
    if (m_state.player->get_position().y < -10.0f) m_state.next_scene_id = 1;
}

void LevelA::render(ShaderProgram *program)
{
    m_state.map->render(program);
    m_state.player->render(program);
    for (int i = 0; i < ENEMY_COUNT; i++) {
        if (m_state.enemies[i].get_entity_status()) {
            m_state.enemies[i].render(program);
        }
    }
    if (!m_state.player->get_entity_status()) {
        std::string text = "GAME OVER!";
        GLuint font_texture_id = Utility::load_texture("assets/font1.png");
        Utility::draw_text(program, font_texture_id, text, 0.45f, 0.001f, glm::vec3(m_state.player->get_position().x - 1.8f, -4.0f, 1.0f));
    }
}
