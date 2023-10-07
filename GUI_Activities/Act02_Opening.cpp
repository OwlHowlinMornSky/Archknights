#include "Act02_Opening.h"

#include <SFML/Graphics.hpp>
#include "../GUI/Callbacks.h"

#include "ActivityIDs.h"

namespace Activity {

Act02_Opening::Act02_Opening() noexcept :
	ref_carnival(nullptr) {}

void Act02_Opening::start(GUI::ICarnival& carnival) {
	ref_carnival = &carnival;
	return;
}

void Act02_Opening::stop() noexcept {
	ref_carnival = nullptr;
	return;
}

void Act02_Opening::pause() noexcept {}

void Act02_Opening::resume() noexcept {}

uint32_t Act02_Opening::getID() noexcept {
	return IDs::ID_Opening;
}

void Act02_Opening::runIndependently() {
	sf::RenderWindow& window = ref_carnival->getRenderWindow();

	sf::CircleShape circle[3];
	circle[0].setRadius(100.0f);
	circle[0].setOrigin(100.0f, 100.0f);
	circle[0].setPosition(500.0f, 300.0f);
	circle[1].setRadius(100.0f);
	circle[1].setOrigin(100.0f, 100.0f);
	circle[1].setPosition(700.0f, 300.0f);
	circle[2].setRadius(100.0f);
	circle[2].setOrigin(100.0f, 100.0f);
	circle[2].setPosition(600.0f, 300.0f);

	sf::Clock clk;
	const sf::Time th0 = sf::Time::Zero;
	const sf::Time th1 = sf::milliseconds(250);
	const sf::Time th2 = sf::milliseconds(1250);
	const sf::Time th3 = sf::milliseconds(1500);
	const sf::Time th4 = sf::milliseconds(2500);
	const sf::Time th5 = sf::milliseconds(2750);
	const sf::Time th6 = sf::milliseconds(3000);
	const sf::Time th7 = sf::milliseconds(4750);
	const sf::Time th8 = sf::milliseconds(5000);
	sf::Time timer;

	bool closed = false;
	auto oldIdle = Callbacks::OnIdle;
	{
		circle[0].setFillColor(sf::Color::Transparent);
		circle[1].setFillColor(sf::Color::Transparent);
		circle[2].setFillColor(sf::Color::Transparent);
		auto f0 = [&]()->void {
			sf::Time dT = clk.restart();
			if (dT > sf::milliseconds(100))
				dT = sf::milliseconds(100);
			timer += dT;
			if (timer < th0)
				;
			else if (timer < th1)
				circle[0].setFillColor(sf::Color(255 * timer.asMilliseconds() / 250, 0, 0));
			else if (timer < th2)
				circle[0].setFillColor(sf::Color(255, 0, 0));
			else if (timer < th3)
				circle[1].setFillColor(sf::Color(0, 255 * (timer - th2).asMilliseconds() / 250, 0));
			else if (timer < th4)
				circle[1].setFillColor(sf::Color(0, 255, 0));
			else if (timer < th5) {
				uint8_t a = 255 * (th5 - timer).asMilliseconds() / 250;
				circle[0].setFillColor(sf::Color(a, 0, 0));
				circle[1].setFillColor(sf::Color(0, a, 0));
			}
			else if (timer < th6) {
				circle[0].setFillColor(sf::Color::Transparent);
				circle[1].setFillColor(sf::Color::Transparent);
				circle[2].setFillColor(sf::Color(0, 0, 255 * (timer - th5).asMilliseconds() / 250));
			}
			else if (timer < th7)
				circle[2].setFillColor(sf::Color(0, 0, 255));
			else if (timer < th8)
				circle[2].setFillColor(sf::Color(0, 0, 255 * (th8 - timer).asMilliseconds() / 250));
			else {
				circle[0].setFillColor(sf::Color::Transparent);
				circle[1].setFillColor(sf::Color::Transparent);
				circle[2].setFillColor(sf::Color::Transparent);
			}
			window.clear();
			window.draw(circle[1]);
			window.draw(circle[0]);
			window.draw(circle[2]);
			window.display();
		};
		Callbacks::OnIdle = f0;
		timer = sf::Time::Zero;
		clk.restart();
		while (timer < th8) {
			ref_carnival->systemMessagePump(true);
			sf::Event evt;
			while (window.pollEvent(evt)) {
				if (evt.type == sf::Event::Closed) {
					timer = th8;
					closed = true;
				}
				else if (evt.type == sf::Event::Resized) {
					ref_carnival->getRenderWindow().setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)evt.size.width, (float)evt.size.height)));
				}
			}
			f0();
		}
	}
	Callbacks::OnIdle = oldIdle;
	if (closed)
		ref_carnival->setTransition(GUI::Transition::Exit);
	else
		ref_carnival->setTransition(GUI::Transition::Switch, IDs::ID_Load);
	return;
}

}
