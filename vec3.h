typedef struct vec3 {
	FLOAT x;
	FLOAT y;
	FLOAT z;
} vec3;

CONSTEXPR vec3 add(vec3 a, const vec3 b)
{
	a.x = a.x + b.x;
	a.y = a.y + b.y;
	a.z = a.z + b.z;
	return a;
}

CONSTEXPR vec3 sub(vec3 a, const vec3 b)
{
	a.x = a.x - b.x;
	a.y = a.y - b.y;
	a.z = a.z - b.z;
	return a;
}

CONSTEXPR vec3 hadamard(vec3 a, const vec3 b)
{
	a.x = a.x * b.x;
	a.y = a.y * b.y;
	a.z = a.z * b.z;
	return a;
}

CONSTEXPR FLOAT dotP(const vec3 a, const vec3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

CONST FLOAT len1(const vec3 a)
{
	return SQRT(a.x*a.x + a.y*a.y + a.z*a.z);
}

CONSTEXPR FLOAT len(const vec3 a)
{
	return SQRT(dotP(a,a));
}

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

CONSTEXPR vec3 norm(vec3 a)
{
	FLOAT l = len(a);
	a.x = a.x / l;
	a.y = a.y / l;
	a.z = a.z / l;
	return a;
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

CONSTEXPR vec3 scalar_mult(vec3 a, const FLOAT amount)
{
	a.x = a.x * amount;
	a.y = a.y * amount;
	a.z = a.z * amount;
	return a;
}

CONSTEXPR vec3 lerp(const vec3 a, vec3 b, const FLOAT amount)
{
	assert(amount >= 0);
	assert(amount <= 1);
	return add(scalar_mult(a, amount), scalar_mult(b, 1-amount));
}
