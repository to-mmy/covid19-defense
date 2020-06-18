#include "Enemy.h"


sf::RectangleShape& Enemy::getEnemy() {
    return enemy;
}

int Enemy::getX() {
    return enemy.getPosition().x;
}

int Enemy::getY() {
    return enemy.getPosition().y;
}

Enemy::Enemy(sf::Vector2f size) {
    enemy.setSize(size);
    enemy.setFillColor(sf::Color::Red);
}

void Enemy::setPos(sf::Vector2f newPos) {
    enemy.setPosition(newPos);
}

void Enemy::checkCollision(Bullet bullet) {
    if (bullet.getRight() > enemy.getPosition().x
        && bullet.getTop() < enemy.getPosition().y + enemy.getSize().y
        && bullet.getBottom() > enemy.getPosition().y) {
        enemy.setPosition(sf::Vector2f(4234432, 4233423));
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(enemy);
}
