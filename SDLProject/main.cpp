#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define FIXED_TIMESTEP 0.0166666f
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_LEFT_EDGE 5.0f

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "cmath"
#include <ctime>
#include <vector>
#include "Entity.h"
#include "Map.h"
#include "UI.h"
#include "Utility.h"
#include "Scene.h"
#include "LevelA.h"
#include "LevelB.h"
#include "LevelC.h"
#include "Menu.h"


//#include "Effects.h"

// ––––– CONSTANTS ––––– //
const int WINDOW_WIDTH  = 640,
          WINDOW_HEIGHT = 480;

const float BG_RED     = 0.0f,
            BG_BLUE    = 0.0f,
            BG_GREEN   = 0.0f,
            BG_OPACITY = 1.0f;

const int VIEWPORT_X = 0,
          VIEWPORT_Y = 0,
          VIEWPORT_WIDTH  = WINDOW_WIDTH,
          VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_lit.glsl",
           F_SHADER_PATH[] = "shaders/fragment_lit.glsl";

const float MILLISECONDS_IN_SECOND = 1000.0;


// ––––– GLOBAL VARIABLES ––––– //
Scene  *g_current_scene;
Menu   *g_main_menu;
LevelA *g_levelA;
LevelB *g_levelB;
LevelC *g_levelC;
int lives = 3;
bool in_battle = false;
bool is_player_turn = false;
Entity* battle_enemy = nullptr;
UI* text_ui;
UI* battle_ui;
UI* weapon_ui;
bool show_dialogue = false;
bool wait_dialogue = true;
std::string dialogue = "HONGBIN's ATTACK MISSED! WHY???";

//Effects *g_effects;
Scene   *g_levels[3];

SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_shader_program;
glm::mat4 g_view_matrix, g_projection_matrix;

float g_previous_ticks = 0.0f;
float g_accumulator = 0.0f;

bool g_is_colliding_bottom = false;

#define LEVEL_WIDTH 20
#define LEVEL_HEIGHT 8

unsigned int TEXT_UI_DATA[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

unsigned int BATTLE_UI_DATA[] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

unsigned int WEAPON_UI_DATA[] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 2, 3, 4, 3, 5, 3, 6, 3, 3, 3, 2, 3, 2, 3, 2, 3, 2, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};
// ––––– GENERAL FUNCTIONS ––––– //
void switch_to_scene(Scene *scene)
{
    g_current_scene = scene;
    g_current_scene->initialise(); // DON'T FORGET THIS STEP!
}

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    g_display_window = SDL_CreateWindow("Minecraft RPG",
                                      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      WINDOW_WIDTH, WINDOW_HEIGHT,
                                      SDL_WINDOW_OPENGL);
    
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    
    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);
    
    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);
    
    glUseProgram(g_shader_program.m_program_id);
    
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);
    
    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    g_main_menu = new Menu();
    g_levelA = new LevelA();
    g_levelB = new LevelB();
    g_levelC = new LevelC();
    
    g_levels[0] = g_levelA;
    g_levels[1] = g_levelB;
    g_levels[2] = g_levelC;
    // Start at Main Menu
    switch_to_scene(g_main_menu);
    
    GLuint textui_texture_id = Utility::load_texture("assets/textui.png");
    text_ui = new UI(LEVEL_WIDTH, LEVEL_HEIGHT, TEXT_UI_DATA, textui_texture_id, 1.0f, 4, 1);

    GLuint battleui_texture_id = Utility::load_texture("assets/battleui.png");
    battle_ui = new UI(LEVEL_WIDTH, LEVEL_HEIGHT, BATTLE_UI_DATA, battleui_texture_id, 1.0f, 4, 1);

    GLuint weaponui_texture_id = Utility::load_texture("assets/weaponui.png");
    weapon_ui = new UI(LEVEL_WIDTH, LEVEL_HEIGHT, WEAPON_UI_DATA, weaponui_texture_id, 1.0f, 10, 1);
}

