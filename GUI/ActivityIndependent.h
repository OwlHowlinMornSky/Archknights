#pragma once

#include "IActivity.h"

namespace GUI {

class ActivityIndependent : public IActivity {
public:
	ActivityIndependent() = default;
	virtual ~ActivityIndependent() override = default;

public:
	/**
	 * @brief �� Acitivity �Ƿ�Ҫ�������С�
	 * @return True �����, ����Ƕ�����
	*/
	virtual bool isIndependent() const override final;
	/**
	 * @brief ���������С�
	*/
	virtual void runIndependently() override = 0;

	/**
	 * @brief �����¼���
	 * @param evt: SFML ���¼���
	*/
	virtual void handleEvent(const sf::Event& evt) override final;
	/**
	 * @brief ���¡�
	 * @param dt ������ʱ�䡣
	*/
	virtual void update(float dt) override final;
};

} // namespace GUI
