#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

class Animation {
private:
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;

	float totalTime;
	float switchTime;
public:
	sf::IntRect uvRect;

public:
	Animation();
	Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	Animation(const Animation&);
	~Animation();
	void Update(int row, float deltaTime); //deltaTime is how long it will take to switch between frames
	float getSwitchTime();
};

#endif // ANIMATION_H
