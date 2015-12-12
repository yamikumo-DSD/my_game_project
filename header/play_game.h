//play_game.h

#ifndef __PLAY_GAME_H
#define __PLAY_GAME_H

#include "scene.h"
#include "smart_ptr.h"
#include <vector>
#include "code_definition.h"

#define DEBUG_MODE 1

namespace MyGameProject
{
	class Device;
	class Player;
	class Bullet;
	class Option;
	class Shot;
	class SEManager;
	class ShortRangeWeapon;
	class MobEnemy;
	class Enemy;
	class Boss;
	class BackGround;
	class Item;
	struct EnemyOrder;

	class PlayGame : public MyGameProject::Scene, public boost::static_visitor<void>
	{
#if DEBUG_MODE
	private:
		mutable int draw_cost, 
					update_cost, 
					draw_back_ground_cost,
					draw_shot_cost,
					draw_bullet_cost;
#endif
	private:
		gp::smart_ptr<Device>                  device;
		gp::smart_ptr<BackGround>              back_ground;
		gp::smart_ptr<BackGround>              hyper_mode_back;
		gp::smart_ptr<Player>                  player;
		std::vector<gp::smart_ptr<Option>>     options;
		std::vector<gp::smart_ptr<Bullet>>     bullets;
		std::vector<gp::smart_ptr<Shot>>       shots;
		std::vector<gp::smart_ptr<Enemy>>      enemies;
		std::vector<gp::smart_ptr<MobEnemy>>   mob_enemies;
		std::vector<gp::smart_ptr<EnemyOrder>> enemy_orders;
		std::vector<gp::smart_ptr<Boss>>       bosses;
		std::vector<gp::smart_ptr<Item>>       items;
		gp::smart_ptr<ShortRangeWeapon>        short_range_weapon;
		gp::smart_ptr<SEManager>               se_manager;
		void hit_and_damage_dealer(void);
		void item_and_score_dealer(void);
		void shot_register(void);
		void convert_bullets(void);
		void load_stage_enemy_list(int _stage_num);
		int static_count;
		int count;
		unsigned int slow;

		CodeList code_list;
	public:
		PlayGame(void);
		void change_play_speed(float _speed);
		virtual void play_se(int _loudness) const override;
		virtual void draw(void) const override;
		virtual void update(void) override;
		virtual void load(void) override;
		virtual void unload(void) override;

		void exec(void);
		void operator()(const Code& _code);
		void operator()(const CreateMob& _create_mob);
		void operator()(const CreateBoss& _create_boss);

		virtual ~PlayGame(void);
	};
}

#endif