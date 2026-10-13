typedef __m128 vec3;

CONSTEXPR vec3 add(const vec3 a, const vec3 b)
{
	return a+b;
}

CONSTEXPR vec3 sub(const vec3 a, const vec3 b)
{
	return a-b;
}

CONST vec3 hadamard(const vec3 a, const vec3 b)
{
	return a*b;
}

CONST vec3 changesign(const vec3 a)
{
	return a*-1;
}

CONSTEXPR FLOAT dotP(const vec3 a, const vec3 b)
{
	if (SISD)
		return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
	return _mm_cvtss_f32(_mm_dp_ps(a, b, 0b01110001));
}

CONSTEXPR vec3 norm(const vec3 a)
{
	if (SISD) {
		FLOAT d = dotP(a,a);
		FLOAT l = SQRT(d);
		FLOAT x = a[0] / l;
		FLOAT y = a[1] / l;
		FLOAT z = a[2] / l;
		vec3 result = { x, y, z, 0 };
		return result;
	}
	const vec3 dot = _mm_dp_ps(a, a, 0b01110111);
	const vec3 rsqrt = _mm_sqrt_ps(dot);
	return a/rsqrt;
}

CONSTEXPR vec3 normV(const vec3 a, const vec3 b)
{
	return norm(sub(b, a));
}

CONSTEXPR vec3 crossP(const vec3 a, const vec3 b)
{
	if (SISD) {
		FLOAT x = (a[1] * b[2]) - (a[2] * b[1]);
		FLOAT y = (a[2] * b[0]) - (a[0] * b[2]);
		FLOAT z = (a[0] * b[1]) - (a[1] * b[0]);
		vec3 result = { x, y, z, 0 };
		return result;
	}
	// https://threadlocalmutex.com/?p=8
	vec3 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
	vec3 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
	vec3 c = _mm_sub_ps(_mm_mul_ps(a, b_yzx), _mm_mul_ps(a_yzx, b));
	return _mm_shuffle_ps(c, c, _MM_SHUFFLE(3, 0, 2, 1));

}

CONSTEXPR vec3 scalar_mult(const vec3 a, const FLOAT amount)
{
	return a*amount;
}
