#pragma once

#include "IActivity.h"

namespace GUI {

class ActivityDependent : public IActivity {
public:
	ActivityDependent() = default;
	virtual ~ActivityDependent() override = default;

public:
	/**
	 * @brief 该 Acitivity 是否要独立运行。
	 * @return True 则独立, 否则非独立。
	*/
	virtual bool isIndependent() const override final;
	/**
	 * @brief 独立地运行。
	*/
	virtual void runIndependently() override final;

	/**
	 * @brief 处理事件。
	 * @param evt: SFML 的事件。
	*/
	virtual void handleEvent(const sf::Event& evt) = 0;
	/**
	 * @brief 更新。
	 * @param dt 经过的时间。
	*/
	virtual void update(float dt) = 0;
};

} // namespace GUI
