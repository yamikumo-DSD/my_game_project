//enemy_order.cpp

#include "enemy_order.h"
#include "csv_reader.h"
#include "enemy_act_pattern.h"

gp::smart_ptr<MyGameProject::EnemyOrder> MyGameProject::EnemyOrder::make_order
(
std::string _name,
int _register_time,
Point2D _initial_pos,
Real _initial_angle,
MobEnemy::ActPattern _act_pattern
)
{
	auto order = gp::make_smart<EnemyOrder>();
	order->name = _name;
	order->register_time = _register_time;
	order->initial_pos = _initial_pos;
	order->initial_angle = _initial_angle;
	order->act_pattern = _act_pattern;
	return order;
}

gp::smart_ptr<MyGameProject::EnemyOrder> MyGameProject::string_to_enemy_order(const std::string& _line)
{
	using namespace std;

	auto data = CSV::line_to_tuple<std::string,int, Real, Real, Real, int>(_line);
	MobEnemy::ActPattern pattern;
	switch (get<5>(data))
	{
	case 0:
		pattern = pattern0;
		break;
	case 1:
		pattern = pattern1;
		break;
	case 2:
		pattern = pattern2;
		break;
	case 3:
		pattern = pattern3;
		break;
	case 4:
		pattern = pattern4;
		break;
	case 5:
		pattern = pattern5;
		break;
	case 6:
		pattern = pattern6;
		break;
	default:
		pattern = default_pattern;
		break;
	}
	return EnemyOrder::make_order(get<0>(data),get<1>(data),Point2D(get<2>(data),get<3>(data)),get<4>(data),pattern);
}
