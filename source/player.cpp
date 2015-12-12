//player.cpp

#include "player.h"
#include "mathematics.h"
#include <dxlib.h>
#include "color.h"
#include "device.h"
#include "image_pool.h"
#include "drawblendmode.h"
#include "dxlib_wrapper.h"
#include "environmental_constants.h"
#include <cmath>
#include "dxsstream.h"
#include <boost/geometry.hpp>
#include <boost/math/constants/constants.hpp>
#include "msgbox.h"

namespace
{
	constexpr int MAX_HEALTH = 8;
	constexpr int WARP_WAIT_TIME = 200;
	constexpr int MAX_WARP_CHARGE = 1000;
	constexpr int UNIT_HYPER_COST = 1000;
	constexpr int UNIT_WARP_COST = 500;
	constexpr MyGameProject::Real WARP_GAP = 250;
	constexpr int WARP_MOTION_TIME = 50;
	constexpr int ACTION_SELECT_WAIT_PERIOD = 7;
	using namespace boost::math::constants;
}

MyGameProject::Player::Player(const Device& _device, std::function<void(const std::string&)> _se_flag_setter)
	:HitableObject(HitBox(ShapeElement::Circle(6), PhysicalState(0, Point2D(WW_2<Real>(), WH<Real>() - 100)))),
	MovingObject(Point2D(WW_2<Real>(), WH<Real>() - 100)),
	shot_flag(false),
	locked_flag(false),
	device(_device),
	lock_icon_brt(255),
	lock_icon_rate(0),
	count(0),
	weapon_type(WeaponType::LONG_RANGE),
	selected_weapon_number(0),
	hyper_mode(false),
	hyper_mode_count(0),
	erect_se_flag_of(_se_flag_setter),
	current_state(State::NORMAL),
	immortal_count(0),
	current_health(MAX_HEALTH),
	velocity(Point2D(0,0)),
	warp_count(0),
	warp_destination(),
	warp_charge(MAX_WARP_CHARGE),
	warp_angle(0),
	discharge_button_pressed_count(0)
{}


MyGameProject::Player::WeaponType MyGameProject::Player::weapon(void) const
{
	if (!hyper_mode)
	{
#if 0
		switch (selected_weapon_number)
		{
		case 0:
			return WeaponType::LONG_RANGE;
		case 1:
			return WeaponType::SHORT_RANGE;
		default:
			return WeaponType::LONG_RANGE;
		}
#endif
		if (device.get_time("X") > 0){ return WeaponType::SHORT_RANGE; }
		else{ return WeaponType::LONG_RANGE; }
	}
	else
	{
#if 0
		switch (selected_weapon_number)
		{
		case 0:
			return WeaponType::HYPER_LONG_RANGE;
		case 1:
			return WeaponType::HYPER_SHORT_RANGE;
		default:
			return WeaponType::HYPER_LONG_RANGE;
		}
#endif
		if (device.get_time("X") > 0){ return WeaponType::HYPER_SHORT_RANGE; }
		else{ return WeaponType::HYPER_LONG_RANGE; }
	}
}

MyGameProject::Player::State MyGameProject::Player::state(void) const
{
	return current_state;
}

void MyGameProject::Player::draw(void) const
{
	if (locked_flag)
	{
		gp::ScopedDrawBlendMode blend(DX_BLENDMODE_ALPHA, lock_icon_brt);
		FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), lock_icon_rate, 0.0, ImagePool::get("../../data/img/lock.png"), TRUE);
	}

	{
		gp::ScopedDrawBlendMode blend(DX_BLENDMODE_ALPHA, 100);
		switch (current_health)
		{
		case 1:
			FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.385, 0.0, ImagePool::get("../../data/img/health_indicator.png@0"), TRUE);
			break;
		case 2:
			FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.385, 0.0, ImagePool::get("../../data/img/health_indicator.png@1"), TRUE);
			break;
		case 3:
			FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.385, 0.0, ImagePool::get("../../data/img/health_indicator.png@2"), TRUE);
			break;
		case 4:
			FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.385, 0.0, ImagePool::get("../../data/img/health_indicator.png@3"), TRUE);
			break;
		case 5:
			FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.385, 0.0, ImagePool::get("../../data/img/health_indicator.png@4"), TRUE);
			break;
		case 6:
			FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.385, 0.0, ImagePool::get("../../data/img/health_indicator.png@5"), TRUE);
			break;
		case 7:
			FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.385, 0.0, ImagePool::get("../../data/img/health_indicator.png@6"), TRUE);
			break;
		case 8:
			FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.385, 0.0, ImagePool::get("../../data/img/health_indicator.png@7"), TRUE);
			break;
		default:
			break;
		}
	}

	if (current_state == State::NORMAL || current_state == State::ACTION_SELECT)
	{
		FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.7, 0.0, ImagePool::get("../../data/img/player.png"), TRUE);
	}
	else if (current_state == State::WARP)
	{
		FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.7, 0.0, ImagePool::get("../../data/img/player.png"), TRUE);
		ScopedDrawBlendMode blend(DX_BLENDMODE_ALPHA, 150);
		FunctionWrapper::DrawCircle(warp_destination.x(),warp_destination.y(),30 * (static_cast<Real>(warp_count) / WARP_WAIT_TIME),gp::sky.get());
	}
	else if (current_state == State::WARP_MOTION)
	{
		int i = 0;
		for (const auto& p : trail)
		{
			ScopedDrawBlendMode blend(DX_BLENDMODE_ALPHA, 100 - 15 * i++);
			FunctionWrapper::DrawRotaGraph(p.x(), p.y(), 0.7, 0.0, ImagePool::get("../../data/img/player.png"), TRUE);
		}
		FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.7, 0.0, ImagePool::get("../../data/img/player.png"), TRUE);
	}
	else if (current_state == State::IMMORTAL)
	{
		if (count % 5 == 0)
		{
			FunctionWrapper::DrawRotaGraph(pos().x(), pos().y(), 0.7, 0.0, ImagePool::get("../../data/img/player.png"), TRUE);
		}
	}
	gp::dxout(static_cast<int>(pos().x()), static_cast<int>(pos().y())) << warp_charge << gp::end;
}

