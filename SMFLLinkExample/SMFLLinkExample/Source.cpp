
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>





using namespace sf;


class Enemy
{
public:
    Sprite shape;

    int HP = 0;
    Vector2f enemySize = Vector2f(0.3, 0.3);

    Enemy(Texture* texture, Vector2u windowSize)
    {
       

        this->shape.setTexture(*texture);

        this->shape.setScale(enemySize);

        this->shape.setPosition((rand()* rand()) % (int)(windowSize.x - this->shape.getGlobalBounds().width),0.f);

        this->shape.setRotation((rand() * rand()) % 360);
    }

    void setEnemySize(Vector2f size)
    {
        this->enemySize = size;
    }

    void setEnemyTexture(Texture* texture)
    {
        this->shape.setTexture(*texture);
    }

    void setEnemyHp(int healtPoint)
    {
        this->HP = healtPoint;
    }
    ~Enemy() {}
};

void time_delay(int delaySec) {

    const int delaySeconds = delaySec; // Delay duration in seconds
    clock_t startTime = clock();

    while (true)
    {
        clock_t currentTime = clock();
        double elapsedTime = double(currentTime - startTime) / CLOCKS_PER_SEC;

        if (elapsedTime >= delaySeconds)
            break;

        // Continue executing the loop until the desired delay duration is reached
    }
    return;
}

class enemyHardness {
public:
    int enemyHealth;
    int enemySpawnDelay;
    Vector2f enemyMovementSpeed;
    Vector2f enemySize1;
    Vector2f enemySize2;

    void initLevel_1Enemy()
    {
        enemyHealth = 1;
        enemySpawnDelay = 70;
        enemyMovementSpeed = Vector2f(0.f, 5.f);
        enemySize1 = Vector2f(0.3,0.3 );
        enemySize2 = Vector2f(0.3,0.3);
    }

    void initLevel_2Enemy()
    {
        enemyHealth = 2;
        enemySpawnDelay = 50;
        enemyMovementSpeed = Vector2f(0.f, 6.f);
        enemySize1 = Vector2f(0.4, 0.4);
        enemySize2 = Vector2f(0.4, 0.4);
    }

    void initLevel_3Enemy()
    {
        enemyHealth = 3;
        enemySpawnDelay = 30;
        enemyMovementSpeed = Vector2f(0.f, 7.f);
        enemySize1 = Vector2f(0.5, 0.5);
        enemySize2 = Vector2f(0.4, 0.4);
    }
};

class ammoCrateStats {
public:
    int refillValue;
    int crateRespawnDelay;
    Vector2f crateSpeed;

    void initLevel1_ammoCrate()
    {
        refillValue = 5;
        crateRespawnDelay = 100;
        crateSpeed = Vector2f(0, 9.f);
    }

    void initLevel2_ammoCrate()
    {
        refillValue = 10;
        crateRespawnDelay = 150;
        crateSpeed = Vector2f(0, 12.f);
    }

    void initLevel3_ammoCrate()
    {
        refillValue = 15;
        crateRespawnDelay = 200;
        crateSpeed = Vector2f(0, 15.f);
    }
};