void process_input()
{
    // VERY IMPORTANT: If nothing is pressed, we don't want to go anywhere
    g_current_scene->m_state.player->set_movement(glm::vec3(0.0f));
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
            // End game
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = false;
                break;
            
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                        
                    case SDLK_RETURN:
                        if (g_current_scene == g_main_menu) {
                            g_current_scene->m_state.next_scene_id = 0;
                        }
                        
                    case SDLK_q:
                        if (in_battle && is_player_turn) {
                            g_current_scene->m_state.player->select_weapon(0);
                        }
                    case SDLK_w:
                        if (in_battle && is_player_turn) {
                            g_current_scene->m_state.player->select_weapon(1);
                        }
                    case SDLK_e:
                        if (in_battle && is_player_turn) {
                            g_current_scene->m_state.player->select_weapon(2);
                        }
                    case SDLK_r:
                        if (in_battle && is_player_turn) {
                            g_current_scene->m_state.player->select_weapon(3);
                        }
                    case SDLK_SPACE:
                        if (wait_dialogue) {
                            wait_dialogue = false;
                        }
                    default:
                        break;
                }
             
            default:
                break;
        }
    }
    
    const Uint8 *key_state = SDL_GetKeyboardState(NULL);
    if (!in_battle) {
        if (key_state[SDL_SCANCODE_LEFT])
        {
            g_current_scene->m_state.player->m_movement.x = -1.0f;
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->LEFT];
        }
        else if (key_state[SDL_SCANCODE_RIGHT])
        {
            g_current_scene->m_state.player->m_movement.x = 1.0f;
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->RIGHT];
        }
        else if (key_state[SDL_SCANCODE_DOWN])
        {
            g_current_scene->m_state.player->m_movement.y = -1.0f;
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->RIGHT];
        }
        else if (key_state[SDL_SCANCODE_UP])
        {
            g_current_scene->m_state.player->m_movement.y = 1.0f;
            g_current_scene->m_state.player->m_animation_indices = g_current_scene->m_state.player->m_walking[g_current_scene->m_state.player->RIGHT];
        }

        if (glm::length(g_current_scene->m_state.player->m_movement) > 1.0f)
        {
            g_current_scene->m_state.player->m_movement = glm::normalize(g_current_scene->m_state.player->m_movement);
        }
    }
}

// ----- BATTLE FUNCTIONS ----- //

void render();

void update_dialogue(std::string new_dialogue) {
    show_dialogue = true;
    dialogue = new_dialogue;
    render();
    wait_dialogue = true;
    while (wait_dialogue) {
        process_input();
    }
    show_dialogue = false;
    render();
}

void player_turn(Entity* player, Entity* enemy) {
    is_player_turn = true;
    const Weapon* player_weapon = nullptr;
    while (!player_weapon) {
        process_input();
        if (player->get_selected_weapon() != -1) {
            player_weapon = player->get_weapon(player->get_selected_weapon());
        }
    }

        int hit_chance = rand() % 100 + 1;
        if (hit_chance <= player_weapon->get_acc()) {
            update_dialogue("Steve attacks with " + player_weapon->get_name() + "!");
            enemy->take_damage(player_weapon->get_dmg());
            update_dialogue("Enemy took " + std::to_string(player_weapon->get_dmg()) + "damage!");
            player->use_weapon(player->get_selected_weapon());
            is_player_turn = false;
            player->select_weapon(-1);
        }
        else {
            update_dialogue("Steve missed!");
            is_player_turn = false;
            player->select_weapon(-1);
        }
}

void enemy_turn(Entity* player, Entity* enemy) {
    const Weapon* enemy_weapon = enemy->get_weapon(0);
    int hit_chance = rand() % 100 + 1;
    if (hit_chance <= enemy_weapon->get_acc()) {
        update_dialogue("Enemy attacks with " + enemy_weapon->get_name() + "!");
        player->take_damage(enemy_weapon->get_dmg());
        update_dialogue("Steve took " + std::to_string(enemy_weapon->get_dmg()) + "damage!");
    }
    else {
        update_dialogue("Enemy missed!");
    }

}