int MyGameProject::Player::rest_hyper_mode_time(void) const
{
	if (is_hyper_mode()){ return hyper_mode_count; }
	else{ return 0; }
}

void MyGameProject::Player::update_mode(void)
{
	if (current_state != State::WARP)
	{
		if (device.get_time("Z") > 0){ shot_flag = true; }
		else{ shot_flag = false; }
		if (device.get_time("Shift") == 1)
		{
			locked_flag = !locked_flag;
			lock_icon_brt = 255; lock_icon_rate = 0;
			erect_se_flag_of("../../data/sound/meka_ge_kaseto_deki_botan03.mp3");
		}
#if 0
		if (device.get_time("C") == 1 && !hyper_mode && warp_charge >= UNIT_HYPER_COST)
		{
			hyper_mode = true;
			warp_charge -= UNIT_HYPER_COST; 
			hyper_mode_count = HYPER_MODE_TIME; 
		}
#endif
	}
#if 0
	if (device.get_time("X") == 1 && warp_charge >= UNIT_WARP_COST)
	{
		shot_flag = false;
		warp_count = WARP_WAIT_TIME; 
		warp_charge -= UNIT_WARP_COST;
		current_state = State::WARP;
	}
	if (current_state == State::WARP && (device.is_just_released("X") || warp_count < 0))
	{
		const auto r = static_cast<Real>(boost::geometry::distance(pos(),warp_destination));
		warp_angle = (warp_destination == pos()? -pi<Real>() / 2: angle_of(warp_destination - pos()));
		pos() = pos() + Point2D((r - WARP_GAP) * static_cast<Real>(cos(warp_angle)), (r - WARP_GAP) * static_cast<Real>(sin(warp_angle)));
		v_warp = Point2D((2 * WARP_GAP / WARP_MOTION_TIME) * cos(warp_angle), (2 * WARP_GAP / WARP_MOTION_TIME) * sin(warp_angle));
		current_state = State::WARP_MOTION;
		erect_se_flag_of("../../data/sound/magic_wave3.mp3");
	}
#endif
	if (device.get_time("C") == 1 && warp_charge >= UNIT_WARP_COST && current_state != State::WARP && current_state != State::WARP_MOTION && current_state != State::ACTION_SELECT)
	{
		current_state = State::ACTION_SELECT;
		discharge_button_pressed_count = count;
	}
	if (current_state == State::ACTION_SELECT && count == discharge_button_pressed_count + ACTION_SELECT_WAIT_PERIOD)
	{
		if (device.get_time("C") >= ACTION_SELECT_WAIT_PERIOD)
		{
			if (warp_charge >= UNIT_WARP_COST)
			{
				shot_flag = false;
				warp_count = WARP_WAIT_TIME;
				warp_charge -= UNIT_WARP_COST;
				current_state = State::WARP;
			}
		}
		else
		{
			if (warp_charge >= UNIT_HYPER_COST)
			{
				if (!hyper_mode)
				{
					hyper_mode = true;
					warp_charge -= UNIT_HYPER_COST;
					hyper_mode_count = HYPER_MODE_TIME;
				}
				current_state = State::NORMAL;
			}
		}
	}
	if (current_state == State::WARP && (device.is_just_released("C") || warp_count < 0))
	{
		const auto r = static_cast<Real>(boost::geometry::distance(pos(), warp_destination));
		warp_angle = (warp_destination == pos() ? -pi<Real>() / 2 : angle_of(warp_destination - pos()));
		pos() = pos() + Point2D((r - WARP_GAP) * static_cast<Real>(cos(warp_angle)), (r - WARP_GAP) * static_cast<Real>(sin(warp_angle)));
		v_warp = Point2D((2 * WARP_GAP / WARP_MOTION_TIME) * cos(warp_angle), (2 * WARP_GAP / WARP_MOTION_TIME) * sin(warp_angle));
		current_state = State::WARP_MOTION;
		erect_se_flag_of("../../data/sound/magic_wave3.mp3");
	}
}

