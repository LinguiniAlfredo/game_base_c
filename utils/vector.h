#pragma once

typedef struct {
    float x;
    float y;
} Vector2f;

typedef struct {
    int x;
    int y;
} Vector2i;

Vector2f vector_create_zero()
{
    Vector2f vector;
    vector.x = 0;
    vector.y = 0;
    return vector;
}

Vector2f vector_create(const float x, const float y)
{
    Vector2f vector;
    vector.x = x;
    vector.y = y;
    return vector;
}

Vector2i vector_ftoi(Vector2f float_vector)
{
    Vector2i int_vector;
    int_vector.x = (int)float_vector.x;
    int_vector.y = (int)float_vector.y;
    return int_vector;
}

float vector_length(const Vector2f vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

Vector2f vector_normalize(const Vector2f vector)
{
    Vector2f normalized = vector_create_zero();
    if (vector_length(vector) != 0) {
        normalized.x = vector.x / vector_length(vector);
        normalized.y = vector.y / vector_length(vector);
        return normalized;
    }
    return vector; // should we return null here instead
}

float vector_dot(const Vector2f a, const Vector2f b)
{
    return a.x * b.x + a.y * b.y;
}

float vector_cross(const Vector2f a, const Vector2f b)
{
    return a.x * b.y - a.y * b.x;
}

float vector_angle(const Vector2f a, const Vector2f b)
{
    Vector2f normalized_a = vector_normalize(a);
    Vector2f normalized_b = vector_normalize(b);

    return acos(vector_dot(normalized_a, normalized_b));
}
