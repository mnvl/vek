
#include <boost/test/unit_test.hpp>
#include "aabb.h"
#include "obb.h"
#include "capsule.h"

BOOST_AUTO_TEST_SUITE (capsule)

BOOST_AUTO_TEST_CASE (bounds_1)
{
	using vek::random;

	unsigned false_positives = 0;

	for (int i = 0; i < 100; i++)
	{
		vek::capsule<3> c(vek::vec<3>(random(), random(), random()), vek::vec<3>(random(), random(), random()),
			vek::abs(random()) + 0.001f);

		vek::aabb<3> b;
		c.get_aabb(b);

		for (int j = 0; j < 100; j++)
		{
			vek::vec<3> point(random(), random(), random());

			bool b1 = c.contains(point);
			bool b2 = b.contains(point);

			if (b1 != b2)
			{
				// bounding box could have only false positioves
				BOOST_REQUIRE(b1 == false && b2 == true);

				++false_positives;
			}
		}
	}

}

BOOST_AUTO_TEST_SUITE_END()
