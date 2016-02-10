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
    
    const float m_radius{10.0f};
    
    const float m_thickness{3.0f};
    
    sf::Color m_color{127, 127, 127};
    
    sf::Vector2f m_position{0.0f, 0.0f};
    
    sf::Vector2f m_speedirection{0.0f, 0.0f};
    
    sf::Vector2f m_accelerection{0.0f, 0.0f};
            
    float m_fuel{0.0f};
        
    sf::CircleShape m_circle{m_radius};
    
    sf::RectangleShape m_rectangle{sf::Vector2f(2.0f*m_radius, m_thickness)};
    
    void set_position(const sf::Vector2f position)
    {
        
        m_position = position;
        
        m_circle.setPosition(m_position);
        
        m_rectangle.setPosition(m_position - sf::Vector2f(0, m_radius + 0.7f*m_thickness));
        
    }
    
    void rescale_rect()
    {
		
		m_rectangle.setScale(m_fuel, 1.0f);
		
	}
    
    player_circle(const float radius, const sf::Color& color, const sf::Vector2f& position);
    
    ~player_circle();
       
};

player_circle::player_circle(const float radius, const sf::Color& color, const sf::Vector2f& position)
    :m_radius(radius), m_color(color), m_position(position), m_circle(m_radius),
     m_rectangle(sf::Vector2f(2.0f*m_radius, m_thickness))
{
    
    assert(radius > 0.0f);
    
    m_circle.setOrigin(m_radius, m_radius);
    m_circle.setPosition(m_position);
    m_circle.setFillColor(m_color);
    
    m_rectangle.setOrigin(m_radius, 0.5f*m_thickness);
    m_rectangle.setPosition(m_position - sf::Vector2f(0, m_radius + 0.7f*m_thickness));
    m_rectangle.setFillColor(m_color);
    
}

player_circle::~player_circle()
{
    
}

void recolor_player_circle(player_circle& play_circ, const sf::Color color)
{
    
    play_circ.m_color = color;
    play_circ.m_circle.setFillColor(play_circ.m_color);
    
}

void stop_circle(player_circle& play_circ, const sf::Vector2f& position)
{
    
    play_circ.set_position(position);
    
    play_circ.m_speedirection = sf::Vector2f(0.0f, 0.0f);
            
}

void moving_circle(player_circle& play_circ, const sf::Vector2f& move)
{
    
    play_circ.set_position(play_circ.m_position + move);
    
}

void move_circle(player_circle& play_circ)
{
    
    moving_circle(play_circ, play_circ.m_speedirection);
    
}

void show_player_circle(sf::RenderWindow& window, player_circle& play_circ)
{
    
    play_circ.rescale_rect();
    
    window.draw(play_circ.m_circle);
    
    window.draw(play_circ.m_rectangle);
    
}

struct object_circle
{
    
    const float m_radius{5.0f};
    
    const sf::Color m_color{127, 127, 127};
    
    sf::Vector2f m_position{0.0f, 0.0f};
    
    sf::Vector2f m_speedirection{0.0f, 0.0f};
    
    sf::CircleShape m_circle{m_radius};

    const float m_mass{1.3333f*3.14159f*m_radius*m_radius*m_radius};
    
    object_circle(const float radius, const sf::Color& color, const sf::Vector2f& position);
    
    ~object_circle();
    
};

object_circle::object_circle(const float radius, const sf::Color& color, const sf::Vector2f& position)
    :m_radius(radius), m_color(color), m_position(position), m_circle(m_radius),
    m_mass(1.3333*3.14159*m_radius*m_radius*m_radius)
{
    
    assert(radius > 0.0f);
    
    m_circle.setOrigin(m_radius, m_radius);
    m_circle.setPosition(m_position);
    m_circle.setFillColor(m_color);
        
}

object_circle::~object_circle()
{
      
}

void show_object_circle(sf::RenderWindow& window, const object_circle& obje_circ)
{
    
    window.draw(obje_circ.m_circle);
    
}

sf::Vector2f distance(player_circle& play_circ, const object_circle& obje_circ)
{
    
    return play_circ.m_position - obje_circ.m_position;
    
}

float abs_vector2f(const sf::Vector2f& vect)
{
    
    return static_cast<float>(sqrt(vect.x*vect.x + vect.y*vect.y));
    
}

void accel_gravity(player_circle& play_circ, const object_circle& obje_circ,
                   const sf::Vector2f& dist, const float mass_mult)
{
    
    const float grav_mult{-1e-9f};
    
    const float abs_dist{abs_vector2f(dist)};
    
    const float abs_accel{grav_mult*obje_circ.m_mass*(1 + static_cast<float>(cos(abs_dist/(mass_mult*obje_circ.m_mass))))};
    
    play_circ.m_accelerection = play_circ.m_accelerection + sf::Vector2f(abs_accel*dist.x/abs_dist, abs_accel*dist.y/abs_dist);
    
}
                       

