#include "core.h"

float lerpAngle(float a, float b, float t) {
    float diff = b - a;
    // Si la différence est proche de 2pi (ou -2pi), on effectue une interpolation linéaire sur 2pi
    if (fabsf(diff - 2.0f * M_PI) < EPSILON || fabsf(diff + 2.0f * M_PI) < EPSILON) {
        return a + 2.0f * M_PI * t;
    }
    float delta = fmodf(diff + M_PI, 2.0f * M_PI) - M_PI;
    return a + delta * t;
}

float smoothStepf(float t) {
    // Original smooth step: t * t * (3 - 2 * t)
    // return t * t * (3.0f - 2.0f * t);

    if (t < 0.4f) {
        // Acceleration rapide au debut
        return 0.8f * (t / 0.4f) * (t / 0.4f);
    } else {
        // Plus lent vers la fin
        float normalized = (t - 0.4f) / 0.6f;
        return 0.8f + 0.2f * (1.0f - powf(1.0f - normalized, 2.0f));
    }
}
