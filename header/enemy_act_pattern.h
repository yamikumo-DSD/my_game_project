//enemy_act_pattern.h

#ifndef __ENEMY_ACT_PATTERN_H
#define __ENEMY_ACT_PATTERN_H

#include "mob_enemy.h"

namespace MyGameProject
{
	extern MobEnemy::ActPattern default_pattern;
	extern MobEnemy::ActPattern pattern0;
	extern MobEnemy::ActPattern pattern1;
	extern MobEnemy::ActPattern pattern2;
	extern MobEnemy::ActPattern pattern3;
	extern MobEnemy::ActPattern pattern4;
	extern MobEnemy::ActPattern pattern5;
	extern MobEnemy::ActPattern pattern6;
	extern MobEnemy::ActPattern pattern7;
	extern MobEnemy::ActPattern pattern8;

	MobEnemy::ActPattern pattern100(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern101(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern102(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern103(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern104(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern105(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern106(const std::vector<Point2D>& _args);
	MobEnemy::ActPattern pattern107(const std::vector<Point2D>& _args);
}

#endif
