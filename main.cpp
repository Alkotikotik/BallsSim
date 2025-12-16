#include <iostream>
#include <cmath>
#include "raylib.h"
#include <cassert>
#include <utility>
#include <vector>
#include <string>
#include <array>
#include <raymath.h>
using namespace std;

// The ball class
class Ball
{
public:
    Vector2 pos;
    Vector2 vel;
    double mass;
    float radiusVisual;
    Color color;
    float e; //bounce coefficient
    double g;

    // Drag parameters
    float realRadius;   
    double SphereDC = 0.47;
    double airDensity = 1.225;
    double A; 

    int planet = 3;
    string planet_name = "Earth";
    Vector2 windAcc;


    // Inside class Ball
    Ball(Vector2 Pos,
        Vector2 Vel,
        double mass,
        double radius,
        Color color,
        float restitution, 
        double g = 98.1f)
        : pos(Pos), vel(Vel), mass(mass), radiusVisual(radius),
        color(color), e(restitution), g(g) 
    {
        realRadius = radiusVisual / 10000.0f;
        A = 3.1415 * realRadius * realRadius;
    }

    struct Derivative
    {
        Vector2 dpos; //dpos is derivate of position - velocity
        Vector2 dvel; //dvel is derivate of velocity - acceleration
    };



    double terVel() const
    {
        double top = 2 * mass * g;
        double bottom = airDensity * SphereDC * A;

        return sqrt(top / bottom);
    }

    Derivative accel(const Vector2& pos, const Vector2& vel, Vector2 windAcc)
    {
        // Again split equation to simplify
        double gravityForce = mass * g;
        double v = vel.y;
        double dragForce = 0.5 * airDensity * SphereDC * A * v * fabs(v);

        // drag direction is opposite velocity
        dragForce *= -1;


        double net = gravityForce + dragForce;

        double ay = net / mass; //ay - acceleration on y axis, so basially falling

        return { vel, { windAcc.x, (float)ay + windAcc.y} };

    }

    void rk4(float dt, Vector2 windAcc)
    {
        Derivative k1 = accel(pos, vel, windAcc);

        Derivative k2 = accel(
            pos + k1.dpos * (dt * 0.5f),
            vel + k1.dvel * (dt * 0.5f),
            windAcc
        );

        Derivative k3 = accel(
            pos + k2.dpos * (dt * 0.5f),
            vel + k2.dvel * (dt * 0.5f),
            windAcc
        );

        Derivative k4 = accel(
            pos + k3.dpos * dt,
            vel + k3.dvel * dt,
            windAcc
        );

        pos += (k1.dpos + k2.dpos * 2 + k3.dpos * 2 + k4.dpos) * (dt / 6.0f);
        vel += (k1.dvel + k2.dvel * 2 + k3.dvel * 2 + k4.dvel) * (dt / 6.0f);
    }

    void CollisionHandling(float halfHeight, float halfWidth)
    {

        float stopSpeed = 50.0f;
        float friction = 0.8f;
        //Floor
        if (pos.y + radiusVisual > halfHeight)
        {
            pos.y = halfHeight - radiusVisual;
            if (abs(vel.y) < stopSpeed)
            {
                vel.y = 0;
            }
            else
            {
                vel.y *= -e;
            }
        }

        //Ceiling
        else if (pos.y - radiusVisual < -halfHeight)
        {
            pos.y = -halfHeight + radiusVisual;
            if (abs(vel.y) < stopSpeed)
            {
                vel.y = 0;
            }
            else
            {
                vel.y *= -e;
            }
        }
        


        //Right Wall
        if (pos.x + radiusVisual > halfWidth)
        {
            pos.x = halfWidth - radiusVisual;
            vel.x *= -e;
        }
        //Left Wall
        else if (pos.x - radiusVisual < -halfWidth)
        {
            pos.x = -halfWidth + radiusVisual;
            vel.x *= -e;
        }

        
    }

