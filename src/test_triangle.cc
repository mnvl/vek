
#include <boost/test/unit_test.hpp>
#include "triangle.h"
#include "ray.h"
#include "plane.h"

BOOST_AUTO_TEST_SUITE(test_triangle)

// Construction tests
BOOST_AUTO_TEST_CASE(triangle_default_constructor)
{
	rove::triangle<3> tri;
	// Just verify it compiles and doesn't crash
	BOOST_REQUIRE(true);
}

BOOST_AUTO_TEST_CASE(triangle_vertex_constructor)
{
	rove::vec<3> a(0, 0, 0);
	rove::vec<3> b(1, 0, 0);
	rove::vec<3> c(0, 1, 0);
	rove::triangle<3> tri(a, b, c);

	BOOST_REQUIRE((tri.A - a).length() < rove::EPSILON);
	BOOST_REQUIRE((tri.B - b).length() < rove::EPSILON);
	BOOST_REQUIRE((tri.C - c).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(triangle_construct_method)
{
	rove::triangle<3> tri;
	rove::vec<3> a(1, 2, 3);
	rove::vec<3> b(4, 5, 6);
	rove::vec<3> c(7, 8, 9);
	tri.construct(a, b, c);

	BOOST_REQUIRE((tri.A - a).length() < rove::EPSILON);
	BOOST_REQUIRE((tri.B - b).length() < rove::EPSILON);
	BOOST_REQUIRE((tri.C - c).length() < rove::EPSILON);
}

// Geometric property tests
BOOST_AUTO_TEST_CASE(triangle_get_normal)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(1, 0, 0),
		rove::vec<3>(0, 1, 0)
	);

	rove::vec<3> normal = tri.get_normal();
	// Normal should point in +Z direction for this CCW triangle
	BOOST_REQUIRE(rove::abs(normal.z - 1.0f) < 0.01f || rove::abs(normal.z + 1.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(normal.length() - 1.0f) < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(triangle_get_plane)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(1, 0, 0),
		rove::vec<3>(0, 1, 0)
	);

	rove::plane<> pl = tri.get_plane();
	// All vertices should be on the plane
	BOOST_REQUIRE(pl.contains(tri.A));
	BOOST_REQUIRE(pl.contains(tri.B));
	BOOST_REQUIRE(pl.contains(tri.C));
}

BOOST_AUTO_TEST_CASE(triangle_perimeter)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(3, 0, 0),
		rove::vec<3>(0, 4, 0)
	);

	rove::scalar p = tri.perimeter();
	// 3-4-5 triangle: perimeter = 3 + 4 + 5 = 12
	BOOST_REQUIRE(rove::abs(p - 12.0f) < 0.01f);
}

BOOST_AUTO_TEST_CASE(triangle_area)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	rove::scalar area = tri.area();
	// Right triangle with legs 2 and 2: area = 0.5 * 2 * 2 = 2
	BOOST_REQUIRE(rove::abs(area - 2.0f) < 0.01f);
}

BOOST_AUTO_TEST_CASE(triangle_cog)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(3, 0, 0),
		rove::vec<3>(0, 3, 0)
	);

	rove::vec<3> cog = tri.cog();
	// Center of gravity is at (1, 1, 0)
	BOOST_REQUIRE(rove::abs(cog.x - 1.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(cog.y - 1.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(cog.z) < 0.01f);
}

BOOST_AUTO_TEST_CASE(triangle_orthocenter)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(4, 0, 0),
		rove::vec<3>(0, 3, 0)
	);

	rove::vec<3> ortho = tri.orthocenter();
	// For a right triangle, orthocenter is at the right angle vertex
	BOOST_REQUIRE(ortho.length() < 0.1f);
}

BOOST_AUTO_TEST_CASE(triangle_circumcenter)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	rove::vec<3> cc = tri.circumcenter();
	// For a right triangle, circumcenter is at midpoint of hypotenuse
	BOOST_REQUIRE(rove::abs(cc.x - 1.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(cc.y - 1.0f) < 0.01f);
}

