
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "ray.h"

BOOST_AUTO_TEST_SUITE (ray)

BOOST_AUTO_TEST_CASE (ray_point_distance)
{
	vek::ray<3> r(vek::vec<3>(0, 0, 0), vek::vec<3>(1, 0, 0));
	BOOST_REQUIRE(vek::abs(r.distance_sq(vek::vec<3>(0, 1, 0)) - 1) < vek::EPSILON);
	BOOST_REQUIRE(vek::abs(r.distance_sq(vek::vec<3>(100, 1, 0)) - 1) < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE (ray_ray_distance)
{
	vek::ray<3> r1(vek::vec<3>(0, 0, 0), vek::vec<3>(1, 0, 0));
	vek::ray<3> r2(vek::vec<3>(100, 1, 0), vek::vec<3>(0, 0, 1));

	BOOST_REQUIRE(vek::abs(r1.distance(r2) - 1) < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE (ray_ray_distance_2)
{
	vek::ray<3> r1(vek::vec<3>(0, 0, 0), vek::vec<3>(1, 0, 0));
	vek::ray<3> r2(vek::vec<3>(100, 1, 0), vek::vec<3>(1, 0, 0));

	BOOST_REQUIRE(r1.distance(r2) == 0);
}

BOOST_AUTO_TEST_CASE (ray_ray_intersection_1a)
{
	vek::ray<2> r1, r2;
	vek::scalar s;

	r1.r0.set(0, 0);
	r1.a.set(1, 1);

	r2.r0.set(1, 0);
	r2.a.set(-1, 1);

	BOOST_REQUIRE (r1.query_intersection(r2, s) == true);

	vek::vec<2> p1 = r1.apply(s);
	BOOST_REQUIRE (r2.contains(p1));

	BOOST_REQUIRE (vek::abs(s - 0.5f) < vek::EPSILON);
}

BOOST_AUTO_TEST_CASE (ray_ray_intersection_1b)
{
	vek::ray<2> r1, r2;
	vek::scalar s;

	r1.r0.set(0.846187f, 0.436567f);
	r1.a.set(0.990295f, -0.700491f);

	r2.r0.set(-0.39787f, -0.0618f);
	r2.a.set(0.818842f, -0.538987f);

	BOOST_REQUIRE (r1.query_intersection(r2, s) == true);

	vek::vec<2> p1 = r1.apply(s);
	BOOST_REQUIRE (r2.contains(p1, 0.001f));
}

BOOST_AUTO_TEST_CASE (ray_ray_intersection_2)
{
	using vek::random;

	for (int i = 0; i < 1000; i++)
	{
		vek::ray<2> r1(vek::vec<2>(random(-1, 1), random(-1, 1)), vek::vec<2>(random(-1, 1), random(-1, 1)));
		vek::ray<2> r2(vek::vec<2>(random(-1, 1), random(-1, 1)), vek::vec<2>(random(-1, 1), random(-1, 1)));

		vek::scalar s;

		if(r1.query_intersection(r2, s))
		{
			vek::vec<2> p1 = r1.apply(s);
			BOOST_REQUIRE (r2.contains(p1, 0.001f));
		}
	}
}

BOOST_AUTO_TEST_CASE (ray_ray_intersection_3)
{
	using vek::random;

	for (int i = 0; i < 1000; i++)
	{
		vek::ray<3> r1(vek::vec<3>(random(-1, 1), random(-1, 1), random(-1, 1)),
			vek::vec<3>(random(-1, 1), random(-1, 1), random(-1, 1)));

		vek::ray<3> r2(vek::vec<3>(random(-1, 1), random(-1, 1), random(-1, 1)),
			vek::vec<3>(random(-1, 1), random(-1, 1), random(-1, 1)));

		vek::scalar s;

		if(r1.query_intersection(r2, s))
		{
			vek::vec<3> p1 = r1.apply(s);
			BOOST_REQUIRE (r2.contains(p1, 0.001f));
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
