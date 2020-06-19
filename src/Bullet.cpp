#include "Bullet.h"

sf::Sprite& Bullet::getSprite() {
    return bullet;
}

Bullet::Bullet(std::string bulletType, double tX, double tY, double eX, double eY) {
    towerX = tX;
    towerY = tY;
    enemyX = eX; //add half of the width of enemy
    enemyY = eY; //add half of the height of enemy
    rotate();
    setPos(sf::Vector2f(tX, tY));
    type = bulletType;

    if (type == "bubble") {
        bullet.setScale((1), (1));
    } else bullet.setScale(1, 1);
}

void Bullet::rotate() {
    float vert = enemyY - towerY;
    float hor = enemyX - towerX;
    float diag = sqrt((vert * vert) + (hor * hor));
    float angle = (asin(vert / diag) * 180) / acos(-1);

    if ((towerX > enemyX && towerY > enemyY) || (towerX > enemyX && towerY < enemyY)) {
        bullet.rotate(180 - angle);
    } else bullet.rotate(angle);
}

void Bullet::setPos(sf::Vector2f newPos) {
    bullet.setPosition(newPos);
}

void Bullet::fire() {
    double x = enemyX - towerX;
    double y = enemyY - towerY;

    float magnitude = sqrt((x * x) + (y * y));
    if (magnitude == 0)
        magnitude = 1;

    if (type == "squirt")
        bullet.move(x / magnitude, y / magnitude);
    else bullet.move(x / (magnitude * 5), y / (magnitude * 5)); //bubbles move slower than squirts
}

int Bullet::getLeft() {
    return bullet.getPosition().x;
}

int Bullet::getTop() {
    return bullet.getPosition().y;
}

void Bullet::draw(sf::RenderWindow& window) {
    window.draw(bullet);
}
