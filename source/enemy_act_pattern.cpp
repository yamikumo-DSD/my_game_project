//enemy_act_pattern.cpp

#include "enemy_act_pattern.h"
#include "bullet.h"
#include "find_vacant_object.h"
#include <boost/math/constants/constants.hpp>
#include "bullet_behavior.h"
#include "mathematics.h"
#include "se_manager.h"
#include "ranged_uniform_random.h"
#include "player.h"

namespace
{
	using namespace boost::math::constants;
	using namespace MyGameProject;
}

MyGameProject::MobEnemy::ActPattern MyGameProject::default_pattern = [](MyGameProject::MobEnemy& _enemy)->void{};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern0 = [](MyGameProject::MobEnemy& _enemy)->void
{
	const auto count = _enemy.get_count();

	if (count == 0){ _enemy.velocity() = Point2D(9,-3); }
	_enemy.velocity().x(_enemy.velocity().x() - static_cast<Real>(0.1)); 
	if (count > 70){ _enemy.velocity().y(_enemy.velocity().y() + static_cast<Real>(0.05)); }

	if (count % 8 == 0)
	{
		*find_vacant_object(_enemy.get_bullets_ref()) = Bullet::create("Cutter", _enemy, _enemy.player_ref(), _enemy.pos(), pi<Real>() / 2, straight_course(9));
		_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
	}

	_enemy.pos() = _enemy.pos() + _enemy.velocity();
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern1 = [](MyGameProject::MobEnemy& _enemy)->void
{
	const auto count = _enemy.get_count();

	if (count == 0){ _enemy.velocity() = Point2D(-9, -3); }
	_enemy.velocity().x(_enemy.velocity().x() + static_cast<Real>(0.1));
	if (count > 70){ _enemy.velocity().y(_enemy.velocity().y() + static_cast<Real>(0.05)); }

	if (count % 8 == 0)
	{
		*find_vacant_object(_enemy.get_bullets_ref()) = Bullet::create("Cutter", _enemy, _enemy.player_ref(), _enemy.pos(), pi<Real>() / 2, straight_course(9));
		_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
	}
	_enemy.pos() = _enemy.pos() + _enemy.velocity();
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern2 = [](MyGameProject::MobEnemy& _enemy)->void
{
	const auto count = _enemy.get_count();

	if (count == 0){ _enemy.velocity() = Point2D(0, -5); }

	if (count % 4 == 0)
	{
		*find_vacant_object(_enemy.get_bullets_ref()) = Bullet::create("Cutter", _enemy, _enemy.player_ref(), _enemy.pos(), pi<Real>() / 2, straight_course(9));
		_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
	}

	_enemy.pos() = _enemy.pos() + _enemy.velocity();
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern3 = [](MyGameProject::MobEnemy& _enemy)->void
{
	const auto count = _enemy.get_count();

	if (count == 0){ _enemy.velocity() = Point2D(0, 2); }

	if (count % 60 == 0)
	{
		gp::RangedUniformRandom generator(-pi<Real>(),pi<Real>());
		Real r = static_cast<Real>(generator.rand());
		static constexpr int N = 10;
		for (int i = 0; i < N;++i)
		{
			*find_vacant_object(_enemy.get_bullets_ref()) = Bullet::create("Typical", _enemy, _enemy.player_ref(), _enemy.pos(),r + i * (2 * pi<Real>() / N), straight_course(4));
		}
		_enemy.get_se_manager_ref().erect_play_flag_of("../../data/sound/enemy_shot.wav");
	}

	_enemy.pos() = _enemy.pos() + _enemy.velocity();
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern4 = [](MyGameProject::MobEnemy& _enemy)->void
{
	const auto count = _enemy.get_count();

	//Motion control.
	if (count == 0){ _enemy.velocity() = Point2D(0, -2); }
	else if (count >= 180 && count <= 200)
	{
		_enemy.velocity() = Point2D(0, count / 10 - 20); 
	}
	
	//Bullet control.
	if (count > 200 && count <= 600)
	{
		if (count % 60 == 0)
		{
			*find_vacant_object(_enemy.get_bullets_ref()) =
				Bullet::create
				(
				"Typical",
				_enemy,
				_enemy.player_ref(),
				_enemy.pos(),
				angle_of(_enemy.player_ref().pos() - _enemy.pos()),
				straight_course(10)
				);
		}
	}

	_enemy.pos() = _enemy.pos() + _enemy.velocity();
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern5 = [](MyGameProject::MobEnemy& _enemy)->void
{
};

MyGameProject::MobEnemy::ActPattern MyGameProject::pattern6 = [](MyGameProject::MobEnemy& _enemy)->void
{
};

MobEnemy::ActPattern MyGameProject::pattern100(const std::vector<Point2D>& _args)
{
	Real v0;
	if (_args.size() == 1) { v0 = _args[0].x(); }
	else { throw std::runtime_error("No argument is not allowed."); }

	return [&, v0](MyGameProject::MobEnemy& _enemy)->void
	{
		const auto count = _enemy.get_count();

		//Motion control.
		if (count == 0)
		{ 
			_enemy.velocity() = Point2D(0, v0);
		}
		else if (count >= 180 && count <= 200)
		{
			_enemy.velocity() = Point2D(0, count / 10 - 20);
		}

		//Bullet control.
		if (count > 200 && count <= 600)
		{
			if (count % 60 == 0)
			{
				*find_vacant_object(_enemy.get_bullets_ref()) =
					Bullet::create
					(
					"Typical",
					_enemy,
					_enemy.player_ref(),
					_enemy.pos(),
					angle_of(_enemy.player_ref().pos() - _enemy.pos()),
					straight_course(10)
					);
			}
		}

		_enemy.pos() = _enemy.pos() + _enemy.velocity();
	};
}
MobEnemy::ActPattern MyGameProject::pattern101(const std::vector<Point2D>& _args)
{
	return pattern0;
}
MobEnemy::ActPattern MyGameProject::pattern102(const std::vector<Point2D>& _args)
{
	return pattern0;
}
MobEnemy::ActPattern MyGameProject::pattern103(const std::vector<Point2D>& _args)
{
	return pattern0;
}
MobEnemy::ActPattern MyGameProject::pattern104(const std::vector<Point2D>& _args)
{
	return pattern0;
}
MobEnemy::ActPattern MyGameProject::pattern105(const std::vector<Point2D>& _args)
{
	return pattern0;
}
MobEnemy::ActPattern MyGameProject::pattern106(const std::vector<Point2D>& _args)
{
	return pattern0;
}
MobEnemy::ActPattern MyGameProject::pattern107(const std::vector<Point2D>& _args)
{
	return pattern0;
}
