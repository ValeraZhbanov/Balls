#pragma once
#include <Windows.h>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include "Settings.h"

INT MaxScore;

class GameObject {
protected:
    POINT point;

public:
    GameObject(POINT point) : point(point) {}

    virtual void Paint() = 0;
    virtual void Clear() = 0;
    virtual void Move(POINT PT) = 0;

    static DOUBLE GetDistance(GameObject & first, GameObject & second) {
        return std::hypot(first.point.x - second.point.x, first.point.y - second.point.y);
    }

    static DOUBLE GetDistance(POINT & point, GameObject & object) {
        return std::hypot(point.x - object.point.x, point.y - object.point.y);
    }

    operator POINT() {
        return point;
    }
};

class Player : public GameObject {

public:
    POINT Purpose;
    Player(POINT point) : GameObject(point), Purpose() {}

    virtual void Paint() {
        auto r = GameSetting.Player.Size;
        SelectObject(Window, GameSetting.Player.BodyColor);
        Ellipse(Window, point.x - r, point.y - r, point.x + r, point.y + r);
    }

    virtual void Clear() {
        auto r = GameSetting.Player.Size;
        SelectObject(Window, GameSetting.Battlefield.Color);
        Ellipse(Window, point.x - r, point.y - r, point.x + r, point.y + r);
    }

    virtual void Move(POINT PT) {
        auto dx = (DOUBLE)Purpose.x - point.x;
        auto dy = (DOUBLE)Purpose.y - point.y;
        auto m = std::hypot(dx, dy);
        auto speed = GameSetting.Player.Speed.Max;
        auto size = GameSetting.Player.Size;
        auto RT = Window.RT;
        if(GameSetting.Player.Size < m) {
            dx /= m, dx *= speed;
            dy /= m, dy *= speed;
            PT = {point.x + (INT)dx, point.y + (INT)dy};
            if(PT.x < RT.left + size)
                PT.x = RT.right - size;
            else if(RT.right - size < PT.x)
                PT.x = RT.left + size;
            else if(PT.y < RT.top + size)
                PT.y = RT.bottom - size;
            else if(RT.bottom - size < PT.y)
                PT.y = RT.top + size;
            Clear();
            point = PT;
        }
    }
};

class Enemy : public GameObject {
    INT Speed;
public:
    Enemy(POINT point) : GameObject(point), Speed(rnd(GameSetting.Enemy.Speed.Min, GameSetting.Enemy.Speed.Max)) {}

    virtual void Paint() {
        auto r = GameSetting.Enemy.Size;
        SelectObject(Window, GameSetting.Enemy.BodyColor);
        Ellipse(Window, point.x - r, point.y - r, point.x + r, point.y + r);
    }

    virtual void Clear() {
        auto r = GameSetting.Enemy.Size;
        SelectObject(Window, GameSetting.Battlefield.Color);
        Ellipse(Window, point.x - r, point.y - r, point.x + r, point.y + r);
    }

    virtual void Move(POINT p) {
        auto dx = (DOUBLE)p.x - point.x;
        auto dy = (DOUBLE)p.y - point.y;
        auto m = std::hypot(dx, dy);
        auto speed = Speed;
        if(GameSetting.Enemy.Size < m) {
            dx /= m, dx *= speed;
            dy /= m, dy *= speed;
            p = {point.x + (INT)dx, point.y + (INT)dy};
            if(PtInRect(&Window.RT, p)) {
                Clear();
                point = p;
                Paint();
            }
        }
    }

    INT Score() {
        auto enemy = 0.0
            + Speed
            - GameSetting.Enemy.Size / 10
            + GameSetting.Enemy.Bullet.ChanceShoot
            + GameSetting.Enemy.Bullet.Size
            + GameSetting.Enemy.Bullet.Speed
            + GameSetting.Enemy.Bullet.Time;
        auto player = 0.0
            - GameSetting.Player.Speed.Max
            + GameSetting.Player.Size
            - GameSetting.Player.Bullet.ChanceShoot
            - GameSetting.Player.Bullet.Size / 10
            - GameSetting.Player.Bullet.Speed
            - GameSetting.Player.Bullet.Time;
        return max(0, (enemy * max(GameSetting.Enemy.Count, 10) + player) / 10);
    }
};

class Bullet : public GameObject {
    Setting::Bullet BulletData;
    POINT prev;
    DOUBLE dx;
    DOUBLE dy;
public:

    INT Time;

    Bullet(POINT point, Setting::Bullet & BulletData, POINT PT) : GameObject(point), prev(), BulletData(BulletData), Time(BulletData.Time) {
        Move(PT);
    }