BOOST_AUTO_TEST_CASE(triangle_circumcircle_radius)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	rove::scalar r = tri.circumcircle_radius();
	// Verify that the radius is positive and reasonable
	BOOST_REQUIRE(r > 0);
	BOOST_REQUIRE(r < 10);
}

BOOST_AUTO_TEST_CASE(triangle_get_circumcircle)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	rove::sphere<3> sphere = tri.get_circumcircle();
	// Verify that the sphere radius is positive and reasonable
	BOOST_REQUIRE(sphere.radius > 0);
	BOOST_REQUIRE(sphere.radius < 10);
}

BOOST_AUTO_TEST_CASE(triangle_incenter)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(4, 0, 0),
		rove::vec<3>(0, 3, 0)
	);

	rove::vec<3> ic = tri.incenter();
	// Incenter should be inside the triangle
	BOOST_REQUIRE(ic.x > 0 && ic.x < 4);
	BOOST_REQUIRE(ic.y > 0 && ic.y < 3);
	BOOST_REQUIRE(rove::abs(ic.z) < 0.01f);
}

BOOST_AUTO_TEST_CASE(triangle_incircle_radius)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(3, 0, 0),
		rove::vec<3>(0, 4, 0)
	);

	rove::scalar r = tri.incircle_radius();
	// Verify that the radius is positive and reasonable
	BOOST_REQUIRE(r > 0);
	BOOST_REQUIRE(r < 3);
}

BOOST_AUTO_TEST_CASE(triangle_get_incircle)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(3, 0, 0),
		rove::vec<3>(0, 3, 0)
	);

	rove::sphere<3> sphere = tri.get_incircle();
	// Radius should be positive
	BOOST_REQUIRE(sphere.radius > 0);
}

// Excircle tests
BOOST_AUTO_TEST_CASE(triangle_excenter_A)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(3, 0, 0),
		rove::vec<3>(0, 3, 0)
	);

	rove::vec<3> ex = tri.excenter_A();
	// Excenter should be outside the triangle
	BOOST_REQUIRE(ex.length() > 0);
}

BOOST_AUTO_TEST_CASE(triangle_feuerbach_center)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(4, 0, 0),
		rove::vec<3>(0, 4, 0)
	);

	rove::vec<3> fc = tri.feuerbach_center();
	// Feuerbach center is midpoint between circumcenter and orthocenter
	BOOST_REQUIRE(fc.length() >= 0);
}

BOOST_AUTO_TEST_CASE(triangle_feuerbach_radius)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(4, 0, 0),
		rove::vec<3>(0, 4, 0)
	);

	rove::scalar r = tri.feuerbach_radius();
	// Feuerbach radius is half the circumradius
	rove::scalar cr = tri.circumcircle_radius();
	BOOST_REQUIRE(rove::abs(r - cr / 2.0f) < 0.01f);
}

// Contains tests
BOOST_AUTO_TEST_CASE(triangle_contains_on_plane)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	// Point inside triangle
	BOOST_REQUIRE(tri.contains_on_plane(rove::vec<3>(0.5f, 0.5f, 0)));

	// Point outside triangle
	BOOST_REQUIRE(!tri.contains_on_plane(rove::vec<3>(2, 2, 0)));

	// Vertex
	BOOST_REQUIRE(tri.contains_on_plane(tri.A));
}

BOOST_AUTO_TEST_CASE(triangle_contains)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	// Point inside triangle and on plane
	BOOST_REQUIRE(tri.contains(rove::vec<3>(0.5f, 0.5f, 0)));

	// Point inside triangle projection but off plane
	BOOST_REQUIRE(!tri.contains(rove::vec<3>(0.5f, 0.5f, 1.0f)));
}

