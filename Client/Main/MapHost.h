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
#pragma once

#include "../Game/Host.h"
#include <vector>
#include <queue>
#include <fstream>
#include "../Physics/IWorld.h"

namespace Main {

namespace Map {

typedef int16_t  CoordType;
constexpr CoordType CoordDef = -1;
typedef uint32_t CostCntType;
constexpr CostCntType CostCntDef = UINT32_MAX;
typedef float_t  DistanceType;
constexpr DistanceType DistanceDef = FP_INFINITE; // 不为无限大
typedef uint16_t ObstacleLvType;
typedef uint16_t BlockedLvType;

struct Tile {
	enum Direct {
		right = 0,
		up,
		left,
		down,
		COUNT
	};
	bool isWall[Direct::COUNT]; // 方向通行标记。例：若 left 为 ture，则 从该点 不能 向左边移动。
	ObstacleLvType obstacleLv; // <障碍>标记，只要 不为0 就 不可以通行。
	BlockedLvType blockedLv; // <阻碍>等级，会增加<通行代价>。

	Tile() :
		isWall{},
		obstacleLv(0),
		blockedLv(0) {}
};

struct TileSearch {
	CoordType sourceX; // 源点的横坐标。
	CoordType sourceY; // 源点的纵坐标。
	CostCntType cost; // <通行代价>: 从该点到<追随点>的总<阻碍>。
	DistanceType distance; // <通行距离>: 从该点到<追随点>的总路程。

	TileSearch() :
		sourceX(CoordDef),
		sourceY(CoordDef),
		cost(CostCntDef),
		distance(DistanceDef) {}

	void Reset() {
		sourceX  = CoordDef;
		sourceY  = CoordDef;
		cost     = CostCntDef;
		distance = DistanceDef;
	}

	void Set(CoordType sx, CoordType sy, CostCntType c, DistanceType d) {
		sourceX  = sx;
		sourceY  = sy;
		cost     = c;
		distance = d;
	}
};

struct Occupation {
	bool isOccupied;
	enum Type {
		CommonGround,
		CommonWall,
		DeployableGround,
		DeployableWall,
		Deployable,
		Hole,
	} type;
	std::weak_ptr<Game::Entity> entity;

	Occupation() :
		isOccupied(false),
		entity(),
		type(Type::CommonGround) {}
};

template<typename _T>
struct MapData {
	size_t m;
	size_t n;
	_T* data;

	MapData() :
		m(0),
		n(0),
		data(nullptr) {}

	MapData(size_t M, size_t N) :
		m(M),
		n(N) {
		data = new _T[M * N];
	}

	void changeSize(size_t M, size_t N) {
		m = M;
		n = N;
		delete[] data;
		data = new _T[M * N];
	}

	void reset() {
		if (data == nullptr)
			return;
		for (size_t i = 0, k = m * n; i < k; ++i) {
			data[i].Reset();
		}
	}

	_T& operator()(size_t x, size_t y) {
		return data[x + y * m];
	}
};

}

class MapHost final :
	public Game::Host {

public:
	MapHost();
	virtual ~MapHost();

public:
	bool load(std::ifstream& ifs);

	virtual Game::MsgResultType receiveMessage(Game::MsgIdType msg, Game::MsgWparamType wparam, Game::MsgLparamType lparam);

protected:
	void updateImmediately();

	void search(const size_t id);

	/**
	 * @brief 搜索各地点前往指定位置的路径。
	 * @param x: 指定位置的横坐标。
	 * @param y: 指定位置的纵坐标。
	 */
	void search(const size_t id, short x, short y);

	/**
	 * @brief 从 一个<临时点> 向外 扩散。
	 * @param x: <临时点>横坐标。
	 * @param y: <临时点>纵坐标。
	 */
	void spreadFrom(const size_t id, const Map::CoordType x, const Map::CoordType y);

	/**
	 * @brief 向一个象限扩散。
	 * @brief 注意：扩散象限范围不包括坐标轴，因为其在SpreadFrom()中已经由直线扩散计算。
	 * @tparam dx: 扩散的横坐标步进方向，取值可为{ -1, +1 }。
	 * @tparam dy: 扩散的纵坐标步进方向，取值可为{ -1, +1 }。
	 * @tparam px: 与横坐标步进方向相反的方向，取值可为{ Tile::left, Tile::right }。
	 * @tparam py: 与纵坐标步进方向相反的方向，取值可为{ Tile::down, Tile::up }。
	 * @param x: 扩散的起始点的横坐标，其在SpreadFrom()中称为<本点>。
	 * @param y: 扩散的起始点的纵坐标，其在SpreadFrom()中称为<本点>。
	 * @param maxX: 横坐标最远位置。
	 * @param maxY: 纵坐标最远位置。
	 */
	template<Map::CoordType dx, Map::CoordType dy, int px, int py>
	inline void spreadQuadrant(const size_t id, const Map::CoordType x, const Map::CoordType y, Map::CoordType maxX, const Map::CoordType maxY);

	std::size_t m_statusChangeCnt;
	std::size_t m_checkpointCnt;
	std::vector<std::pair<Map::CoordType, Map::CoordType>> m_checkpoints;
	std::vector<std::size_t> m_checkppointLastChange;
	std::vector<std::size_t> m_mapRefCnt;
	std::vector<Map::MapData<Map::TileSearch>> m_searches; // Result of search.
	Map::MapData<Map::Tile> m_tiles; // Map data.
	std::queue<std::pair<short, short>> m_queue; // Temporary queue when searching.

	std::unique_ptr<Physics::IWall> m_wall;

	Map::MapData<Map::Occupation> m_occupation;
};

}

