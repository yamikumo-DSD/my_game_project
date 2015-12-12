//shot.h

#ifndef __SHOT_H
#define __SHOT_H

#include "object.h"
#include "mathematics.h"
#include "smart_ptr.h"
#include <functional>

namespace MyGameProject
{
	class Enemy;

	class Shot : public MovingObject, public DynamicObject, public HitableObject
	{
	private:
		Real ang;
		int pow;
		virtual void custom_updater(void) = 0;
	public:
		Shot(const Shape& _shape,int _pow,Real _angle, const Point2D& _p);
		virtual void draw(void) const = 0;
		virtual void update(void) override final;
		virtual bool is_active(void) const = 0;
		virtual void hit(void) = 0;
		Real angle(void) const;
		void angle(Real _angle);
		int power(void) const; 
		static void preperation(void);
		virtual ~Shot(void);
	};

	class NormalShot : public Shot
	{
	private:
		virtual void custom_updater(void) override final;
		bool hit_flag;
		int count;
		int hit_moment;
	public:
		NormalShot(Real _angle,const Point2D& _p);
		virtual void draw(void) const override final;
		virtual void hit(void) override final;
		virtual bool is_active(void) const override final;
		static void preperation(void);
		virtual ~NormalShot(void);
	};

	class HyperShot2 : public Shot
	{
	private:
		virtual void custom_updater(void) override final;
		bool hit_flag;
		int count;
		int hit_moment;
	public:
		HyperShot2(Real _angle, const Point2D& _p);
		virtual void draw(void) const override final;
		virtual void hit(void) override final;
		virtual bool is_active(void) const override final;
		static void preperation(void);
		virtual ~HyperShot2(void);
	};

	class HomingShot : public Shot
	{
	private:
		bool hit_flag;
		gp::smart_ptr<Enemy> target;
		virtual void custom_updater(void) override final;
		std::function<gp::smart_ptr<Enemy>(HomingShot&)> find_target;
		int count;
		Real rot;
		Real mag;
		int brt;
		const int color_type;
		int count_of_hit;
		static constexpr size_t TRAIL_NUM = 20;
		std::array<Point2D, TRAIL_NUM> trail;
		std::array<std::array<Point2D, 2>, TRAIL_NUM> MX;
	public:
		HomingShot(std::function<gp::smart_ptr<Enemy>(HomingShot&)> _find_target, Real _angle, const Point2D& _p,int _color_type);
		HomingShot(std::function<gp::smart_ptr<Enemy>(HomingShot&)> _find_target,Real _angle, const Point2D& _p);
		virtual void draw(void) const override final;
		virtual void hit(void) override final;
		virtual bool is_active(void) const override final;
		static void preperation(void);
		virtual ~HomingShot(void);
	};

	class HomingLazer : public Shot
	{
	private:
		bool hit_flag;
		gp::smart_ptr<Enemy> target;
		virtual void custom_updater(void) override final;
		std::function<gp::smart_ptr<Enemy>(HomingLazer&)> find_target;
		int count;
		int count_of_hit;
		int brt;
		const int color_type;
		static constexpr size_t TRAIL_NUM = 15;
		std::array<Point2D, TRAIL_NUM> trail;
		std::array<std::array<Point2D, 2>, TRAIL_NUM + 1> MX;
		static std::array<int, TRAIL_NUM>& texture(void){ static std::array<int, TRAIL_NUM> tex; return tex; }
		static std::array<int, TRAIL_NUM>& hl_tex(void){ static std::array<int, TRAIL_NUM> tex; return tex; }
		const int tail_tex;
	public:
		HomingLazer(std::function<gp::smart_ptr<Enemy>(HomingLazer&)> _find_target, Real _angle, const Point2D& _p, int _color_type);
		virtual void draw(void) const override final;
		virtual void hit(void) override final;
		virtual bool is_active(void) const override final;
		static void preperation(void);
		virtual ~HomingLazer(void);
	};
}
#endif
