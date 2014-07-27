#include "BulletManager.hpp"
#include "Bullet.hpp"

BulletManager::BulletManager()
{
}

BulletCommand* BulletManager::createBullet(BulletMLState* state, double x, double y, double d, double s, Mover* target)
{
    Mover* shot_ = createProjectile(x, y, d, s);
    BulletCommand* ret;
    ret = new BulletCommand(state, shot_, target, this);
    mCommands.push_back(ret);
    return ret;
}

BulletCommand* BulletManager::createBullet(BulletMLParser* parser, Mover* origin, Mover* target)
{
    BulletCommand* ret;
    ret = new BulletCommand(parser, origin, target, this);
    mCommands.push_back(ret);
    return ret;
}

void BulletManager::tick()
{
    uint size = mShots.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        if (mShots[i]->dead)
        {
            mPool.push_back(mShots[i]);
            mShots[i] = mShots.back();
            mShots.pop_back();
            --size;
        }
        if (i < size)
        {
            mShots[i]->tick();
        }
    }

    size = mCommands.size();
    for (unsigned int i = 0; i < size; ++i)
    {
        if (mCommands[i]->isDead())
        {
            delete mCommands[i];
            mCommands[i] = mCommands.back();
            mCommands.pop_back();
            --size;
        }
        if (i < size) mCommands[i]->run();
    }

    BulletCommand::turn++;
}

void BulletManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (Shot* shot : mShots)
    {
        if (!shot->dead)
            target.draw(*shot);
    }
}

Shot* BulletManager::createProjectile(double x, double y, double d, double s)
{
    Shot* ret;
    if (mPool.empty())
    {
        ret = new Shot(x, y, d, s);
    }
    else
    {
        ret = mPool.back();
        ret->x = x;
        ret->y = y;
        ret->d = d;
        ret->s = s;
        mPool.pop_back();
    }
    ret->dead = false;
    mShots.push_back(ret);
    return ret;
}

void BulletManager::destroyProjectile(Shot* projectile)
{
    projectile->dead = true;
}

void BulletManager::clearAll()
{
    for (BulletCommand* command : mCommands)
    {
        delete command;
    }
    mCommands.clear();

    for (Shot* shot : mShots)
    {
        delete shot;
    }
    mShots.clear();
}

/* void BulletManager::checkCollision(Ship* target, double hitDistSq) */
/* { */
/*     double nx; */
/*     double ny; */

/*     double dseg; */
/*     double dline; */
/*     for (uint i = 0; i < mShots.size(); ++i) */
/*     { */
/*         nx = mShots[i]->x + (mShots[i]->s * sin(mShots[i]->d)); */
/*         ny = mShots[i]->y + (-mShots[i]->s * cos(mShots[i]->d)); */

/*         distanceFromLine(target->x, target->y, mShots[i]->x, mShots[i]->y, nx, ny, dseg, dline); */
/*         if (dseg <= hitDistSq) */
/*         { */
/*             target->hit(mShots[i]); */
/*         } */
/*     } */
/* } */
