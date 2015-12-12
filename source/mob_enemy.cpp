//mob_enemy.cpp

#include "mob_enemy.h"
#include "enemy_order.h"
#include "concrete_enemy.h"
#include <exception>
#include "se_manager.h"
#include "bullet.h"

gp::smart_ptr<MyGameProject::MobEnemy> MyGameProject::MobEnemy::create
(
const std::string& _name, 
BulletPtrContainer& _bullets,
SEManager& _se_manager,
gp::smart_ptr<EnemyOrder> _order,
const Player& _player
)
{
	if (_name == "Crow"){ return gp::make_smart<Crow>(_bullets,_se_manager,_order, _player); }
	else if (_name == "EmergeFromCloud"){ return gp::make_smart<EmergeFromCloud>(_bullets, _se_manager, _order, _player); }
	else{ throw std::runtime_error("Undefined type of enemy."); }
}

MyGameProject::MobEnemy::MobEnemy
(
BulletPtrContainer& _bullets,
SEManager& _se_manager,
gp::smart_ptr<EnemyOrder> _order,
int _initial_health, Class _enemy_class,
const Player& _player,
const Shape& _hitbox_shape
)
	:bullets(_bullets),
	se_manager(_se_manager),
	order(_order),
	hp(_initial_health),
	act_pattern(_order->act_pattern),
	Enemy(_order->initial_pos,_order->initial_angle,_enemy_class,_player,_hitbox_shape)
{}

void MyGameProject::MobEnemy::custom_updater(void)
{
	if (hp < 0){ set_flag_off(); }
	auto pre_pos = pos();
	accessory_custom_updater();
	act_pattern(*this);
	v = pos() - pre_pos;
}

const MyGameProject::Point2D& MyGameProject::MobEnemy::velocity(void) const
{
	return v;
}

MyGameProject::Point2D& MyGameProject::MobEnemy::velocity(void)
{
	return v;
}

void MyGameProject::MobEnemy::hit(int _shot_power)
{
	hp -= _shot_power;
}

MyGameProject::MobEnemy::BulletPtrContainer& MyGameProject::MobEnemy::get_bullets_ref(void)
{
	return bullets;
}

MyGameProject::SEManager& MyGameProject::MobEnemy::get_se_manager_ref(void)
{
	return se_manager;
}

void MyGameProject::MobEnemy::preperation(void)
{
	Crow::preperation();
	EmergeFromCloud::preperation();
}

MyGameProject::MobEnemy::~MobEnemy(void){}