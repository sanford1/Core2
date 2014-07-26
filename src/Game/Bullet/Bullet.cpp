#include "Bullet.hpp"
#include "Mover.hpp"
#include "BulletManager.hpp"

#include "../../Math/Math.hpp"

int BulletCommand::turn = 0;

BulletCommand::BulletCommand(BulletMLParser* parser, Mover* bullet, Mover* target, BulletManager* owner)
: BulletMLRunner(parser), mMover(bullet), mTarget(target), mOwner(owner)
{}

BulletCommand::BulletCommand(BulletMLState* state, Mover* bullet, Mover* target, BulletManager* owner)
: BulletMLRunner(state), mMover(bullet), mTarget(target), mOwner(owner)

{}

double BulletCommand::getBulletDirection() 
{
    return mMover->d;
}

double BulletCommand::getAimDirection() 
{
    return Math::radToDeg(M_PI-atan2(mTarget->x - mMover->x, mTarget->y - mMover->y));
}

double BulletCommand::getBulletSpeed() 
{
    return mMover->s;
}

double BulletCommand::getDefaultSpeed() 
{
    return 1.0;
}

double BulletCommand::getRank() 
{
    return 0.5;
}

void BulletCommand::createSimpleBullet(double direction, double speed) 
{
    mOwner->createProjectile(mMover->x, mMover->y, Math::degToRad(direction), speed);
}

void BulletCommand::createBullet(BulletMLState* state, double direction, double speed)
{
    mOwner->createBullet(state, mMover->x, mMover->y, Math::degToRad(direction), speed, mTarget);
}

int BulletCommand::getTurn() 
{
    return BulletCommand::turn;
}

void BulletCommand::doVanish() 
{
    mMover->dead = true;
}

void BulletCommand::doChangeDirection(double direction) 
{
    mMover->d = Math::degToRad(direction);
}

void BulletCommand::doChangeSpeed(double speed) 
{
    mMover->s = speed;
}

void BulletCommand::doAccelX(double speedx) 
{
    double sx = getBulletSpeedX();
    double sy = getBulletSpeedY();
    sx = speedx;
    mMover->d = atan2(sy, sx);
    mMover->s = sqrt(sx*sx+sy*sy);
}

void BulletCommand::doAccelY(double speedy) 
{
    double sx = getBulletSpeedX();
    double sy = getBulletSpeedY();
    sy = speedy;
    mMover->d = atan2(sy, sx);
    mMover->s = sqrt(sx*sx+sy*sy);
}

double BulletCommand::getBulletSpeedX()
{
    return mMover->s * sin(mMover->d);
}

double BulletCommand::getBulletSpeedY() 
{
    return -mMover->s * cos(mMover->d);
}

bool BulletCommand::isDead()
{
    return mMover->dead || isEnd();
}