void object_gravity(player_circle& play_circ, const object_circle& obje_circ)
{
    
    const float PI{3.14159f};

    const sf::Vector2f dist{distance(play_circ, obje_circ)};
    
    const float mass_mult{1e-3f};
        
    if (abs_vector2f(dist) < PI*mass_mult*obje_circ.m_mass)
    {
        
        accel_gravity(play_circ, obje_circ, dist, mass_mult);
        
    }
    
}

void speed_accel(player_circle& play_circ, const float delta_delaz)
{
    
    assert(delta_delaz > 0);
    
    play_circ.m_speedirection = play_circ.m_speedirection + delta_delaz*play_circ.m_accelerection;
    
    play_circ.m_accelerection = sf::Vector2f{0.0f, 0.0f};
    
}

int collision_check(player_circle& play_circ, const object_circle& obje_circ)
{
   
    const sf::Vector2f dist{distance(play_circ, obje_circ)};
    

    const float abs_dist{abs_vector2f(dist)};
    
    const float play_radi{play_circ.m_radius};
    const float obje_radi{obje_circ.m_radius};
    
    const float total_radi{play_radi + obje_radi};
    
    const float nu_mult{1.0001f};
                                            
    if (abs_dist < total_radi)
    {
        
        const sf::Vector2f nu_dist{nu_mult*dist.x*total_radi/abs_dist, nu_mult*dist.y*total_radi/abs_dist};
        
        stop_circle(play_circ, obje_circ.m_position + nu_dist);
        
        return 1;
        
    }   
    
    return 0;
    
}

void collision_color(player_circle& play_circ, const object_circle& obje_circ,
                     const sf::Color& colli_color, const sf::Color apart_color,
                     const float delta_fuel)
{
    
    assert(delta_fuel > 0);
    
    const float fuel_mult{20.0f};
    
    if (collision_check(play_circ, obje_circ) != 0)
    {
        
        const float max_fuel{1.0f};
        
        recolor_player_circle(play_circ, colli_color);
        
        if (play_circ.m_fuel < max_fuel)
        {
			
			play_circ.m_fuel += fuel_mult*delta_fuel;
			
		}
		
		if (play_circ.m_fuel > max_fuel)
		{
			
			play_circ.m_fuel = max_fuel;
			
		}		
        
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
    
    moving_circle(play_circ, move);
        
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
                         const float decay_mult, const float delta_fuel)
{
	
	assert(delta_fuel > 0);
    
    assert(speed_mult > 0.0f);    
    assert(speed_mult < 1.0f);
    
    assert(decay_mult > 0.0f);    
    assert(decay_mult < 1.0f);
    
    // play_circ.m_speedirection = decay_mult*play_circ.m_speedirection;

    
    if (left_mouse_click())
    {
        
        if (play_circ.m_fuel > 0)
        {
        
			play_circ.m_speedirection = play_circ.m_speedirection + 
										(speed_mult/abs_vector2f(mouse_position(window) - play_circ.m_position))*
										(mouse_position(window) - play_circ.m_position);
			
			play_circ.m_fuel -= delta_fuel;
										
		}
		
		if (play_circ.m_fuel < 0)
        {
			
			play_circ.m_fuel = 0;
			
		}
    }
    
    move_circle(play_circ);
    
}


int main()
{
    
    const std::string program_name{"Boussigon V0.7"};
    
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
    const float delta_delaz{delaz/static_cast<float>(count_max)};
    
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
    
    const float delta_fuel{0.00025f};
    
    const float circ_radius{5.0f};    
    const sf::Color circ_color{purple};
    const sf::Color colli_color{orange};    
    const sf::Vector2f circ_position{0.3f*window_x, 0.5f*window_y};    
    player_circle player{circ_radius, circ_color, circ_position};
    
    const float init_speed_mult{0.004};
    
    const float speed_mult{init_speed_mult/static_cast<float>(count_max)};
    const float speed_to_decay{0.5f};
    const float decay_mult{1.0f - speed_to_decay*speed_mult};
    
    const float rect_radius{25.0f};    
    const sf::Color rect_color{green};    
    const sf::Vector2f rect_position{0.5f*window_x, 0.5f*window_y};    
    object_circle object{rect_radius, rect_color, rect_position};
        
    while (window.isOpen())
    {

        sf::Event event;
        
        window.clear(black);        
        
        show_object_circle(window, object);
        
        show_player_circle(window, player);
        
        window.display();
        
        std::this_thread::sleep_for(delay);
        
        for (int count{0}; count < count_max; ++count)
        {
        
            // move_dir(player, delta_dist);
            
            object_gravity(player, object);
            
            speed_accel(player, delta_delaz);
            
            mouse_speedirection(window, player, speed_mult, decay_mult, delta_fuel);
            
            
                    
        }
        
        collision_color(player, object, colli_color, circ_color, delta_fuel);
                    
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
