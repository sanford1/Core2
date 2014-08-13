#include "BulletLuaManager.hpp"

#include "../../Utils/Log.hpp"

float BulletLuaManager::rank = 0.8;

BulletLuaManager::BulletLuaManager(unsigned int initialCapacity)
{
    increaseCapacity(initialCapacity);
}

BulletLuaManager::~BulletLuaManager()
{
    for (auto iter = blocks.begin(); iter != blocks.end(); ++iter)
    {
        delete [] *iter;
    }
}

void BulletLuaManager::createBullet(const std::string& filename, Mover* origin, Mover* target)
{
    BulletLua* b = getFreeBullet();
    b->set(filename, origin, target, this);
    bullets.push_back(b);
}

void BulletLuaManager::createBullet(std::shared_ptr<sol::state> lua, const std::string& func, Mover* origin, Mover* target)
{
    BulletLua* b = getFreeBullet();
    b->set(lua, func, origin, target, this);
    bullets.push_back(b);
}

void BulletLuaManager::createBullet(std::shared_ptr<sol::state> lua, const std::string& func,
                  double x, double y, double d, double s, Mover* target)
{
    BulletLua* b = getFreeBullet();
    b->set(lua, func, x, y, d, s, target, this);
    bullets.push_back(b);
}

void BulletLuaManager::tick()
{
    for (auto iter = bullets.begin(); iter != bullets.end(); ++iter)
    {
        BulletLua* bullet = *iter;

        if (bullet->isDead())
        {
            freeBullets.push(bullet);
            iter = bullets.erase(iter);
        }
        else
        {
            bullet->run();
        }
    }
}

void BulletLuaManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& bullet : bullets)
    {
        target.draw(bullet->getMover());
    }
}

unsigned int BulletLuaManager::bulletCount() const
{
    return bullets.size();
}

unsigned int BulletLuaManager::freeCount() const
{
    return freeBullets.size();
}

unsigned int BulletLuaManager::blockCount() const
{
    return blocks.size();
}

BulletLua* BulletLuaManager::getFreeBullet()
{
    if (freeBullets.empty())
        increaseCapacity();

    BulletLua* bullet = freeBullets.top();
    freeBullets.pop();

    return bullet;
}

void BulletLuaManager::increaseCapacity(unsigned int blockSize)
{
    blocks.push_back(new BulletLua[blockSize]);

    // Throw all bullets into free stack
    for (unsigned int i = 0; i < blockSize; ++i)
    {
        freeBullets.push(&blocks.back()[i]);
    }
}