int main()
{
    RenderWindow window(VideoMode(720,900), "Astro Smash");
    window.setFramerateLimit(60);
    srand(time(NULL));

    //Font Load and Texts Initialization
    Font font;
    if(! font.loadFromFile("Fonts/Dosis-Light.ttf"))
        throw("Could not load file");
    std::string hit_count;

    Text txt;
    txt.setFont(font);
    txt.setCharacterSize(24);
    txt.setFillColor(sf::Color(255, 165, 0,200));
    txt.setStyle(Text::Bold);
    txt.setPosition(Vector2f(10.f,10.f));

    Text HP;
    HP.setFont(font);
    HP.setCharacterSize(16);
    HP.setFillColor(Color::Green);
    HP.setStyle(Text::Regular);

    Text  AmmoTxt;
   AmmoTxt.setFont(font);
   AmmoTxt.setCharacterSize(20);
   AmmoTxt.setFillColor(Color::Yellow);
   AmmoTxt.setStyle(Text::Bold);

    Text gameOverTxt;
    gameOverTxt.setFont(font);
    gameOverTxt.setCharacterSize(30);
    gameOverTxt.setFillColor(Color::Red);
    gameOverTxt.setStyle(Text::Bold);
    gameOverTxt.setPosition(Vector2f(window.getSize().x / 2-80.f, window.getSize().y / 2-20.f));
    gameOverTxt.setString("GAME OVER !!");
    
    Text levelTxt;
    levelTxt.setFont(font);
    levelTxt.setCharacterSize(24);
    levelTxt.setStyle(Text::Bold);
    levelTxt.setPosition(Vector2f(10.f, 35.f));
   
    //--------------------------------------------------------------------

    // Sound load and soundBuffer Init
    // Bkg Game Music
     
    Music gameMusic;

    if(!gameMusic.openFromFile("Sounds/bkgGameMusic.wav"))
        std::cout << "Could not play background music\n";
    gameMusic.play();
    gameMusic.setVolume(70.f);
    //Missile fire sound
    SoundBuffer missileSound;
    Sound missileFire;
    
    if (!missileSound.loadFromFile("Sounds/missileFire.wav"))
        std::cout << "Could not load bullet sound effect\n";
    missileFire.setBuffer(missileSound);
    missileFire.setVolume(85.f);
    missileFire.setPitch(1.f);

    //Collision with asteroid alarm sound
    SoundBuffer collisionSound;
    Sound collisionAlarm;

    if (!collisionSound.loadFromFile("Sounds/collisionSound.wav"))
        std::cout << "Could not load bullet sound effect\n";
    collisionAlarm.setBuffer(collisionSound);
    collisionAlarm.setVolume(50.f);

    //Reload sound
    SoundBuffer ReloadSound;
    Sound reloadSound;

    if (!ReloadSound.loadFromFile("Sounds/reloadSound.wav"))
        std::cout << "Could not load reload sound effect\n";
    reloadSound.setBuffer(ReloadSound);
    reloadSound.setVolume(100.f);
    //reloadSound.setPitch(1.f);

    //Ship Explosion sound
    SoundBuffer explosionSound;
    Sound shipDestroyedSound;

    if (!explosionSound.loadFromFile("Sounds/explode&Over.wav"))
        std::cout << "Could not load explosion sound effect\n";
    shipDestroyedSound.setBuffer(explosionSound);
    shipDestroyedSound.setVolume(100.f);

    //Game Over sound
    SoundBuffer OverSoundBuffer;
    Sound gameOverSound;

    if (!OverSoundBuffer.loadFromFile("Sounds/gameOver.wav"))
        std::cout << "Could not load game sound effect\n";
    gameOverSound.setBuffer(OverSoundBuffer);
    gameOverSound.setVolume(100.f);
    //shipDestroyedSound.setPitch(1.f);

    //repairKit Pickup Sound
    SoundBuffer repairSound;
    Sound repairPickUp;

    if (!repairSound.loadFromFile("Sounds/repairKitSound.wav"))
        std::cout << "Could not load repair pick up sound effect\n";
    repairPickUp.setBuffer(repairSound);
    repairPickUp.setVolume(200.f);
    //shipDestroyedSound.setPitch(1.f);
    


    //----------------------------------------------------------------------
    
    //Projectile
    Texture Bullet;
    Sprite projectile;
    if (!Bullet.loadFromFile("Textures/missileTex01.png"))
        std::cout << "Could not load bullet texture\n";
    projectile.setTexture(Bullet);
    projectile.setScale(Vector2f(0.07, 0.07));

    //AmmoRefil Init
    Texture  ammoCrateTexture;
    Texture  specialAmmoTexture;
    Sprite ammoCrate;

    if (!ammoCrateTexture.loadFromFile("Textures/ammoCrate.png"))
        std::cout << "Could not load ammoCrate texture\n";
    if (!specialAmmoTexture.loadFromFile("Textures/specialAmmo.png"))
        std::cout << "Could not load special Ammo Crate texture\n";
    ammoCrate.setTexture(ammoCrateTexture);
    ammoCrate.setScale(Vector2f(0.09, 0.09));
    std::vector <Sprite> ammoCrates;
    ammoCrates.push_back(Sprite(ammoCrate));

    
    

    //Enemy Init
    Texture asteroid;
    Texture asteroidType2;
    //Sprite enemy;


    //Initial load for level 1
    if (!asteroid.loadFromFile("Textures/asteroid.png"))
        std::cout << "Could not load texture\n";

    if (!asteroidType2.loadFromFile("Textures/asteroidType2.png"))
        std::cout << "Could not load texture\n"; 
    std::vector <Enemy> enemies;

    //Background Init
    Texture SpaceTexture;
    Sprite bkg;

    if (!SpaceTexture.loadFromFile("Textures/SpaceBkg.png"))
        std::cout << "Could not load texture\n";
    bkg.setTexture(SpaceTexture);
    bkg.setScale(Vector2f(1.f, 3.f));

    //Player Init
    Texture ShipTexture;
    Texture destroyedShipTexture;
    Sprite player;

    if (!ShipTexture.loadFromFile("Textures/Ship.png"))
        std::cout << "Could not load texture\n";
    if (!destroyedShipTexture.loadFromFile("Textures/destroyedShip.png"))
        std::cout << "Could not load texture\n";
    player.setTexture(ShipTexture);
    player.setScale(Vector2f(0.15,0.15));
   
    player.setPosition(window.getSize().x / 2 - player.getGlobalBounds().getSize().x, window.getSize().y - player.getGlobalBounds().getSize().y - 10.f);
    Vector2f leftWingPos;
    Vector2f rightWingPos;


    
    //Healthbar Init
    RectangleShape healthbar;
    healthbar.setFillColor(Color::Red);
    healthbar.setSize(Vector2f(100.f, 5.f));
    healthbar.setPosition(window.getSize().x-healthbar.getSize().x-5,10.f);
    //----------HealthBar Background Init-------------------//
    RectangleShape healthbarBkg;
    healthbarBkg.setFillColor(sf::Color(255, 255, 255, 100));
    healthbarBkg.setSize(Vector2f(100.f, 8.f));
    healthbarBkg.setPosition(window.getSize().x - healthbar.getSize().x - 5, 8.5);

    //-----------------------------------------------------------------------------

    //shipRepairKit Init
    Sprite repairKit;
    Texture repairKitTexture;
    if (!repairKitTexture.loadFromFile("Textures/shipRepairKit.png"))
        std::cout << "Could not load repairKit texture\n";
    repairKit.setTexture(repairKitTexture);
    repairKit.setScale(Vector2f(0.07, 0.07));
    int repairVal = 25;

    std::vector <Sprite> repairKits;
    repairKits.push_back(Sprite(repairKit));



    Vector2f healtbarPos;
    healtbarPos.x = window.getSize().x - healthbar.getSize().x - 27;
    healtbarPos.y = 3.f;
    AmmoTxt.setPosition(Vector2f(healtbarPos.x, healtbarPos.y + 15));
    HP.setPosition(healtbarPos);
    HP.setString("HP: ");

    //bool variables
    bool infiniteStatus = false;
    bool gameOver = false;
    bool level_1 = true;
    bool level_2 = false;
    bool level_3 = false;
    bool pushed = false;

    //int variables Init
    int enemy_count = 0;
    int ammo = 10;
    int shootTimerLeft = 0;
    int shootTimerRight = 0;
    int enemySpawnTimer = 0;
    int ammoCrateTimer = 0;
    int specialCrateTimer = 0;
    int repairKitTimer = 0;
    int hits = 0;
    int kitSpawnDelay =  700 + rand() % 100;
    int specialCrateDelay = 1200 + rand() % 300;
    const int infiniteAmmoDuration = 30; // Duration in seconds
    clock_t startTime = clock();

    //int variables for level progressions
    enemyHardness gameEnemyHardness;
    ammoCrateStats gameCrateParam;
   
   
    std::vector <Sprite> projectiles;
    projectiles.push_back(Sprite(projectile));

    while (window.isOpen())
    {
        if (level_1 == true)
        {
            gameEnemyHardness.initLevel_1Enemy();
            gameCrateParam.initLevel1_ammoCrate();
            
        }
        else if (level_2 == true)
        {
            gameEnemyHardness.initLevel_2Enemy();
            gameCrateParam.initLevel2_ammoCrate();

            //Load Level 2 asteroid textures
            if (!asteroid.loadFromFile("Textures/asteroidType3.png"))
                std::cout << "Could not load texture\n";

            if (!asteroidType2.loadFromFile("Textures/asteroidType4.png"))
                std::cout << "Could not load texture\n";
        }
        else if (level_3 == true)
        {
            gameEnemyHardness.initLevel_3Enemy();
            gameCrateParam.initLevel3_ammoCrate();

            //Load Level 3 asteroid textures
            if (!asteroid.loadFromFile("Textures/asteroidType5.png"))
                std::cout << "Could not load texture\n";

            if (!asteroidType2.loadFromFile("Textures/asteroidType6.png"))
                std::cout << "Could not load texture\n";
        }

        if (gameOver == false) {
            gameMusic.setLoop(true);
        }
        else
        {
            gameMusic.stop();
        }

        srand(time(NULL));
            Event event;
            while (window.pollEvent(event))
            {
               if (event.type == Event::Closed)
                   window.close();
            }
        //update
            txt.setPosition(10.f, 10.f);
            //Player
            leftWingPos = Vector2f(player.getPosition().x + 23, player.getPosition().y );
            rightWingPos = Vector2f(player.getPosition().x + 53, player.getPosition().y);
           
            //player.setPosition(Vector2f(Mouse::getPosition(window).x, player.getPosition().y));
           //Player
            if (Keyboard::isKeyPressed(Keyboard::W))
                player.move(0.f, -10.f);
            if (Keyboard::isKeyPressed(Keyboard::A))
                player.move(-10.f, 0.f);
            if (Keyboard::isKeyPressed(Keyboard::S))
                player.move(0.f, 10.f);
            if (Keyboard::isKeyPressed(Keyboard::D))
                player.move(10.f, 0.f);

            //Collision with window
            if (player.getPosition().x <= 0) //Left
                player.setPosition(0.f, player.getPosition().y);
            if (player.getPosition().x >= window.getSize().x - player.getGlobalBounds().width) //Right
                player.setPosition(window.getSize().x - player.getGlobalBounds().width, player.getPosition().y);
            if (player.getPosition().y <= 0) //Top
                player.setPosition(player.getPosition().x, 0.f);
            if (player.getPosition().y >= window.getSize().y - player.getGlobalBounds().height) //Bottom
                player.setPosition(player.getPosition().x, window.getSize().y - player.getGlobalBounds().height);
            
            if (shootTimerLeft < 10)
                shootTimerLeft++;
            if (shootTimerRight < 10)
                shootTimerRight++;

            
                if ((ammo > 0 || infiniteStatus == true) && Mouse::isButtonPressed(Mouse::Left) && shootTimerLeft >= 10 && healthbar.getSize().x > 0)//Shoot
                {
                    

                    if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
                        missileFire.play();
                    projectile.setPosition(leftWingPos);
                    projectiles.push_back(Sprite(projectile));

                    if(infiniteStatus == false)
                        ammo--;
                    else
                    {
                        clock_t currentTime = clock();
                        double elapsedTime = double(currentTime - startTime) / CLOCKS_PER_SEC;

                        if (elapsedTime >= infiniteAmmoDuration)
                        {
                            infiniteStatus = false;
                            startTime = clock();
                        }
                    }

                    std::cout << ammo << "\n";
                    shootTimerLeft = 0;
                }
                if ((ammo > 0 || infiniteStatus == true) && Mouse::isButtonPressed(Mouse::Right) && shootTimerRight >= 10 && healthbar.getSize().x > 0)//Shoot
                {
                    if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
                        missileFire.play();
                    projectile.setPosition(rightWingPos);
                    projectiles.push_back(Sprite(projectile));

                    if (infiniteStatus == false)
                        ammo--;

                    else
                    {
                        clock_t currentTime = clock();
                        int elapsedTime = int(currentTime - startTime) / CLOCKS_PER_SEC;

                        if (elapsedTime >= infiniteAmmoDuration)
                        {
                            infiniteStatus = false;
                            startTime = clock();
                        }
                    }

                    std::cout << ammo << "\n";
                    shootTimerRight = 0;
                }

            
            for (size_t i = 0; i < projectiles.size(); i++)
            {
                projectiles[i].move(0.f, -10.f); // the bullets will move in a verticle straight line upwards

                if (projectiles[i].getPosition().y < 0)
                    projectiles.erase(projectiles.begin() + i); 

            }

            //Enemies
            if (enemySpawnTimer < gameEnemyHardness.enemySpawnDelay)
                enemySpawnTimer++;

            if (enemySpawnTimer >= gameEnemyHardness.enemySpawnDelay && healthbar.getSize().x > 0)
            {
                
                if (rand() % 2 == 0) {
                    
                    enemies.push_back(Enemy(&asteroid,window.getSize()));
                    enemies.back().shape.setScale(gameEnemyHardness.enemySize1);
                    enemies.back().HP = gameEnemyHardness.enemyHealth;
                        
                }
                else {
                   
                    enemies.push_back(Enemy(&asteroidType2, window.getSize()));
                    enemies.back().shape.setScale(gameEnemyHardness.enemySize2);
                    enemies.back().HP = gameEnemyHardness.enemyHealth;
                }
                
            //std::cout << enemies.size() << " |\n";
                enemy_count++;
                std::cout << enemy_count<<"| spawnDelay: "<< gameEnemyHardness.enemySpawnDelay << "Line active: 536 \n";
                pushed = true;
                enemySpawnTimer = 0;
            }
            

            // level progression checker
            if (enemy_count >= 50 && level_1 == true)
            {
                level_2 = true;
                level_1 = false;

            }
            if (enemy_count >= 100 && level_2 == true)
            {
                level_2 = false;
                level_3 = true;
            }
            // End of level checker

            srand(time(NULL));
           
            for (size_t i = 0; i < enemies.size(); i++)
            {       
                
                if (healthbar.getSize().x <= 0)
                    enemies[i].shape.setPosition(Vector2f(enemies[i].shape.getPosition().x, enemies[i].shape.getPosition().y));//
                else {
                    
                    enemies[i].shape.move(gameEnemyHardness.enemyMovementSpeed);
                }
                if (enemies[i].shape.getPosition().x <= 0 - enemies[i].shape.getGlobalBounds().width)
                {
                    enemies.erase(enemies.begin() + i);
                    break;
                }
            }

            //AmmoCrate Spawn
            int delay = 100 ;
            if (ammoCrateTimer <= gameCrateParam.crateRespawnDelay)
                ammoCrateTimer++;

            if (specialCrateTimer <= specialCrateDelay)
                specialCrateTimer++;

            if (ammoCrateTimer >= gameCrateParam.crateRespawnDelay && healthbar.getSize().x > 0)
            {
                if (specialCrateTimer >= specialCrateDelay)
                {
                    ammoCrate.setTexture(specialAmmoTexture);
                    specialCrateTimer = 0;
                }
                ammoCrate.setPosition(rand() % (int)(window.getSize().x - ammoCrate.getGlobalBounds().getPosition().x), 0.f);
                ammoCrates.push_back(Sprite(ammoCrate));
                ammoCrate.setTexture(ammoCrateTexture); // Limits special ammoCrate spaw to one spawn at a time
                ammoCrateTimer = 0;         
            }
            for (size_t i = 0; i < ammoCrates.size(); i++)
            {
                if (healthbar.getSize().x <= 0)
                    ammoCrates[i].setPosition(Vector2f(ammoCrates[i].getPosition().x, ammoCrates[i].getPosition().y));
                else
                    ammoCrates[i].move(gameCrateParam.crateSpeed);
                if (ammoCrates[i].getPosition().y > window.getSize().y)
                    ammoCrates.erase(ammoCrates.begin() + i);


            }

            //std::cout << kitSpawnDelay << "\n";
            //repairKit Spawn
            if (repairKitTimer <= kitSpawnDelay)
                repairKitTimer++;
            if (repairKitTimer > kitSpawnDelay && healthbar.getSize().x > 0)
            {
                repairKit.setPosition((rand()*rand()) % (int)(window.getSize().x - repairKit.getGlobalBounds().getPosition().x), 0.f);
                repairKits.push_back(Sprite(repairKit));
                repairKitTimer = 0;
            }
            for (size_t i = 0; i < repairKits.size(); i++)
            {
                if (healthbar.getSize().x <= 0)
                    repairKits[i].setPosition(Vector2f(repairKits[i].getPosition().x, repairKits[i].getPosition().y));
                else
                    repairKits[i].move(0.f, 3.f);
                if (repairKits[i].getPosition().y > window.getSize().y)
                    repairKits.erase(repairKits.begin() + i);


            }


        //Collisions
            for (size_t i = 0; i < projectiles.size(); i++)
            {
                for (size_t k = 0; k < enemies.size(); k++)
                {
                    if (projectiles[i].getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))//
                    {
                        if (enemies[k].HP <= 1)
                        {
                            hits += gameEnemyHardness.enemyHealth;
                            enemies.erase(enemies.begin() + k);
                        }
                        else
                        {
                            enemies[k].HP--;
                        }
                       
                        //std::cout << hits << "\n";
                        
                        projectiles.erase(projectiles.begin() + i);
                        break;

                    }
                }
            }
           
                for (size_t k = 0; k < ammoCrates.size(); k++)
                {
                    if (player.getGlobalBounds().intersects(ammoCrates[k].getGlobalBounds()) && healthbar.getSize().x >0)
                    {
                        reloadSound.play();
                        if (ammoCrates[k].getTexture() == &specialAmmoTexture) {
                            repairPickUp.play();
                            infiniteStatus = true;
                        }
                        else {
                            ammo += gameCrateParam.refillValue;
                        }
                       
                        //std::cout << ammo << " active line:590\n";
                        ammoCrates.erase(ammoCrates.begin() + k);
                        break;

                    }
                }

                for (size_t k = 0; k < repairKits.size(); k++)
                {
                    if (player.getGlobalBounds().intersects(repairKits[k].getGlobalBounds()) && healthbar.getSize().x > 0)
                    {
                        repairPickUp.play();
                        if (healthbar.getSize().x <= 75) {
                            healthbar.setSize(Vector2f(healthbar.getSize().x + repairVal, 5.f));
                        }
                        else
                        {
                            int delRepairVal = 100 - healthbar.getSize().x;
                            healthbar.setSize(Vector2f(healthbar.getSize().x + delRepairVal, 5.f));
                        }
                        repairKits.erase(repairKits.begin() + k);
                        break;

                    }
                }
            
      
            //Damage to healt checker
            for (size_t k = 0; k < enemies.size(); k++)
            {
                if (healthbar.getSize().x <= 0) {
                    healthbar.setSize(Vector2f(0.f, 5.f));
                    player.setTexture(destroyedShipTexture);
                    
                }
                else if (player.getGlobalBounds().intersects(enemies[k].shape.getGlobalBounds()))//
                {
                    collisionAlarm.play();
                    healthbar.setSize(Vector2f(healthbar.getSize().x - (5*enemies[k].HP), 5.f));
                    enemies.erase(enemies.begin() + k);
                    break;

                }
            }

            if(infiniteStatus == false)
            AmmoTxt.setString("Ammo: " + std::to_string(ammo));
            else {
                AmmoTxt.setString("Ammo: Infinite");

            }
            txt.setString("Score:" + std::to_string(hits));
        //Draw
        window.clear();
        window.draw(bkg);
        
        window.draw(player);

       
        

        window.draw(txt);
        //level declaration
        if (level_1 == true)
        {
            levelTxt.setFillColor(Color::Green);
            levelTxt.setString("lvl 1");
        }
        else if (level_2 == true)
        {
            levelTxt.setFillColor(Color::Yellow);
            levelTxt.setString("lvl 2");
        }
        else if (level_3 == true)
        {
            levelTxt.setFillColor(Color::Red);
            levelTxt.setString("lvl 3");
        }
        window.draw(levelTxt);
        //level declaration ends----------------------

        for (size_t i = 0; i < repairKits.size(); i++)
        {
            window.draw(repairKits[i]);
        }
       

        for (size_t i = 0; i < enemies.size(); i++)//
        {
            window.draw(enemies[i].shape);//
        }

        for (size_t i = 0; i < ammoCrates.size(); i++)
        {
            window.draw(ammoCrates[i]);
        }


        for (size_t i = 0; i < projectiles.size(); i++)
        {
            window.draw(projectiles[i]);
        }

        window.draw(AmmoTxt);
        window.draw(HP);
        window.draw(healthbarBkg);
        window.draw(healthbar);
        if (healthbar.getSize().x <= 0)
        {

            window.draw(gameOverTxt);
            if (gameOver == false) {
                shipDestroyedSound.play();
                gameOver = true;
            }
        }
        
        //std::cout << infiniteStatus << " line active: 627\n";
        window.display();
    }
    
    return 0;
}