void update()
{
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;
    
    delta_time += g_accumulator;
    
    if (delta_time < FIXED_TIMESTEP)
    {
        g_accumulator = delta_time;
        return;
    }
    
    while (delta_time >= FIXED_TIMESTEP) {
        g_current_scene->update(ticks, FIXED_TIMESTEP, lives, &battle_enemy);
        if (battle_enemy) {
            in_battle = true;
            battle_enemy->toggle_battle_status();
        }


        g_is_colliding_bottom = g_current_scene->m_state.player->m_collided_bottom;
        
        delta_time -= FIXED_TIMESTEP;
    }
    
    g_accumulator = delta_time;
    
    // Prevent the camera from showing anything outside of the "edge" of the level
    g_view_matrix = glm::mat4(1.0f);
    if (in_battle) {
        g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5, 3.75, 0));
    }
    else {
        if (g_current_scene->m_state.player->get_position().x > LEVEL1_LEFT_EDGE) {
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-g_current_scene->m_state.player->get_position().x, 3.75, 0));
        }
        else {
            g_view_matrix = glm::translate(g_view_matrix, glm::vec3(-5, 3.75, 0));
        }
    }
    
    
    
    if (g_current_scene == g_levelC && g_current_scene->m_state.player->get_position().y < -10.0f) {
        std::string text = "YOU WIN!";
        GLuint font_texture_id = Utility::load_texture("assets/font1.png");
        Utility::draw_text(&g_shader_program, font_texture_id, text, 0.45f, 0.001f, glm::vec3(g_current_scene->m_state.player->get_position().x, -5.0f, 0.0f));
    }
 

    
    //g_shader_program.set_light_position(g_current_scene->m_state.player->get_position());
}

void render()
{
    g_shader_program.set_view_matrix(g_view_matrix);
    glClear(GL_COLOR_BUFFER_BIT);
 
    glUseProgram(g_shader_program.m_program_id);

    if (in_battle) {
        weapon_ui->render(&g_shader_program);
    }
    else {
        g_current_scene->render(&g_shader_program);
    }
    

    std::string text = "HP:" + std::to_string(g_current_scene->m_state.player->get_hp());
    GLuint font_texture_id = Utility::load_texture("assets/font1.png");
    Utility::draw_text(&g_shader_program, font_texture_id, text, 0.3f, 0.001f, glm::vec3(g_current_scene->m_state.player->get_position().x -0.5f, g_current_scene->m_state.player->get_position().y + 1.0f, 1.0f));
    if (show_dialogue) {
        Utility::draw_text(&g_shader_program, font_texture_id, dialogue, 0.3f, 0.0f, glm::vec3(0.5f, -4.0f, 0.0f));
    }
    SDL_GL_SwapWindow(g_display_window);

}

void shutdown()
{
    SDL_Quit();
    
    delete g_levelA;
    delete g_levelB;
    delete g_levelC;
    delete g_main_menu;
}



// ––––– DRIVER GAME LOOP ––––– //
int main(int argc, char* argv[])
{
    initialise();
    
    while (g_game_is_running)
    {
        process_input();
        update();
        if (battle_enemy) {
            if (g_current_scene->m_state.player->get_speed() >= battle_enemy->get_speed()) {
                update_dialogue("Steve is faster, he moves first!");
                while (in_battle) {
                    player_turn(g_current_scene->m_state.player, battle_enemy);
                    render();
                    if (battle_enemy->get_hp() <= 0) {
                        render();
                        battle_enemy->deactivate();
                        in_battle = false;
                        break;
                    }
                    enemy_turn(g_current_scene->m_state.player, battle_enemy);
                    render();
                    if (g_current_scene->m_state.player->get_hp() <= 0) {
                        render();
                        g_current_scene->m_state.player->deactivate();
                        in_battle = false;
                        break;
                    }
                }
                battle_enemy = nullptr;
            }
            else {
                update_dialogue("Enemy is faster, they moves first!");
                while (in_battle) {
                    enemy_turn(g_current_scene->m_state.player, battle_enemy);
                    if (g_current_scene->m_state.player->get_hp() <= 0) {
                        render();
                        g_current_scene->m_state.player->deactivate();
                        in_battle = false;
                        break;
                    }
                    player_turn(g_current_scene->m_state.player, battle_enemy);
                    render();
                    if (battle_enemy->get_hp() <= 0) {
                        render();
                        battle_enemy->deactivate();
                        in_battle = false;
                        break;
                    }
                }
                battle_enemy = nullptr;
            }
        }
        if (g_current_scene->m_state.next_scene_id >= 0) switch_to_scene(g_levels[g_current_scene->m_state.next_scene_id]);
        
        render();
    }
    
    shutdown();
    return 0;
}
