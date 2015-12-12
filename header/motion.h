//motion.h

#ifndef __MOTION_H
#define __MOTION_H

#include <cmath>
#include "msgbox.h"
#include <boost/lexical_cast.hpp>

namespace MyGameProject
{
	template<class Vector2D,class TimeType>
	class LinearMotionOfUniformAcceleration2D
	{
	private:
		const Vector2D dpt;
		const Vector2D dst;
		const TimeType term;
	public:
		LinearMotionOfUniformAcceleration2D(const Vector2D& _start_point, const Vector2D& _end_point, TimeType _dt)
			:dpt(_start_point), dst(_end_point), term(_dt){}
		Vector2D v0(void) const
		{
			using RealType = decltype(dpt.x());
			return Vector2D((dst - dpt).x() * (static_cast<RealType>(2) / static_cast<RealType>(term)), (dst - dpt).y() * (static_cast<RealType>(2) / static_cast<RealType>(term)));
		}
		Vector2D a(void) const
		{
			using RealType = decltype(dpt.x());
			return Vector2D((dst - dpt).x() * (static_cast<RealType>(-2) / static_cast<RealType>(std::pow(term, 2))), (dst - dpt).y() * (static_cast<RealType>(-2) / static_cast<RealType>(std::pow(term, 2))));
		}
	};

	template<class Vector2D, class TimeType>
	LinearMotionOfUniformAcceleration2D<Vector2D, TimeType> create_linear_motion_of_uniform_acceleration_2D
		(
		const Vector2D& _start_point, 
		const Vector2D& _end_point, 
		TimeType _dt
		)
	{
		return LinearMotionOfUniformAcceleration2D<Vector2D, TimeType>(_start_point,_end_point,_dt);
	}
}

#endif
