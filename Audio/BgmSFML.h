/*
*    Archknights
*
*    Copyright (C) 2023  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*    Tyler Parret True <mysteryworldgod@outlook.com><https://github.com/OwlHowlinMornSky>
*/
#pragma once

#include <memory>
#include <SFML/Audio/Music.hpp>
#include <SFML/System/FileInputStream.hpp>
#include "IBgm.h"

namespace Audio {

/**
 * @brief IBgm 在 SFML 的实现。
*/
class BgmSFML final : public IBgm {
public:
	BgmSFML();
	virtual ~BgmSFML() override;

public:
	sf::Music::Status getStatusSFML() const;

	bool getLoop() const;
	void setLoop(bool loop);
	sf::Music::TimeSpan getLoopPoints() const;

	float getPitch() const;
	void setPitch(float pitch);

	sf::Vector3f getPosition() const;
	void setPosition(float x, float y, float z);
	void setPosition(const sf::Vector3f& position);

	bool isRelativeToListener() const;
	void setRelativeToListener(bool relative);

	float getMinDistance() const;
	void setMinDistance(float distance);

	float getAttenuation() const;
	void setAttenuation(float attenuation);

	sf::Time getPlayingOffset() const;
	void setPlayingOffset(sf::Time timeOffset);

	unsigned int getChannelCount() const;
	unsigned int getSampleRate() const;

	sf::Time getDuration() const;

public:
	virtual [[nodiscard]] bool openFromFile(std::string_view filename) override;

	virtual void play() override;
	virtual void pause() override;
	virtual void stop() override;
	virtual Status getStatus() const override;

	virtual float getVolume() const override;
	virtual void setVolume(float volume) override;

	virtual float getTime() const override;
	virtual void setTime(float seconds) override;
	virtual float getLength() const override;

protected:
	std::unique_ptr<sf::Music> m_music;
	std::unique_ptr<sf::FileInputStream> m_stream;
}; // class BGM

} // namespace Audio