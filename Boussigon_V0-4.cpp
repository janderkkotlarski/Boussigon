#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <chrono>
#include <thread>
#include <utility>

#include <SFML/Graphics.hpp>

struct player_circle
{
    
    const float m_radius{5.0f};
    
    sf::Color m_color{127, 127, 127};
    
    sf::Vector2f m_position{0.0f, 0.0f};
    
    sf::Vector2f m_speedirection{0.0f, 0.0f};
    
    sf::CircleShape m_circle{m_radius};
    
    player_circle(const float radius, const sf::Color& color, const sf::Vector2f& position);
    
    ~player_circle();
       
};

player_circle::player_circle(const float radius, const sf::Color& color, const sf::Vector2f& position)
    :m_radius(radius), m_color(color), m_position(position), m_circle(m_radius)
{
    
    assert(radius > 0.0f);
    
    m_circle.setOrigin(m_radius, m_radius);
    m_circle.setPosition(m_position);
    m_circle.setFillColor(m_color);
}

player_circle::~player_circle()
{
    
}

void recolor_player_circle(player_circle& play_circ, const sf::Color color)
{
    
    play_circ.m_color = color;
    play_circ.m_circle.setFillColor(play_circ.m_color);
    
}

void moving(player_circle& play_circ, const sf::Vector2f& move)
{
    
    play_circ.m_position = play_circ.m_position + move;
    
    play_circ.m_circle.setPosition(play_circ.m_position);
    
}

void move_circle(player_circle& play_circ)
{
    
    moving(play_circ, play_circ.m_speedirection);
    
}

void show_player_circle(sf::RenderWindow& window, const player_circle& play_circ)
{
    
    window.draw(play_circ.m_circle);
    
}

struct object_square
{
    
    const float m_radius{5.0f};
    
    const sf::Color m_color{127, 127, 127};
    
    sf::Vector2f m_position{0.0f, 0.0f};
    
    sf::Vector2f m_speedirection{0.0f, 0.0f};
    
    sf::RectangleShape m_rectangle{sf::Vector2f(2.0f*m_radius, 2.0f*m_radius)};
    
    object_square(const float radius, const sf::Color& color, const sf::Vector2f& position);
    
    ~object_square();
    
};

object_square::object_square(const float radius, const sf::Color& color, const sf::Vector2f& position)
    :m_radius(radius), m_color(color), m_position(position), m_rectangle(sf::Vector2f(2.0f*m_radius, 2.0f*m_radius))
{
    
    assert(radius > 0.0f);
    
    m_rectangle.setOrigin(m_radius, m_radius);
    m_rectangle.setPosition(m_position);
    m_rectangle.setFillColor(m_color);
    
}

object_square::~object_square()
{
      
}

void show_object_square(sf::RenderWindow& window, const object_square& obj_sqr)
{
    
    window.draw(obj_sqr.m_rectangle);
    
}

float square_corner_circle(const sf::Vector2f& circ_posi, const sf::Vector2f& squr_posi,
                    const float radius_x, const float radius_y)
{
    
    assert(radius_x != 0.0f);
    assert(radius_y != 0.0f);
    
    return (squr_posi.x + radius_x - circ_posi.x)*(squr_posi.x + radius_x - circ_posi.x) +
           (squr_posi.y + radius_y - circ_posi.y)*(squr_posi.y + radius_y - circ_posi.y);
    
}

bool collision_check(const player_circle& play_circ, const object_square& obj_sqr)
{
    const float circ_radi{play_circ.m_radius};
    const float squr_radi{obj_sqr.m_radius};
    
    const float dist_x{std::abs(play_circ.m_position.x - obj_sqr.m_position.x)};
    const float dist_y{std::abs(play_circ.m_position.y - obj_sqr.m_position.y)};
    
    const float radi_radi_a{square_corner_circle(play_circ.m_position, obj_sqr.m_position,
                                                     squr_radi, squr_radi)};
                                                     
    const float radi_radi_b{square_corner_circle(play_circ.m_position, obj_sqr.m_position,
                                                     -squr_radi, squr_radi)};

    const float radi_radi_c{square_corner_circle(play_circ.m_position, obj_sqr.m_position,
                                                     squr_radi, -squr_radi)};

    const float radi_radi_d{square_corner_circle(play_circ.m_position, obj_sqr.m_position,
                                                     -squr_radi, -squr_radi)};
                                                     
    if ((((dist_x <= squr_radi) && (dist_y <= circ_radi + squr_radi))) ||
        (((dist_y <= squr_radi) && (dist_x <= circ_radi + squr_radi))) ||
        ((radi_radi_a <= circ_radi*circ_radi)) ||
        ((radi_radi_b <= circ_radi*circ_radi)) ||
        ((radi_radi_c <= circ_radi*circ_radi)) ||
        ((radi_radi_d <= circ_radi*circ_radi)))
    {
        
        return true;
        
    }

    return false;
    
}

