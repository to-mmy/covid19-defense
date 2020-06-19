#include "Animation.h"
#include <iostream>

Animation::Animation() {}

Animation::Animation(const Animation& a) {
    this->uvRect.width = a.uvRect.width;
    this->uvRect.height = a.uvRect.height;
    currentImage.x = 0;
    totalTime = 0.0f;
    imageCount = a.imageCount;
    switchTime = a.switchTime;
}

Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime) {
    this->imageCount = imageCount;
    this->switchTime = switchTime;
    totalTime = 0.0f;
    currentImage.x = 0; // the starting frame for the animation must be in the first column
    uvRect.width = texture->getSize().x / float(imageCount.x);
    uvRect.height = texture->getSize().y / float(imageCount.y);
}

Animation::~Animation() {}

void Animation::Update(int row, float deltaTime) {
    currentImage.y = row;
    totalTime += deltaTime;

    if (totalTime >= switchTime) {
        totalTime -= switchTime;
        currentImage.x++;

        if (currentImage.x >= imageCount.x) {
            currentImage.x = 0;
        }
    }

    uvRect.left = currentImage.x * uvRect.width;
    uvRect.top = currentImage.y * uvRect.height;
}

float Animation::getSwitchTime() {
    return switchTime;
}
