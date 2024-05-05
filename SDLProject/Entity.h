#pragma once
#include "Map.h"
#include "Weapon.h"

enum EntityType { PLATFORM, PLAYER, ENEMY };
enum AIType { WALKER, GUARD, PATROLLER };
enum AIState { WALKING, IDLE, ATTACKING, PATROLLING, JUMPING, DEAD };

class Entity
{
private:
    bool       m_is_active = true;
    EntityType m_entity_type;
    AIType     m_ai_type;
    AIState    m_ai_state;

    bool is_battling = false;
    int hp;
    int speed;
    int selected_weapon      = -1;
    int weapon_count = 0;
    Weapon* weapons[4];

    int *m_animation_right = NULL; // move to the right
    int *m_animation_left  = NULL; // move to the left
    int *m_animation_up    = NULL; // move upwards
    int *m_animation_down  = NULL; // move downwards
    
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_acceleration;
    
    float m_width  = 0.8f;
    float m_height = 0.8f;
    
public:
    // Static attributes
    static const int SECONDS_PER_FRAME = 4;
    static const int LEFT  = 0,
                     RIGHT = 1,
                     UP    = 2,
                     DOWN  = 3;
    
    // Existing
    GLuint    m_texture_id;
    glm::mat4 m_model_matrix;
    
    // Translating
    float     m_speed;
    glm::vec3 m_movement;
    
    // Animating
    int **m_walking           = new int*[4] { m_animation_left, m_animation_right, m_animation_up, m_animation_down };
    int  *m_animation_indices = NULL;
    int   m_animation_frames  = 0;
    int   m_animation_index   = 0;
    float m_animation_time    = 0.0f;
    int   m_animation_cols    = 0;
    int   m_animation_rows    = 0;
    
    // Jumping
    bool  m_is_jumping = false;
    float m_jumping_power = 0;

    // Colliding
    bool m_collided_top    = false;
    bool m_collided_bottom = false;
    bool m_collided_left   = false;
    bool m_collided_right  = false;

    // Methods
    Entity();
    Entity(int hp, int speed);
    ~Entity();

    void draw_sprite_from_texture_atlas(ShaderProgram *program, GLuint texture_id, int index);
    void update(float ticks, float delta_time, Entity *player, Entity *objects, int object_count, Map *map, int& lives);
    void render(ShaderProgram *program);
    
    void ai_activate(float ticks, Entity *player);
    void ai_walk();
    void ai_guard(float ticks, Entity *player);
    
    void const check_collision_y(Entity *collidable_entities, int collidable_entity_count, int& lives);
    void const check_collision_x(Entity *collidable_entities, int collidable_entity_count, int& lives);
    void const check_collision_y(Map *map);
    void const check_collision_x(Map *map);
    
    bool const check_collision(Entity *other) const;
    
    void activate()   { m_is_active = true;  };
    void deactivate() { m_is_active = false; };
    bool       const get_entity_status()      const { return m_is_active; };
    EntityType const get_entity_type()  const { return m_entity_type;  };
    AIType     const get_ai_type()      const { return m_ai_type;      };
    AIState    const get_ai_state()     const { return m_ai_state;     };
    glm::vec3  const get_position()     const { return m_position;     };
    glm::vec3  const get_movement()     const { return m_movement;     };
    glm::vec3  const get_velocity()     const { return m_velocity;     };
    glm::vec3  const get_acceleration() const { return m_acceleration; };
    int        const get_width()        const { return m_width;        };
    int        const get_height()       const { return m_height;       };
    int              get_speed()        const { return speed;          };
    int              get_selected_weapon() const { return selected_weapon; };
    int              get_hp()           const { return hp; };
    bool              get_battle_status()const { return is_battling; };
    const Weapon* get_weapon(int index)       const { return weapons[index]; };
    
    void const set_entity_type(EntityType new_entity_type)  { m_entity_type  = new_entity_type;      };
    void const set_ai_type(AIType new_ai_type)              { m_ai_type      = new_ai_type;          };
    void const set_ai_state(AIState new_state)              { m_ai_state     = new_state;            };
    void const set_position(glm::vec3 new_position)         { m_position     = new_position;         };
    void const set_movement(glm::vec3 new_movement)         { m_movement     = new_movement;         };
    void const set_velocity(glm::vec3 new_velocity)         { m_velocity     = new_velocity;         };
    void const set_acceleration(glm::vec3 new_acceleration) { m_acceleration = new_acceleration;     };
    void const set_width(float new_width)                   { m_width        = new_width;            };
    void const set_height(float new_height)                 { m_height       = new_height;           };
    void       set_hp(int new_hp)                           { hp             = new_hp;               };
    void       set_speed(int new_speed)                     { hp             = new_speed;            };
    void       select_weapon(int weapon_index)                    { if (weapons[weapon_index] != nullptr) selected_weapon = weapon_index; };
    void       take_damage(int damage)                      { hp -= damage; };
    void       toggle_battle_status() { if (is_battling) is_battling = false; else is_battling = true; }
    void       add_weapon(Weapon* weapon) {
        if (weapon_count < 4) {
            weapons[weapon_count] = weapon;
            weapon_count++;
        }
    }
    void        use_weapon(int weapon_index) { 
        weapons[weapon_index]->use();
        if (weapons[weapon_index]->get_dur() == 0) {
            weapons[weapon_index] = nullptr;
        }
    }
};
