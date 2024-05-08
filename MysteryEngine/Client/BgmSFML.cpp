/*
*    Mystery Engine
*
*    Copyright (C) 2023-2024  Tyler Parret True
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
#include "BgmSFML.h"

#include <SFML/System/Err.hpp>

#define FILE_STRICT

namespace {

/**
 * @brief 循环点数据。
*/
struct OHMSAUDIOCOMMENTSTRUCTURE {
	long long offset; // 循环起点。
	long long length; // 循环长度。

	OHMSAUDIOCOMMENTSTRUCTURE() :
		offset(0),
		length(0) {}
};

#ifdef FILE_STRICT
/**
 * @brief 读取 OGG 文件里 key 为 “OHMSSPC” 的注释。
 * @param stream: OGG 文件。
 * @param buffer: 缓冲区。
 * @param bufferLength: 缓冲区长度。
 * @return 读取是否成功。
*/
bool getMusicOggCommentData(
	sf::InputStream& stream,
	unsigned char* buffer,
	unsigned int& bufferLength
) {
	long long pos = 0;
	unsigned char tmp[16];
	unsigned int tmplength = 0;

	if (stream.seek(0) != 0) {
		sf::err() << "getMusicCommentData: seek failed" << std::endl;
		return false;
	}

	// Ogg 文件 首4字节 为 “OggS”。
	if (stream.read(tmp, 4) == -1) return false;
	if (tmp[0] != 'O' || tmp[1] != 'g' || tmp[2] != 'g' || tmp[3] != 'S') {
		sf::err() << "getMusicCommentData: wrong file type" << std::endl;
		return false;
	}

	if (stream.seek(0) != 0) return false;
	bool fin = false;
	while (!fin) {
		pos = stream.tell();
		if (pos == -1) return false;
		if (pos >= stream.getSize()) break;

		if (stream.seek(pos + 26) == -1) return false;

		unsigned char n;
		if (stream.read(&n, 1) == -1) return false;

		unsigned char segLength[256];
		if (stream.read(segLength, n) == -1) return false;

		// read segs
		for (int i = 0; !fin && i < n; ++i) {
			long long segStartPos = stream.tell();
			if (segStartPos == -1) return false;

			if (stream.read(tmp, 1) == -1) return false;

			// seg type is not COMMENT
			if (tmp[0] != 3) {
				if (stream.seek(segStartPos + segLength[i]) == -1) return false;
				continue;
			}

			// seg type is COMMENT
			if (stream.seek(segStartPos + 7) == -1) return false;

			if (stream.read(tmp, 4) == -1) return false;
			tmplength = static_cast<unsigned long long>(tmp[3]);
			tmplength = (((((tmplength << 8) + tmp[2]) << 8) + tmp[1]) << 8) + tmp[0];

			if (stream.seek(segStartPos + 11 + tmplength + 4) == -1) return false;

			while (!fin) {
				pos = stream.tell();
				if (pos == -1) return false;
				if (pos >= segStartPos + segLength[i]) break;

				if (stream.read(tmp, 4) == -1) return false;
				tmplength = static_cast<unsigned long long>(tmp[3]);
				tmplength = (((((tmplength << 8) + tmp[2]) << 8) + tmp[1]) << 8) + tmp[0];

				if (tmplength < 13) {
					if (stream.seek(pos + 4 + tmplength) == -1) return false;
					continue;
				}

				if (stream.read(tmp, 8) == -1) return false;
				// not 'OHMSSPC='
				if (!(tmp[0] == 'O' && tmp[1] == 'H' && tmp[2] == 'M' && tmp[3] == 'S' &&
					  tmp[4] == 'S' && tmp[5] == 'P' && tmp[6] == 'C' && tmp[7] == '=')) {
					if (stream.seek(pos + 4 + tmplength) == -1) return false;
					continue;
				}
				// is 'OHMSSPC='
				tmplength = tmplength - 8;

				if (tmplength >= bufferLength) return false;
				bufferLength = tmplength;
				if (stream.read(buffer, tmplength) == -1) return false;
				buffer[tmplength] = '\0';
				fin = true;
			}
			if (stream.seek(segStartPos + segLength[i]) == -1) return false;
		}
	}
	return fin;
}
#else
bool getMusicOggCommentData(
	sf::InputStream& stream,
	unsigned char* buffer,
	unsigned int& bufferLength
) {
	unsigned char tmp[64];
	unsigned int length = 0;
	if (stream.seek(0xA4) != 0xA4) {
		sf::err() << "getMusicCommentData: seek failed, 0xA4." << std::endl;
		return false;
	}
	if (stream.read(tmp, 64) != 64) {
		sf::err() << "getMusicCommentData: read file failed, 64." << std::endl;
		return false;
	}
	length = static_cast<unsigned int>(tmp[3]);
	length = (((((length << 8) + tmp[2]) << 8) + tmp[1]) << 8) + tmp[0];
	length -= 8;
	if (length > bufferLength) {
		sf::err() << "getMusicCommentData: buffer too short." << std::endl;
		return false;
	}
	if (!(tmp[4] == 'O' && tmp[5] == 'H' && tmp[6] == 'M' && tmp[7] == 'S' &&
		  tmp[8] == 'S' && tmp[9] == 'P' && tmp[10] == 'C' && tmp[11] == '=')) {
		sf::err() << "getMusicCommentData: wrong file type." << std::endl;
		return false;
	}
	for (unsigned int i = 0; i < length; ++i) {
		buffer[i] = tmp[12 + i];
	}
	return true;
}
#endif