    virtual void Paint() {
        prev = point;
        point.x += dx, point.y += dy;
        Time--;
        if(!PtInRect(&Window.RT, point))
            dx = -dx, dy = -dy;
        auto r = BulletData.Size;
        SelectObject(Window, BulletData.Color);
        Ellipse(Window, point.x - r, point.y - r, point.x + r, point.y + r);
        SelectObject(Window, GameSetting.Battlefield.Color);
        Ellipse(Window, prev.x - r, prev.y - r, prev.x + r, prev.y + r);
    }

    virtual void Clear() {
        auto r = BulletData.Size;
        SelectObject(Window, GameSetting.Battlefield.Color);
        Ellipse(Window, point.x - r, point.y - r, point.x + r, point.y + r);
    }

    virtual void Move(POINT PT) {
        dx = (DOUBLE)PT.x - point.x;
        dy = (DOUBLE)PT.y - point.y;
        auto m = std::hypot(dx, dy);
        auto speed = BulletData.Speed;
        dx /= m, dx *= speed;
        dy /= m, dy *= speed;
    }
};

struct Game {
    INT Score = 0;
    Player Player;
    std::vector<Bullet> PlayerBullets;

    std::vector<Enemy> Enemys;
    std::vector<Bullet> EnemyBullets;


    Game() : Player({300, 300}) {
        Window.Fill();
    }

    void GameOver(const char * death) {
        if(MaxScore <= Score) {
            MaxScore = Score;
            SetWindowText(Window.Menu.BestScore, std::to_string(MaxScore).c_str());
            SetWindowText(Window.Menu.BestDeath, death);
        }
        SetWindowText(Window.Menu.LastScore, std::to_string(Score).c_str());
        SetWindowText(Window.Menu.LastDeath, death);
        SendMessage(Window, WM_COMMAND, IDC_NEW, 0);
        SendMessage(Window, WM_COMMAND, IDC_PAUSE, 0);
    }

    void Tick() {
        Player.Move({});

        if(Enemys.size() < GameSetting.Enemy.Count) {
            POINT PT;
            while(GameObject::GetDistance(PT = Window.GetRndPoint(), Player) < GameSetting.Player.Size + 300);
            Enemys.push_back(Enemy(PT));
        }

        if(GetMinGistance(Player, EnemyBullets) < GameSetting.Player.Size + GameSetting.Enemy.Bullet.Size) {
            GameOver("Вы погибли от пуль");
        }

        if(GetMinGistanceWithEnemys() < GameSetting.Player.Size + GameSetting.Enemy.Size) {
            GameOver("Вы погибли от избиения");
        }

        if(Enemys.size() && rnd(0, 100) < GameSetting.Player.Bullet.ChanceShoot) {
            PlayerBullets.push_back(Bullet(Player, GameSetting.Player.Bullet, Enemys[rnd(0, Enemys.size())]));
        }

        for(auto & bullet : PlayerBullets) {
            bullet.Paint();
        }

        for(auto & bullet : EnemyBullets) {
            bullet.Paint();
        }

        for(auto & enemy : Enemys) {
            enemy.Move(Player);
            if(rnd(0, 100) < GameSetting.Enemy.Bullet.ChanceShoot) {
                EnemyBullets.push_back(Bullet(enemy, GameSetting.Enemy.Bullet, Player));
            }
        }

        Player.Paint();

        PlayerBullets.erase(std::remove_if(PlayerBullets.begin(), PlayerBullets.end(), EraseBullet), PlayerBullets.end());
        EnemyBullets.erase(std::remove_if(EnemyBullets.begin(), EnemyBullets.end(), EraseBullet), EnemyBullets.end());

        Enemys.erase(std::remove_if(Enemys.begin(), Enemys.end(), [&](Enemy & enemy) {
            if(GetMinGistance(enemy, PlayerBullets) < GameSetting.Player.Size + GameSetting.Enemy.Bullet.Size) {
                enemy.Clear(), Score += enemy.Score();
                return true;
            }
            return false;
        }), Enemys.end());

        std::ostringstream sout;
        sout << "Счёт: " << Score;

        SelectObject(Window, GameSetting.Battlefield.Color);
        Rectangle(Window, Window.RT.right - 100, 10, Window.RT.right - 20, 25);
        TextOut(Window, Window.RT.right - 100, 10, sout.str().c_str(), sout.str().size());

        Window.Paint();
    }

    static bool EraseBullet(Bullet & bullet) {
        if(bullet.Time < 0) {
            bullet.Clear();
            return true;
        }
        return false;
    }

    static INT GetMinGistance(GameObject & object, std::vector<Bullet> bullets) {
        INT result = 0xFFFF;
        for(auto & bullet : bullets)
            result = min(result, GameObject::GetDistance(object, bullet));
        return result;
    }

    INT GetMinGistanceWithEnemys() {
        INT result = 0xFFFF;
        for(auto & enemy : Enemys)
            result = min(result, GameObject::GetDistance(Player, enemy));
        return result;
    }

} CurrentGame;