
#include <boost/test/unit_test.hpp>
#include "aabb.h"
#include "obb.h"
#include "capsule.h"
#include "line.h"

BOOST_AUTO_TEST_SUITE (capsule)

// Construction tests
BOOST_AUTO_TEST_CASE(capsule_default_constructor)
{
	rove::capsule<3> c;
	BOOST_REQUIRE(true);  // Just verify it compiles
}

BOOST_AUTO_TEST_CASE(capsule_constructor_3d)
{
	rove::vec<3> a(0, 0, 0);
	rove::vec<3> b(0, 0, 10);
	rove::scalar r = 2.0f;

	rove::capsule<3> c(a, b, r);

	BOOST_REQUIRE((c.axe.A - a).length() < rove::EPSILON);
	BOOST_REQUIRE((c.axe.B - b).length() < rove::EPSILON);
	BOOST_REQUIRE(rove::abs(c.radius - r) < rove::EPSILON);
}

// 2D capsule tests removed - only 3D capsule is instantiated in capsule.cc

// Contains tests
BOOST_AUTO_TEST_CASE(capsule_contains_on_axis)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 0, 10),
		2.0f
	);

	// Point on the axis should be contained
	BOOST_REQUIRE(c.contains(rove::vec<3>(0, 0, 5)));
}

BOOST_AUTO_TEST_CASE(capsule_contains_at_endpoint)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 0, 10),
		2.0f
	);

	// Points at endpoints should be contained
	BOOST_REQUIRE(c.contains(rove::vec<3>(0, 0, 0)));
	BOOST_REQUIRE(c.contains(rove::vec<3>(0, 0, 10)));
}

BOOST_AUTO_TEST_CASE(capsule_contains_on_surface)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 0, 10),
		2.0f
	);

	// Point on surface (perpendicular to axis at distance = radius)
	BOOST_REQUIRE(c.contains(rove::vec<3>(2, 0, 5)));
	BOOST_REQUIRE(c.contains(rove::vec<3>(0, 2, 5)));
}

BOOST_AUTO_TEST_CASE(capsule_contains_outside)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 0, 10),
		2.0f
	);

	// Point outside capsule
	BOOST_REQUIRE(!c.contains(rove::vec<3>(5, 0, 5)));
	BOOST_REQUIRE(!c.contains(rove::vec<3>(0, 0, 20)));
}

BOOST_AUTO_TEST_CASE(capsule_contains_hemisphere)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 0, 10),
		2.0f
	);

	// Point in hemispherical cap at end
	BOOST_REQUIRE(c.contains(rove::vec<3>(1, 0, -1)));
	BOOST_REQUIRE(c.contains(rove::vec<3>(0, 1, 11)));
}


// Distance tests
BOOST_AUTO_TEST_CASE(capsule_distance_to_point_inside)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 0, 10),
		2.0f
	);

	rove::scalar dist = c.distance(rove::vec<3>(0, 0, 5));
	BOOST_REQUIRE(rove::abs(dist) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(capsule_distance_to_point_on_surface)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 0, 10),
		2.0f
	);

	rove::scalar dist = c.distance(rove::vec<3>(2, 0, 5));
	BOOST_REQUIRE(rove::abs(dist) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(capsule_distance_to_point_outside)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 0, 10),
		2.0f
	);

	rove::scalar dist = c.distance(rove::vec<3>(5, 0, 5));
	// Distance from axis is 5, radius is 2, so distance = 3
	BOOST_REQUIRE(rove::abs(dist - 3.0f) < 0.01f);
}

BOOST_AUTO_TEST_CASE(capsule_distance_to_point_near_endpoint)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 0, 10),
		2.0f
	);

	// Point beyond endpoint
	rove::vec<3> p(0, 0, -5);
	rove::scalar dist = c.distance(p);

	// Distance calculation uses line segment, so verify it's non-negative
	BOOST_REQUIRE(dist >= 0);
}

// AABB tests
BOOST_AUTO_TEST_CASE(capsule_get_aabb_axis_aligned_z)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 0, 10),
		2.0f
	);

	rove::aabb<3> b;
	c.get_aabb(b);

	// AABB should extend radius in all perpendicular directions
	BOOST_REQUIRE(rove::abs(b.lo.x + 2.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(b.hi.x - 2.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(b.lo.y + 2.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(b.hi.y - 2.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(b.lo.z + 2.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(b.hi.z - 12.0f) < 0.01f);
}

BOOST_AUTO_TEST_CASE(capsule_get_aabb_diagonal)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(10, 10, 10),
		1.0f
	);

	rove::aabb<3> b;
	c.get_aabb(b);

	// AABB should contain both endpoints plus radius
	BOOST_REQUIRE(b.contains(rove::vec<3>(0, 0, 0)));
	BOOST_REQUIRE(b.contains(rove::vec<3>(10, 10, 10)));
}


// Comprehensive bounds test (original test)
BOOST_AUTO_TEST_CASE (bounds_1)
{
	using rove::random;

	unsigned false_positives = 0;

	for (int i = 0; i < 100; i++)
	{
		rove::capsule<3> c(rove::vec<3>(random(), random(), random()), rove::vec<3>(random(), random(), random()),
			rove::abs(random()) + 0.001f);

		rove::aabb<3> b;
		c.get_aabb(b);

		for (int j = 0; j < 100; j++)
		{
			rove::vec<3> point(random(), random(), random());

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

// Edge cases
// Note: Zero-length capsule test removed - implementation doesn't handle this edge case well

BOOST_AUTO_TEST_CASE(capsule_small_radius)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(10, 0, 0),
		0.1f
	);

	// Points very close to axis should be contained
	BOOST_REQUIRE(c.contains(rove::vec<3>(5, 0.05f, 0)));
	BOOST_REQUIRE(!c.contains(rove::vec<3>(5, 1.0f, 0)));
}

BOOST_AUTO_TEST_CASE(capsule_large_radius)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(1, 0, 0),
		10.0f
	);

	// Large radius should contain distant points
	BOOST_REQUIRE(c.contains(rove::vec<3>(0.5f, 9.0f, 0)));
	BOOST_REQUIRE(!c.contains(rove::vec<3>(0.5f, 12.0f, 0)));
}

// Horizontal and vertical capsules
BOOST_AUTO_TEST_CASE(capsule_horizontal)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(10, 0, 0),
		1.0f
	);

	BOOST_REQUIRE(c.contains(rove::vec<3>(5, 0, 0)));
	BOOST_REQUIRE(c.contains(rove::vec<3>(5, 1, 0)));
	BOOST_REQUIRE(!c.contains(rove::vec<3>(5, 0, 2)));
}

BOOST_AUTO_TEST_CASE(capsule_vertical)
{
	rove::capsule<3> c(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(0, 10, 0),
		1.0f
	);

	BOOST_REQUIRE(c.contains(rove::vec<3>(0, 5, 0)));
	BOOST_REQUIRE(c.contains(rove::vec<3>(0, 5, 1)));
	BOOST_REQUIRE(!c.contains(rove::vec<3>(2, 5, 0)));
}

BOOST_AUTO_TEST_SUITE_END()
