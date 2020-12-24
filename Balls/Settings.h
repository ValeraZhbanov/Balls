#pragma once
#include <Windows.h>
#include <random>

struct Setting {

    struct Speed {
        INT Min = 0;
        INT Max = 5;
    };

    struct Bullet {
        HBRUSH Color = {};
        INT Size = 10;
        DOUBLE Speed = 10;
        DOUBLE ChanceShoot = 50;
        INT Time = 10;
    };

    struct Battlefield {
        HBRUSH Color = {};
    };

    struct Entity {
        HBRUSH BodyColor = {};
        INT Size = 20;
        INT Count = 1;
        Bullet Bullet;
        Speed Speed;
    };

    struct Status {
        INT Score = 0;
    };

    Battlefield Battlefield;
    Entity Player;
    Entity Enemy;

    Setting() {
        Battlefield.Color = CreateSolidBrush(0x333333);

        Player.BodyColor = CreateSolidBrush(0x00AA00);
        Player.Speed = {0, 10};
        Player.Size = 20;
        Player.Bullet.Color = CreateSolidBrush(0x00FF00);
        Player.Bullet.Speed = 7;
        Player.Bullet.Size = 20;
        Player.Bullet.Time = 450;
        Player.Bullet.ChanceShoot = 3;

        Enemy.Speed = {0, 6};
        Enemy.Size = 20;
        Enemy.Count = 7;
        Enemy.BodyColor = CreateSolidBrush(0xAA0000);
        Enemy.Bullet.Color = CreateSolidBrush(0xFF0000);
        Enemy.Bullet.Speed = 7;
        Enemy.Bullet.Size = 10;
        Enemy.Bullet.Time = 50;
        Enemy.Bullet.ChanceShoot = 0.1;


    }

} GameSetting;

static INT rnd(int x, int y) {
    static std::default_random_engine dre;
    return std::uniform_int_distribution<INT>(x, y - 1)(dre);
}