void MyGameProject::Player::update(void)
{
	if (count == 0){ for (auto& p : trail){ p = pos(); } }
	else
	{
		for (int i = trail.size() - 1; i != 0; --i)
		{
			trail[i] = trail[i - 1];
		}
		trail[0] = pos();
	}

	const Point2D prev_p = pos();
	Real speed = 7;
	const bool up_input = device.get_time("Up") > 0;
	const bool down_input = device.get_time("Down") > 0;
	const bool right_input = device.get_time("Right") > 0;
	const bool left_input = device.get_time("Left") > 0;

	if ((up_input || down_input) && (right_input || left_input)){ speed /= static_cast<Real>(std::sqrt(2)); }

	if (current_state == State::WARP)
	{
		if (up_input && warp_destination.y() > 0)            { warp_destination += Point2D(0, -speed); }
		if (down_input && warp_destination.y() < WH<Real>()) { warp_destination += Point2D(0, speed); }
		if (right_input && warp_destination.x() < WW<Real>()){ warp_destination += Point2D(speed, 0); }
		if (left_input && warp_destination.x() > 0)          { warp_destination += Point2D(-speed, 0); }

		--warp_count;
	}
	else if (current_state == State::WARP_MOTION)
	{
		if (warp_destination == pos()){ current_state = State::NORMAL; }
		else
		{
			pos() = pos() + v_warp;
			v_warp = v_warp - Point2D((2 * WARP_GAP / static_cast<Real>(pow(WARP_MOTION_TIME, 2))) * static_cast<Real>(cos(warp_angle)), (2 * WARP_GAP / static_cast<Real>(pow(WARP_MOTION_TIME, 2))) * static_cast<Real>(sin(warp_angle)));
			if (boost::geometry::distance(pos(), warp_destination) <= norm(v_warp)){ pos() = warp_destination; }
		}
	}
	else
	{
		if (up_input && pos().y() > 0)            { pos() += Point2D(0, -speed); }
		if (down_input && pos().y() < WH<Real>()) { pos() += Point2D(0, speed); }
		if (right_input && pos().x() < WW<Real>()){ pos() += Point2D(speed, 0); }
		if (left_input && pos().x() > 0)          { pos() += Point2D(-speed, 0); }
		warp_destination = pos();

		if (warp_charge < MAX_WARP_CHARGE)
		{
			if (++warp_charge >= MAX_WARP_CHARGE)
			{
				erect_se_flag_of("../../data/sound/button04b.mp3"); 
			}
		}
	}

	if (hyper_mode)
	{
		--hyper_mode_count; 
		if (hyper_mode_count < 0){ hyper_mode = false; }
	}

	area().get_physical_state().p = pos();

	velocity = pos() - prev_p;

	if (lock_icon_rate < 1.5)
	{
		lock_icon_rate += static_cast<Real>(0.05); 
		lock_icon_brt -= 7;
	}
	else
	{
		lock_icon_rate = 0; 
		lock_icon_brt = 255;
	}

	if (current_state == State::IMMORTAL)
	{
		--immortal_count;
		if (immortal_count < 0){ current_state = State::NORMAL; immortal_count = 0; }
	}

	++count;
}

void MyGameProject::Player::hit(void)
{
	static constexpr int IMMORTAL_TIME = 180;

	switch (current_state)
	{
	case State::NORMAL:
		current_state = State::IMMORTAL;
		--current_health;
		erect_se_flag_of("../../data/sound/sei_ge_garasu_hibi02.mp3");
		immortal_count = IMMORTAL_TIME;
		break;
	case State::WARP:
		break;
	case State::WARP_MOTION:
		break;
	case State::IMMORTAL:
		break;
	case State::ACTION_SELECT:
		break;
	default:
		break;
	}
}

void MyGameProject::Player::charge_warp_point(int _point)
{
	if (warp_charge < MAX_WARP_CHARGE)
	{
		if ((warp_charge += _point) >= MAX_WARP_CHARGE)
		{
			if (warp_charge > MAX_WARP_CHARGE){ warp_charge = MAX_WARP_CHARGE; }
			erect_se_flag_of("../../data/sound/button04b.mp3"); 
		}
	}
}

const MyGameProject::Point2D& MyGameProject::Player::v(void) const
{
	return velocity;
}

bool MyGameProject::Player::shot_if(void) const
{
	return shot_flag;
}

int MyGameProject::Player::health(void) const
{
	return current_health;
}

bool MyGameProject::Player::is_hyper_mode(void) const
{
	return hyper_mode;
}

bool MyGameProject::Player::is_locked(void) const
{
	return locked_flag;
}

void MyGameProject::Player::preperation(void)
{
	ImagePool::add("../../data/img/player.png");
	ImagePool::add("../../data/img/lock.png");
	ImagePool::add_multi("../../data/img/health_indicator.png",10,5,2,400,400);
}

MyGameProject::Player::~Player(void){}

