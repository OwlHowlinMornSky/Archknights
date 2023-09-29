#pragma once

#include "IActivity.h"

namespace GUI {

class ActivityDependent : public IActivity {
public:
	ActivityDependent() = default;
	virtual ~ActivityDependent() override = default;

public:
	/**
	 * @brief �� Acitivity �Ƿ�Ҫ�������С�
	 * @return True �����, ����Ƕ�����
	*/
	virtual bool isIndependent() const override final;
	/**
	 * @brief ���������С�
	*/
	virtual void runIndependently() override final;

	/**
	 * @brief �����¼���
	 * @param evt: SFML ���¼���
	*/
	virtual void handleEvent(const sf::Event& evt) = 0;
	/**
	 * @brief ���¡�
	 * @param dt ������ʱ�䡣
	*/
	virtual void update(float dt) = 0;
};

} // namespace GUI
