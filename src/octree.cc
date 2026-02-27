#include "octree.h"

namespace rove
{

template class octree<aabb<3>, int>;
template class octree<sphere<3>, int>;

}
