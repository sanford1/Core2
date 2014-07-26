#include "PatternState.hpp"
#include "../System/ResourceHolder.hpp"
#include "../System/ResourceIdentifiers.hpp"
#include "../Utils/Log.hpp"

#include "Bullet/Bullet.hpp"

PatternState::PatternState(StateStack& stack, Context context)
    : State(stack, context)
{
    try
    {
        // context.fonts->load(Res::Fonts::normal,    "data/fonts/DroidSansFallback.ttf");
        // context.textures->load(Res::Textures::bg1, "data/bg/Frac2.png");
        // context.textures->load(Res::Textures::bg2, "data/bg/Frac3.png");
        // context.music->addSong(Res::Music::main,   "data/music/ReflectionEternal.ogg");
    }
    catch (std::runtime_error& e)
    {
        Console::logf("Resource loading block failed: %s", e.what());
    }

    parsers.push_back(make_unique<BulletMLParserTinyXML>("data/pattern/10flower.xml"));
    parsers.push_back(make_unique<BulletMLParserTinyXML>("data/pattern/circle_fireworks.xml"));
    parsers.push_back(make_unique<BulletMLParserTinyXML>("data/pattern/hakkyou.xml"));
    parsers.push_back(make_unique<BulletMLParserTinyXML>("data/pattern/otk2-hanabi.xml"));
    parsers.push_back(make_unique<BulletMLParserTinyXML>("data/pattern/self-mis02.xml"));

    for (auto& parser : parsers)
        parser->parse();

    ship = make_unique<Mover>(320, 120, 0, 0);
    boss = make_unique<Mover>(320, 370, 0, 0);
    ship->dead = false;
    boss->dead = false;
}

void PatternState::draw()
{
    sf::RenderWindow& window = *(getContext().window);
    window.draw(manager);
    window.draw(*ship);
    window.draw(*boss);
}

bool PatternState::update(sf::Time dt)
{
    manager.tick();
    return false;
}

bool PatternState::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Q)
            manager.createBullet(parsers[0].get(), ship.get(), ship.get());
        if (event.key.code == sf::Keyboard::W)
            manager.createBullet(parsers[1].get(), ship.get(), boss.get());
        if (event.key.code == sf::Keyboard::E)
            manager.createBullet(parsers[2].get(), ship.get(), boss.get());
        if (event.key.code == sf::Keyboard::R)
            manager.createBullet(parsers[3].get(), ship.get(), boss.get());
        if (event.key.code == sf::Keyboard::T)
            manager.createBullet(parsers[4].get(), ship.get(), boss.get());

        Console::logf("%d %d %d", BulletCommand::turn, manager.mCommands.size(), manager.mShots.size());
    }

    return false;
}