void collision_color(player_circle& play_circ, const object_square& obj_sqr,
                     const sf::Color& colli_color, const sf::Color apart_color)
{
    
    if (collision_check(play_circ, obj_sqr))
    {
        
        recolor_player_circle(play_circ, colli_color);
        
    }
    else
    {
        
        recolor_player_circle(play_circ, apart_color);
        
    }   
    
}

void move_dir(player_circle& play_circ, const float delta_dist)
{
        
    sf::Vector2f move{0.0f, 0.0f};
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        
        move.y -= delta_dist;
        
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        
        move.y += delta_dist;
        
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        
        move.x += delta_dist;
        
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        
        move.x -= delta_dist;
        
    }
    
    moving(play_circ, move);
        
}

sf::Vector2f mouse_position(sf::RenderWindow& window)
{
    
    return static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
    
}


bool left_mouse_click()
{
    
    return (sf::Mouse::isButtonPressed(sf::Mouse::Left));
    
}

void mouse_speedirection(sf::RenderWindow& window, player_circle& play_circ, const float speed_mult,
                         const float decay_mult)
{
    
    assert(speed_mult > 0.0f);    
    assert(speed_mult < 1.0f);
    
    assert(decay_mult > 0.0f);    
    assert(decay_mult < 1.0f);
    
    play_circ.m_speedirection = decay_mult*play_circ.m_speedirection;
    
    if (left_mouse_click())
    {
        
        play_circ.m_speedirection = speed_mult*(mouse_position(window) - play_circ.m_position);
                
    }
    
    move_circle(play_circ);
    
}

int main()
{
    
    const std::string program_name{"Boussigon V0.4"};
    
    assert(program_name != "");
    
    const float window_x{700.0f};
    const float window_y{700.0f};    
        
    assert(window_x > 0.0f);    
    assert(window_y > 0.0f);
    
    const sf::Vector2f window_sizes{window_x, window_y};
    
    const int delaz{25};    
    assert(delaz > 0);
    
    const std::chrono::milliseconds delay{delaz};
    
    const int count_max{10};
    
    const sf::Color white{sf::Color(255, 255, 255)};    
    const sf::Color black{sf::Color(0, 0, 0)};    
    const sf::Color orange{sf::Color(255, 127, 0)};    
    const sf::Color purple{sf::Color(127, 0, 255)};    
    const sf::Color cyan{sf::Color(127, 255, 255)};
    const sf::Color red{sf::Color(255, 0, 0)};
    const sf::Color yellow{sf::Color(255, 255, 0)};
    const sf::Color green{sf::Color(0, 255, 0)};
    const sf::Color blue{sf::Color(0, 0, 255)};    
    
    sf::RenderWindow window{sf::VideoMode(window_x, window_y), program_name, sf::Style::Default};
    
    const float init_delta_dist{3.0};
    
    const float delta_dist{init_delta_dist/static_cast<float>(count_max)};
    
    const float circ_radius{50.0f};    
    const sf::Color circ_color{purple};
    const sf::Color colli_color{orange};    
    const sf::Vector2f circ_position{0.25f*window_x, 0.5f*window_y};    
    player_circle player{circ_radius, circ_color, circ_position};
    
    const float init_speed_mult{0.01};
    
    const float speed_mult{init_speed_mult/static_cast<float>(count_max)};
    const float speed_to_decay{0.5f};
    const float decay_mult{1.0f - speed_to_decay*speed_mult};
    
    const float rect_radius{50.0f};    
    const sf::Color rect_color{green};    
    const sf::Vector2f rect_position{0.75f*window_x, 0.5f*window_y};    
    object_square object{rect_radius, rect_color, rect_position};
        
    while (window.isOpen())
    {

        sf::Event event;
        
        window.clear(black);        
        
        show_object_square(window, object);
        
        show_player_circle(window, player);
        
        window.display();
        
        std::this_thread::sleep_for(delay);
        
        for (int count{0}; count < count_max; ++count)
        {
        
            move_dir(player, delta_dist);
            
            mouse_speedirection(window, player, speed_mult, decay_mult);
                    
        }
        
        collision_color(player, object, colli_color, circ_color);
                    
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            
            window.close();
                    
            return 1;
                    
        }    
        
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed)
            {
                
                window.close();
                
                return 2;
                
            }
        
        }
            

    }

    return 0;
    
}