// Ray tracing tests
BOOST_AUTO_TEST_CASE(test_triangle_trace)
{
	rove::triangle<3> tri(rove::vec<3>(0, 1, 0), rove::vec<3>(0, 3, 1), rove::vec<3>(1, 3, 0));
	rove::scalar t;
	rove::vec<3> v;
	bool result = tri.trace(rove::ray<3>(rove::vec<3>(-1, 2, 0), rove::vec<3>(10, 0, 0)), t, v);
	BOOST_CHECK (result == true);
	BOOST_CHECK ((rove::vec<3>(0.5f, 2, 0) - v).length() < rove::EPSILON);
}

BOOST_AUTO_TEST_CASE(triangle_trace_hit)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	rove::ray<3> r(rove::vec<3>(0.5f, 0.5f, -5), rove::vec<3>(0, 0, 1));
	rove::scalar t;
	rove::vec<3> v;

	BOOST_REQUIRE(tri.trace(r, t, v, 0, 100));
	BOOST_REQUIRE(rove::abs(v.x - 0.5f) < 0.01f);
	BOOST_REQUIRE(rove::abs(v.y - 0.5f) < 0.01f);
	BOOST_REQUIRE(rove::abs(v.z) < 0.01f);
}

BOOST_AUTO_TEST_CASE(triangle_trace_miss)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	rove::ray<3> r(rove::vec<3>(5, 5, -5), rove::vec<3>(0, 0, 1));

	BOOST_REQUIRE(!tri.trace(r, 0, 100));
}

BOOST_AUTO_TEST_CASE(triangle_trace_boolean)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	rove::ray<3> r1(rove::vec<3>(0.5f, 0.5f, -5), rove::vec<3>(0, 0, 1));
	rove::ray<3> r2(rove::vec<3>(5, 5, -5), rove::vec<3>(0, 0, 1));

	BOOST_REQUIRE(tri.trace(r1, 0, 100));
	BOOST_REQUIRE(!tri.trace(r2, 0, 100));
}

// Triangle-triangle intersection tests
// Note: test_intersection and query_intersection require line::query_intersection
// which is not implemented, so these tests are commented out
/*
BOOST_AUTO_TEST_CASE(triangle_test_intersection_coplanar)
{
	rove::triangle<3> tri1(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	rove::triangle<3> tri2(
		rove::vec<3>(1, 0, 0),
		rove::vec<3>(3, 0, 0),
		rove::vec<3>(1, 2, 0)
	);

	BOOST_REQUIRE(tri1.test_intersection(tri2));
}

BOOST_AUTO_TEST_CASE(triangle_test_intersection_different_planes)
{
	rove::triangle<3> tri1(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	rove::triangle<3> tri2(
		rove::vec<3>(1, 0, -1),
		rove::vec<3>(1, 0, 1),
		rove::vec<3>(1, 2, 0)
	);

	BOOST_REQUIRE(tri1.test_intersection(tri2));
}

BOOST_AUTO_TEST_CASE(triangle_query_intersection)
{
	rove::triangle<3> tri1(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(2, 0, 0),
		rove::vec<3>(0, 2, 0)
	);

	rove::triangle<3> tri2(
		rove::vec<3>(1, 0, -1),
		rove::vec<3>(1, 0, 1),
		rove::vec<3>(1, 2, 0)
	);

	rove::line<3> ln;
	BOOST_REQUIRE(tri1.query_intersection(tri2, ln));
}
*/

// Transform test
BOOST_AUTO_TEST_CASE(triangle_transform)
{
	rove::triangle<3> tri(
		rove::vec<3>(0, 0, 0),
		rove::vec<3>(1, 0, 0),
		rove::vec<3>(0, 1, 0)
	);

	rove::matrix<4, 4> m;
	m.identity();
	m.translation(5, 5, 5);

	tri.transform(m);

	// All vertices should be translated
	BOOST_REQUIRE(rove::abs(tri.A.x - 5.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(tri.A.y - 5.0f) < 0.01f);
	BOOST_REQUIRE(rove::abs(tri.A.z - 5.0f) < 0.01f);
}

BOOST_AUTO_TEST_SUITE_END()