namespace {

/**
 * @brief 判断 新的<通行代价> 与 新的<通行距离> 是否 更优。
 * @param t: 指定点。
 * @param cost: 新的<通行代价>。
 * @param dist: 新的<通行距离>。
 * @return 若 更优 则 为 true.
 */
static inline bool TileBetter(
	const Main::Map::TileSearch& t,
	const Main::Map::CostCntType cost,
	const Main::Map::DistanceType dist
) {
	//        点不可达       <通行代价>更小         <通行代价>相同时，<通行距离>更短。
	return t.sourceX < 0 || cost < t.cost || (cost == t.cost && dist < t.distance);
}

}

namespace Main {

template<Map::CoordType dx, Map::CoordType dy, int px, int py>
inline void MapHost::spreadQuadrant(const size_t id, const Map::CoordType x, const Map::CoordType y, Map::CoordType maxX, const Map::CoordType maxY) {
	Map::MapData<Map::TileSearch>& searches = m_searches[id];
	Map::MapData<Map::Tile>& tiles = m_tiles;
	// i 代表 横坐标（列号），j 代表 纵坐标（行号）。
	// 一行一行步进，因为 Map 按行存储。
	short v = y; // 旧<行号>，用于下面计算[可达]性。
	short j = v + dy; // <行号>的初始值。
	while (j != maxY) { // 步进<行号>直到纵坐标最远。
		short u = x; // 旧<列号>，用于下面计算[可达]性。
		short i = u + dx; // <列号>的初始值。
		while (i != maxX) { // 步进<列号>直到横坐标最远。
			// ~注~ 以下描述以第一象限为例。
			bool inaccessible =                                             // 若
				searches(u, j).sourceX < 0 || searches(i, v).sourceX < 0 || // 左边 和 下边 不[可达]
				tiles(i, j).obstacleLv ||                                   // 该点是<障碍>
				tiles(i, j).isWall[px] || tiles(i, j).isWall[py] ||         // 该点 不可 向左 或 向下 移动
				tiles(u, j).blockedLv || tiles(i, v).blockedLv              // 左边 或 下边 有<阻碍>
				;                                                           // 则 该点 不[可达]。
			if (inaccessible) { // 若 不[可达]
				maxX = i; // 收缩 向右的 最远距离。
				break; // 停止向右步进。
			}
			// 若 [可达] 则 计算 可能值。
			Map::DistanceType distance = searches(x, y).distance + std::sqrtf(1.0f * (i - x) * (i - x) + 1.0f * (j - y) * (j - y));
			Map::CostCntType cost = searches(x, y).cost + tiles(i, j).blockedLv;
			Map::TileSearch& s = searches(i, j);
			if (TileBetter(s, cost, distance)) { // 若 新值 较优。
				s.Set(x, y, cost, distance); // 覆盖。
				m_queue.emplace(i, j); // 入队(SPFA)。
			}
			// ~/注~

			u = i; // 记录旧<列号>
			i += dx; // 步进<列号>。
		}
		v = j; // 记录旧<行号>。
		j += dy; // 步进<行号>。
	}
	return;
}

}