    std::string velString() const {
        return "Vel: (" + std::to_string(vel.x) + ", " + std::to_string(vel.y) + ")";
    }

    std::string posString() const {
        return "Pos: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ")";
    }




    void draw() 
    { 

        //float dt = GetFrameTime();
        float dt = 0.07;
        rk4(dt,windAcc);
        DrawCircle((int)pos.x, (int)pos.y, radiusVisual, color);

        // Pass the floor height to collision handler
        CollisionHandling(540, 960);


        //DrawText(velString().c_str(), 600, -500, 20, RAYWHITE);
        //DrawText(posString().c_str(), 600, -400, 20, RAYWHITE);
    }


private:
    double drag = 0.1; 
};


// --- BALLS SYSTEM CLASS ---


class BallSys
{
public:
    vector<Ball> balls;

    int current_planet = 3;
    char* current_planet_name = "Earth";
    double current_g;
    double current_airDensity;

    Vector2 wind = { -10.0f, 5.0f };



    BallSys(std::vector<Ball> initialBalls)
        : balls(initialBalls)
    {

    }

    float randf(float min, float max) {
        return min + (float)GetRandomValue(0, 10000) / 10000.0f * (max - min);
    }

    
    void addOnClick(Vector2 position)
    {
        // Asked AI to create this array
        Color allColors[25] = {
            LIGHTGRAY,  // 0
            GRAY,       // 1
            DARKGRAY,   // 2
            YELLOW,     // 3
            GOLD,       // 4
            ORANGE,     // 5
            PINK,       // 6
            RED,        // 7
            MAROON,     // 8
            GREEN,      // 9
            LIME,       // 10
            DARKGREEN,  // 11
            SKYBLUE,    // 12
            BLUE,       // 13
            DARKBLUE,   // 14
            PURPLE,     // 15
            VIOLET,     // 16
            DARKPURPLE, // 17
            BEIGE,      // 18
            BROWN,      // 19
            DARKBROWN,  // 20
            WHITE,      // 21
            BLANK,      // 22
            MAGENTA,    // 23
            RAYWHITE    // 24
        };
        
        Vector2 random_vel = { randf(-100.0f, 100.0f), randf(-100.0f, 100.0f)};
        float random_m = randf(1.0f, 75.0f);
        float random_r = randf(1.0f, 75.0f);
        balls.push_back(Ball(position, random_vel, random_m, random_r, allColors[GetRandomValue(0, 25)], randf(0.8, 1)));

    }



    void ballsCollision()
    {
        for (int i = 0; i < balls.size(); ++i)
        {
            for (int j = i + 1; j < balls.size(); ++j)
            {
                Vector2 distanceVec = Vector2Subtract(balls[j].pos, balls[i].pos);

                double dist = Vector2Length(distanceVec);

                Vector2 normal = { 0.0f, 0.0f }; 
                //Avoid devision by 0
                if (dist > 0.001f) 
                {
                   
                    normal.x = distanceVec.x / dist;
                    normal.y = distanceVec.y / dist;
                }
                else
                {

                    normal.x = 1.0f;
                    normal.y = 0.0f;
                }

                float radii_sum = balls[i].radiusVisual + balls[j].radiusVisual;
                double overlap = (radii_sum - dist) / 2;

                Vector2 relative_vel = balls[j].vel - balls[i].vel;
                double speed_normal = Vector2DotProduct(relative_vel, normal);

                float average_e = (balls[i].e + balls[j].e) / 2;

                if ((dist * dist) < (radii_sum * radii_sum))
                {
                    // UNSTUCK
                    balls[i].pos = Vector2Subtract(balls[i].pos, Vector2Scale(normal, overlap));
                    balls[j].pos = Vector2Add(balls[j].pos, Vector2Scale(normal, overlap));

                    if (speed_normal > 0)
                        continue;
                    else
                    {
                        double top_fraction = -(1 + average_e) * speed_normal;
                        double bottom_fraction = 1 / balls[i].mass + 1 / balls[j].mass;
                        double impulseScalar = top_fraction / bottom_fraction; //Impulse value

                        Vector2 impulseVector = Vector2Scale(normal, impulseScalar); //Impuse value with direction

                        balls[i].vel = Vector2Subtract(balls[i].vel, Vector2Scale(impulseVector, 1.0f / balls[i].mass));
                        balls[j].vel = Vector2Add(balls[j].vel, Vector2Scale(impulseVector, 1.0f / balls[j].mass));


                    }
                    

                }

            }
        }


    }

