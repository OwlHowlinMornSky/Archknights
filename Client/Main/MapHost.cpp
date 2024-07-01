/*
*    Archknights
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
#include "MapHost.h"

#include "../Game/MsgResult.h"
#include "HostMsgId.h"
#include "../Game/Global.h"
#include "../Game/Board.h"

namespace Main {

MapHost::MapHost() :
	m_statusChangeCnt(1),
	m_checkpointCnt(0) {}

MapHost::~MapHost() {}

bool MapHost::load(std::ifstream& ifs) {
	int m, n;
	ifs >> m >> n;
	m_tiles.changeSize(m, n);

	m_wall = Game::Global::board->m_world->createWall();

	m_wall->setGroundSize(m, n);

	for (int j = 0; j < n; ++j) {
		for (int i = 0; i < m; ++i) {
			int s;
			ifs >> s;
			if (s & 0x01) { // 高台
				m_tiles(i, j).obstacleLv = 1;
				m_wall->addWallTile(i, j);
			}
		}
	}

	return false;
}

Game::MsgResultType MapHost::receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam) {
	switch (msg) {
	case HostMsgId::MapInitOk:
		for (size_t i = 0, n = m_checkpointCnt; i < n; ++i) {
			search(i);
			m_checkppointLastChange[i] = m_statusChangeCnt;
		}
		break;
	case HostMsgId::MapInitCheckpointCnt:
		m_checkpointCnt = wparam;
		m_checkppointLastChange.resize(m_checkpointCnt);
		m_checkpoints.resize(m_checkpointCnt);
		m_mapRefCnt.resize(m_checkpointCnt);
		m_searches.resize(m_checkpointCnt);
		break;
	case HostMsgId::MapInitCheckpointSet:
		m_checkpoints[wparam].first = ((int*)lparam)[0];
		m_checkpoints[wparam].second = ((int*)lparam)[1];
		break;
	case HostMsgId::MapLeadQuery:
	{
		if (m_checkppointLastChange[wparam] < m_statusChangeCnt) {
			search(wparam);
			m_checkppointLastChange[wparam] = m_statusChangeCnt;
		}
		int oldx = ((int*)lparam)[0];
		int oldy = ((int*)lparam)[1];
		if (oldx < 0 || oldx >= m_searches[wparam].m || oldy < 0 || oldy > m_searches[wparam].n)
			return Game::MsgResult::Leader_OutOfMap;
		int newx = m_searches[wparam](oldx, oldy).sourceX;
		int newy = m_searches[wparam](oldx, oldy).sourceY;
		if (newx == -1 || newy == -1)
			return Game::MsgResult::Leader_NoAvailablePath;
		if (m_tiles(oldx, oldy).obstacleLv)
			return Game::MsgResult::Leader_AtInvalidBlock;
		if (newx == oldx && newy == oldy)
			return Game::MsgResult::Leader_AlreadyReached;
		((int*)lparam)[0] = newx;
		((int*)lparam)[1] = newy;
		if (newx == m_checkpoints[wparam].first && newy == m_checkpoints[wparam].second)
			return Game::MsgResult::Leader_FinalRes;
		return Game::MsgResult::Leader_TempRes;
	}
	case HostMsgId::MapStatusChanged:
		m_statusChangeCnt++;
		updateImmediately();
		break;
	case HostMsgId::MapIncRef:
		m_mapRefCnt[wparam]++;
		break;
	case HostMsgId::MapDecRef:
		m_mapRefCnt[wparam]--;
		break;
	}
	return Game::MsgResult::OK;
}

void MapHost::updateImmediately() {
	for (size_t i = 0, n = m_checkpointCnt; i < n; ++i) {
		if (m_mapRefCnt[i]) {
			search(i);
			m_checkppointLastChange[i] = m_statusChangeCnt;
		}
	}
}

void MapHost::search(const size_t id) {
	return search(id, m_checkpoints[id].first, m_checkpoints[id].second);
}

void MapHost::search(const size_t id, short x, short y) {
	Map::MapData<Map::TileSearch>& searches = m_searches[id];

	// 初始化数值;
	if (searches.m != m_tiles.m || searches.n != m_tiles.n)
		searches.changeSize(m_tiles.m, m_tiles.n);
	else
		searches.reset();

	// 写入终点;
	searches(x, y).Set(x, y, 0, 0);
	m_queue.emplace(x, y);

	// SPFA;
	while (!m_queue.empty()) {
		std::pair<short, short> pt = m_queue.front();
		m_queue.pop();
		spreadFrom(id, pt.first, pt.second);
	}
	return;
}

void MapHost::spreadFrom(const size_t id, const Map::CoordType x, const Map::CoordType y) {
	// DEFINITIONS
	// <本点>: 由 形式参数 (x, y) 指定 的 点。
	// <触及点>: 向外扩散时 由 (i, j) 指定 的 点。
	// [可达]: 一个点 可以 直线通行至 <本点>。
	// <可达点>: 若 一个<触及点>[可达]，则 也称为<可达点>。
	Map::MapData<Map::TileSearch>& searches = m_searches[id];
	Map::MapData<Map::Tile>& tiles = m_tiles;

	// 本点不可达 则 放弃。
	if (searches(x, y).sourceX < 0)
		return;

	Map::CoordType maxd[Map::Tile::Direct::COUNT] = {}; // 四个方向 [可达]的 最远距离，开区间。
	Map::CoordType i, j, k; // 用以 标记<触及点> 与 上一个<触及点> 的 坐标值。

	// 向右寻找 最远[可达]的 点;
	i = x + 1;
	j = y;
	if (i >= tiles.m || tiles(i, j).obstacleLv != 0 || tiles(i, j).isWall[Map::Tile::left]) { // 相邻的点 就已经 不[可达]。
		maxd[Map::Tile::Direct::right] = i;
		// maxd 定位在 本点的右边那个点 (x + 1, y);
		// 横坐标区间 表示为 (x, x + 1);
		// 由 开区间约定，即 右边没有<可达点>。
	}
	else { // 相邻的点 可达。
		do {
			Map::CostCntType c = searches(x, y).cost + tiles(i, j).blockedLv; // <触及点>或将获得的<通行代价>。
			Map::DistanceType d = searches(x, y).distance + (i - x); // <触及点>或将获得的<通行距离>。
			Map::TileSearch& s = searches(i, j); // <触及点>的记录。
			if (TileBetter(s, c, d)) { // 若新的<通行代价>、<通行距离>更优。
				s.Set(x, y, c, d); // 覆盖<触及点>。
				m_queue.emplace(i, j); // 加入队列(SPFA)。
			}
			k = i++; // 继续移动一格。
			// 直到 不[可达]。
		} while (i < tiles.m && tiles(i, j).obstacleLv == 0 && !tiles(i, j).isWall[Map::Tile::left] && tiles(k, j).blockedLv == 0);
		maxd[Map::Tile::Direct::right] = i; // 保存最远距离。
	}
	// 向左寻找最远可通行至本点的点;
	i = x - 1;
	j = y;
	if (i < 0 || tiles(i, j).obstacleLv != 0 || tiles(i, j).isWall[Map::Tile::right]) {
		maxd[Map::Tile::Direct::left] = i;
	}
	else {
		do {
			Map::CostCntType c = searches(x, y).cost + tiles(i, j).blockedLv;
			Map::DistanceType d = searches(x, y).distance + (x - i);
			Map::TileSearch& s = searches(i, j);
			if (TileBetter(s, c, d)) {
				s.Set(x, y, c, d);
				m_queue.emplace(i, j);
			}
			k = i--;
		} while (i >= 0 && tiles(i, j).obstacleLv == 0 && !tiles(i, j).isWall[Map::Tile::right] && tiles(k, j).blockedLv == 0);
		maxd[Map::Tile::Direct::left] = i;
	}
	// 向上寻找最远可通行至本点的点;
	i = x;
	j = y + 1;
	if (j >= tiles.n || tiles(i, j).obstacleLv != 0 || tiles(i, j).isWall[Map::Tile::down]) {
		maxd[Map::Tile::Direct::up] = j;
	}
	else {
		do {
			Map::CostCntType c = searches(x, y).cost + tiles(i, j).blockedLv;
			Map::DistanceType d = searches(x, y).distance + (j - y);
			Map::TileSearch& s = searches(i, j);
			if (TileBetter(s, c, d)) {
				s.Set(x, y, c, d);
				m_queue.emplace(i, j);
			}
			k = j++;
		} while (j < tiles.n && tiles(i, j).obstacleLv == 0 && !tiles(i, j).isWall[Map::Tile::down] && tiles(i, k).blockedLv == 0);
		maxd[Map::Tile::Direct::up] = j;
	}
	// 向下寻找最远可通行至本点的点;
	i = x;
	j = y - 1;
	if (j < 0 || tiles(i, j).obstacleLv != 0 || tiles(i, j).isWall[Map::Tile::up]) {
		maxd[Map::Tile::Direct::down] = j;
	}
	else {
		do {
			Map::CostCntType c = searches(x, y).cost + tiles(i, j).blockedLv;
			Map::DistanceType d = searches(x, y).distance + (y - j);
			Map::TileSearch& s = searches(i, j);
			if (TileBetter(s, c, d)) {
				s.Set(x, y, c, d);
				m_queue.emplace(i, j);
			}
			k = j--;
		} while (j >= 0 && tiles(i, j).obstacleLv == 0 && !tiles(i, j).isWall[Map::Tile::up] && tiles(i, k).blockedLv == 0);
		maxd[Map::Tile::Direct::down] = j;
	}
	// 分别向四个象限扩散。
	spreadQuadrant<+1, +1, Map::Tile::left, Map::Tile::down>(id, x, y, maxd[Map::Tile::right], maxd[Map::Tile::up]); // 第一象限（右上）。
	spreadQuadrant<-1, +1, Map::Tile::right, Map::Tile::down>(id, x, y, maxd[Map::Tile::left], maxd[Map::Tile::up]); // 第二象限（左上）。
	spreadQuadrant<-1, -1, Map::Tile::right, Map::Tile::up>(id, x, y, maxd[Map::Tile::left], maxd[Map::Tile::down]); // 第三象限（左下）。
	spreadQuadrant<+1, -1, Map::Tile::left, Map::Tile::up>(id, x, y, maxd[Map::Tile::right], maxd[Map::Tile::down]); // 第四象限（右下）。
	return;
}

}
