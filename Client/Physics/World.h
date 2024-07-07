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

#include "IWorld.h"
#include "FrictionBody.h"

#include <box2d/box2d.h>

namespace Physics {

class MyContactListener :
	public b2ContactListener {
public:
	/// Called when two fixtures begin to touch.
	virtual void BeginContact(b2Contact* contact);

	/// Called when two fixtures cease to touch.
	virtual void EndContact(b2Contact* contact);

	/// This is called after a contact is updated. This allows you to inspect a
	/// contact before it goes to the solver. If you are careful, you can modify the
	/// contact manifold (e.g. disable contact).
	/// A copy of the old manifold is provided so that you can detect changes.
	/// Note: this is called only for awake bodies.
	/// Note: this is called even when the number of contact points is zero.
	/// Note: this is not called for sensors.
	/// Note: if you set the number of contact points to zero, you will not
	/// get an EndContact callback. However, you may get a BeginContact callback
	/// the next step.
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	/// This lets you inspect a contact after the solver is finished. This is useful
	/// for inspecting impulses.
	/// Note: the contact manifold does not include time of impact impulses, which can be
	/// arbitrarily large if the sub-step is small. Hence the impulse is provided explicitly
	/// in a separate data structure.
	/// Note: this is only called for contacts that are touching, solid, and awake.
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

class World final :
	public IWorld {
public:
	World();
	virtual ~World();

public:
	virtual void update() override;

	virtual std::unique_ptr<IWall> createWall() override;

	virtual std::unique_ptr<IBody> createBodyTowerCircle(float x, float y, uint8_t type = FilterType::ArmyStand) override;
	virtual std::unique_ptr<IBody> createBodyMoverCircle(float x, float y, uint8_t type = FilterType::EnemyStand) override;
	virtual std::unique_ptr<IBody> createBodyMoverRectangle(float x, float y, uint8_t type = FilterType::EnemyStand) override;

	virtual std::unique_ptr<IDetector> createDetectorCircle(uint8_t target, float x, float y, float radius) override;
	virtual std::unique_ptr<IDetector> createDetectorRows(uint8_t target, float x, float y, Rows* rows) override;
	virtual std::unique_ptr<IDetector> createDetectorTiles(uint8_t target, float x, float y, size_t length, int* tiles) override;

	virtual std::unique_ptr<IDetector> createBlockerCircle(float x, float y, float radius) override;

protected:
	b2World m_world;
	MyContactListener m_contactListener;
	std::unique_ptr<FrictionBody> m_frictionBody;
};

}