    void attractionPoint(Vector2 attPoint_pos)
    {

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) 
        {

            for (int i = 0; i < balls.size(); ++i)
            {
                Vector2 distanceVec = Vector2Subtract(balls[i].pos, attPoint_pos);

                double dist = Vector2Length(distanceVec);

                Vector2 normal = { 0.0f, 0.0f };
                //Avoid devision by 0
                if (dist > 0.001f)
                {

                    normal.x = distanceVec.x / dist;
                    normal.y = distanceVec.y / dist;
                }
                else
                {

                    normal.x = 1.0f;
                    normal.y = 0.0f;
                }

                double speed_normal = Vector2DotProduct(balls[i].vel, normal);

                float direction = IsKeyDown(KEY_LEFT_SHIFT) ? -1.0f : 1.0f;
                Vector2 impulseVector = Vector2Scale(normal, direction*1000.0f); // 100 is just a value
 
                balls[i].vel = Vector2Subtract(balls[i].vel, Vector2Scale(impulseVector, 1.0f / balls[i].mass));

            }
        }

    }


    void changeEnviroment()
    {

        if (IsKeyPressed(KEY_ONE))   current_planet = 1;
        if (IsKeyPressed(KEY_TWO))   current_planet = 2;
        if (IsKeyPressed(KEY_THREE)) current_planet = 3;
        if (IsKeyPressed(KEY_FOUR))  current_planet = 4;
        if (IsKeyPressed(KEY_FIVE))  current_planet = 5;
        if (IsKeyPressed(KEY_SIX))   current_planet = 6;
        if (IsKeyPressed(KEY_SEVEN)) current_planet = 7;
        if (IsKeyPressed(KEY_EIGHT)) current_planet = 8;
        if (IsKeyPressed(KEY_NINE))  current_planet = 9;
        if (IsKeyPressed(KEY_ZERO))  current_planet = 10;

        switch (current_planet)
        {
        case 1:
            current_g = 38;
            current_airDensity = 0;
            current_planet_name = "Mercury";
            break;

        case 2:
            current_g = 90;
            current_airDensity = 65;
            current_planet_name = "Venus";
            break;

        case 3:
            current_g = 98.1;
            current_airDensity = 1.225;
            current_planet_name = "Earth";
            break;

        case 4:
            current_g = 37;
            current_airDensity = 0.02;
            current_planet_name = "Mars";
            break;

        case 5:
            current_g = 240.8;
            current_airDensity = 0.16;
            current_planet_name = "Jupiter";
            break;

        case 6:
            current_g = 104.4;
            current_airDensity = 0.19;
            current_planet_name = "Saturn";
            break;

        case 7:
            current_g = 87.3;
            current_airDensity = 0.42;
            current_planet_name = "Uranus";
            break;

        case 8:
            current_g = 111.8;
            current_airDensity = 0.45;
            current_planet_name = "Neptune";
            break;

        case 9:
            current_g = 16.25;
            current_airDensity = 0;
            current_planet_name = "Moon";
            break;

        case 10:
            current_g = 2740;
            current_airDensity = 0.0001;
            current_planet_name = "Sun";
            break;

        default:
            current_g = 98.1;
            current_airDensity = 1.225;
            current_planet_name = "Earth";
            break;
        }

        for (auto& b : balls)
        {
            b.g = current_g;
            b.airDensity = current_airDensity;
            
        }

        


    }
    void drawUI()
    {

        string g_str = to_string(current_g / 10.0) + " m/s^2";
        string density_str = to_string(current_airDensity) + " kg/m^3";
        string balls_num = to_string(balls.size());
        string wind_str_x = to_string((int)wind.x);
        string wind_str_y = to_string((int)wind.x);
        string wind_str = "x: "+wind_str_x + "    y: " + wind_str_y+"    (w) to change mode";

        DrawText("Currently you are on:", -950, -500, 20, WHITE);
        DrawText(current_planet_name, -700, -500, 20, WHITE);

        DrawText("Gravitational acceleration:", -950, -450, 20, WHITE);
        DrawText(g_str.c_str(), -650, -450, 20, WHITE);

        DrawText("Air density (surface level):", -950, -400, 20, WHITE);
        DrawText(density_str.c_str(), -650, -400, 20, WHITE);

        DrawText("Number of balls is:", -950, -350, 20, WHITE);
        DrawText(balls_num.c_str(), -750, -350, 20, WHITE);

        DrawText("Current wind is:", -950, -300, 20, WHITE);
        DrawText(wind_str.c_str(), -750, -300, 20, WHITE);

    }

    void changeWind()
    {
        static int is_enabled = 0;

        if (IsKeyPressed(KEY_W))
            is_enabled++;

        if (is_enabled > 2)
            is_enabled = 0;

        switch (is_enabled)
        {
        case 0:
            wind = { 0, 0 };
            break;

        case 1:
            wind = { 15.0f, 3.0f };
            break;

        case 2:
        {
            float t = GetTime();
            wind.x = sin(t * 0.5f) * 15.0f;
            wind.y = 0;
        }
        break;
        }
    }


    void draw()
    {
        changeEnviroment();
        drawUI();
        ballsCollision();
        changeWind();


        for (auto& b : balls) 
        {
            b.g = current_g;
            b.airDensity = current_airDensity;
            b.windAcc = wind;
            b.draw();
        }
    }

    

};



    