/**
 * @brief 读取 OHMS 特定的 循环点。
 * @param stream: 文件流。
 * @param data: [Out] 读到的循环点数据。
 * @return 读取是否成功。
*/
bool readMusicLoopPoint(sf::InputStream& stream, OHMSAUDIOCOMMENTSTRUCTURE& data) {
	unsigned char tmp[48];
	unsigned int length = 48;
	if (!getMusicOggCommentData(stream, tmp, length)) {
		return false;
	}
	if (tmp[0] != '>') {
		return false;
	}
	long long val = 0;
	size_t i = 1;
	for (; i < length; ++i) {
		if (tmp[i] == ':') break;
		if (tmp[i] < '0' || tmp[i] > '9') return false;
		val = val * 10 + tmp[i] - '0';
	}
	data.offset = val;
	if (i >= length - 1 || tmp[i] != ':') return false;
	val = 0;
	for (++i; i < length; ++i) {
		if (tmp[i] == '<') break;
		if (tmp[i] < '0' || tmp[i] > '9') return false;
		val = val * 10 + tmp[i] - '0';
	}
	data.length = val;
	if (i >= length || tmp[i] != '<') return false;
	return true;
}

} // namespace

namespace ME {

BgmSFML::BgmSFML() :
	m_music(std::make_unique<sf::Music>()) {}

BgmSFML::~BgmSFML() {
	this->m_music.reset();
	this->m_stream.reset();
	return;
}

sf::Music::Status BgmSFML::getStatusSFML() const {
	return this->m_music->getStatus();
}

bool BgmSFML::getLoop() const {
	return this->m_music->getLoop();
}

void BgmSFML::setLoop(bool loop) {
	return this->m_music->setLoop(loop);
}

sf::Music::TimeSpan BgmSFML::getLoopPoints() const {
	return this->m_music->getLoopPoints();
}

float BgmSFML::getPitch() const {
	return this->m_music->getPitch();
}

void BgmSFML::setPitch(float pitch) {
	return this->m_music->setPitch(pitch);
}

sf::Vector3f BgmSFML::getPosition() const {
	return this->m_music->getPosition();
}

void BgmSFML::setPosition(float x, float y, float z) {
	return this->m_music->setPosition(x, y, z);
}

void BgmSFML::setPosition(const sf::Vector3f& position) {
	return this->m_music->setPosition(position);
}

bool BgmSFML::isRelativeToListener() const {
	return this->m_music->isRelativeToListener();
}

void BgmSFML::setRelativeToListener(bool relative) {
	return this->m_music->setRelativeToListener(relative);
}

float BgmSFML::getMinDistance() const {
	return this->m_music->getMinDistance();
}

void BgmSFML::setMinDistance(float distance) {
	return this->m_music->setMinDistance(distance);
}

float BgmSFML::getAttenuation() const {
	return this->m_music->getAttenuation();
}

void BgmSFML::setAttenuation(float attenuation) {
	return this->m_music->setAttenuation(attenuation);
}

sf::Time BgmSFML::getPlayingOffset() const {
	return this->m_music->getPlayingOffset();
}

void BgmSFML::setPlayingOffset(sf::Time timeOffset) {
	return this->m_music->setPlayingOffset(timeOffset);
}

unsigned int BgmSFML::getChannelCount() const {
	return this->m_music->getChannelCount();
}

unsigned int BgmSFML::getSampleRate() const {
	return this->m_music->getSampleRate();
}

sf::Time BgmSFML::getDuration() const {
	return this->m_music->getDuration();
}

bool BgmSFML::openFromFile(std::string_view filename) {
	// 必要的，先停止旧的 Music。
	this->m_music->stop();

	std::unique_ptr<sf::FileInputStream> stream = std::make_unique<sf::FileInputStream>();
	// 尝试打开文件流。
	if (!stream->open(filename.data())) {
		sf::err() << "ohms::audio::BGM: open file stream failed" << std::endl;
		return false;
	}

	bool failed = false;
	OHMSAUDIOCOMMENTSTRUCTURE data;
	// 读取循环点信息。
	if (!readMusicLoopPoint(*stream, data)) {
		sf::err() << "ohms::audio::BGM: read comment failed" << std::endl;
		failed = true;
	}

	// 从文件流打开 Music。
	if (!m_music->openFromStream(*stream)) {
		sf::err() << "ohms::audio::BGM: Music open failed" << std::endl;
		return false;
	}

	// 读到的话就设置循环点。
	if (!failed) {
		this->m_music->setLoopPoints(
			sf::Music::TimeSpan(
				sf::microseconds(data.offset),
				sf::microseconds(data.length)
			)
		);
	}
	// 默认开启循环。
	this->m_music->setLoop(true);

	this->m_stream = std::move(stream);
	return true;
}

void BgmSFML::play() {
	return this->m_music->play();
}

void BgmSFML::pause() {
	return this->m_music->pause();
}

void BgmSFML::stop() {
	return this->m_music->stop();
}

Bgm::Status BgmSFML::getStatus() const {
	sf::Music::Status status = this->getStatusSFML();
	Bgm::Status res = Status::Stopped;
	switch (status) {
	case sf::Music::Stopped:
		res = Status::Stopped;
		break;
	case sf::Music::Paused:
		res = Status::Paused;
		break;
	case sf::Music::Playing:
		res = Status::Playing;
		break;
	}
	return res;
}

float BgmSFML::getVolume() const {
	return this->m_music->getVolume();
}

void BgmSFML::setVolume(float volume) {
	return this->m_music->setVolume(volume);
}

float BgmSFML::getTime() const {
	return this->getPlayingOffset().asSeconds();
}

void BgmSFML::setTime(float seconds) {
	return this->setPlayingOffset(sf::seconds(seconds));
}

float BgmSFML::getLength() const {
	return this->getDuration().asSeconds();
}

} // namespace ME
