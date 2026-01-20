
#include "aabb.h"
#include "obb.h"
#include "capsule.h"

namespace vek
{

template<int N, class T> void
capsule<N, T>::get_aabb(aabb<ARITY, scalar_t> &bounds) const
{
	vec_t diagonal(radius, radius, radius);

	bounds.lo = bounds.hi = axe.A;
	bounds.extend(axe.B);

	bounds.extend(axe.A - diagonal);
	bounds.extend(axe.A + diagonal);

	bounds.extend(axe.B - diagonal);
	bounds.extend(axe.B + diagonal);
}

template class capsule<3>;

}