int main()
{

    Ball OneBall
    (
        { 0, -500 }, //Start pos
        { 0, 0}, //Start vel
        5.0f, //Mass
        10.0f, //radius
        ORANGE, //Color
        98.1f //10x g on the Earth
    );
    cout << OneBall.terVel() << endl;

    BallSys ballSystem
    (
        {}

    );

    const int ScreenWidth = 1920;
    const int ScreenHight = 1080;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(ScreenWidth, ScreenHight, "Falling ball");

    Camera2D camera2D = { 0 };
    camera2D.target = { 0.0f, 0.0f };
    camera2D.offset = { ScreenWidth / 2.0f, ScreenHight / 2.0f };
    camera2D.rotation = 0.0f;
    camera2D.zoom = 1.0f;

    SetTargetFPS(120);

    //DisableCursor();

    float radius = 2000.0f;

    while (!WindowShouldClose())
    {

        //camera2D.zoom += wheelMove * 0.1f;


        if (camera2D.zoom < 0.1f) camera2D.zoom = 0.1f;

        Vector2 screenPosition = GetMousePosition();
        Vector2 worldPosition = GetScreenToWorld2D(screenPosition, camera2D);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ballSystem.addOnClick(worldPosition);
        }
        ballSystem.attractionPoint(worldPosition);

        if (IsKeyPressed(KEY_R)) {
            ballSystem.balls.clear();
        }

        BeginDrawing();

        ClearBackground(BLACK);

        
        BeginMode2D(camera2D);
        ballSystem.draw();
        EndMode2D();
        
        

        DrawFPS(10, 10);

        EndDrawing();
    }

    CloseWindow();
    cout << OneBall.terVel() << endl;
    return 0;
}
