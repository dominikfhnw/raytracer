typedef struct vec3 {
	FLOAT x;
	FLOAT y;
	FLOAT z;
} vec3;

CONSTEXPR vec3 add(const vec3 a, const vec3 b)
{
	vec3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

CONSTEXPR vec3 sub(const vec3 a, const vec3 b)
{
	vec3 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}

CONSTEXPR vec3 hadamard(const vec3 a, const vec3 b)
{
	vec3 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return result;
}

CONSTEXPR vec3 changesign(const vec3 a)
{
	vec3 result;
	result.x = -a.x;
	result.y = -a.y;
	result.z = -a.z;
	return result;
}

CONSTEXPR FLOAT dotP(const vec3 a, const vec3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

/*
CONSTEXPR FLOAT len1(const vec3 a)
{
	return SQRT(a.x*a.x + a.y*a.y + a.z*a.z);
}
*/

CONSTEXPR FLOAT len(const vec3 a)
{
	return SQRT(dotP(a,a));
}

/*
CONSTEXPR bool normal(const vec3 a)
{
	if (!isfinite(a.x))
		return false;
	if (!isfinite(a.y))
		return false;
	if (!isfinite(a.z))
		return false;
	return true;
}

CONSTEXPR vec3 limit(vec3 a, const FLOAT l)
{
	a.x = a.x > l ? l : a.x;
	a.y = a.y > l ? l : a.y;
	a.z = a.z > l ? l : a.z;
	return a;
}
*/

CONSTEXPR vec3 norm(const vec3 a)
{
	FLOAT l = len(a);
	vec3 result;
	result.x = a.x / l;
	result.y = a.y / l;
	result.z = a.z / l;
	return result;
}

CONSTEXPR vec3 normV(const vec3 a, const vec3 b)
{
	return norm(sub(b, a));
}

CONSTEXPR vec3 crossP(const vec3 a, const vec3 b)
{
	vec3 result;
	result.x = (a.y * b.z) - (a.z * b.y);
	result.y = (a.z * b.x) - (a.x * b.z);
	result.z = (a.x * b.y) - (a.y * b.x);
	return result;
}

CONSTEXPR vec3 scalar_mult(const vec3 a, const FLOAT amount)
{
	vec3 result;
	result.x = a.x * amount;
	result.y = a.y * amount;
	result.z = a.z * amount;
	return result;
}

/*
CONSTEXPR vec3 lerp(const vec3 a, vec3 b, const FLOAT amount)
{
	assert(amount >= 0);
	assert(amount <= 1);
	return add(scalar_mult(a, amount), scalar_mult(b, 1-amount));
}
*/
