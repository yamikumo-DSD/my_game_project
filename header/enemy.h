//enemy.h

#ifndef __ENEMY_H
#define __ENEMY_H

#include "object.h"
#include <functional>
#include "mathematics.h"
#include <string>
#include "smart_ptr.h"

namespace MyGameProject
{
	class Player;
	struct EnemyOrder;

	class Enemy : public MovingObject, public DynamicObject, public HitableObject
	{
	public:
		enum class Class
		{
			Boss,
			LARGE,
			MIDDLE,
			SMALL
		};
	private:
		const Class enemy_class;
		const Player& player;
		int count;
		Real ang;
		virtual void custom_updater(void) = 0;
	public:
		Enemy
			(
			const Point2D& _initial_pos,
			Real _initial_angle,
			Class _enemy_class,
			const Player& _player,
			const Shape& _hitbox_shape
			);
		virtual void draw(void) const = 0;
		Real angle(void) const;
		void angle(Real _ang);
		Class get_class(void) const;
		virtual void hit(int _shot_power) = 0;
		int get_count(void) const;
		virtual void update(void) override final;
		const Player& player_ref(void) const;
		virtual ~Enemy(void);
	};
}

#endif
