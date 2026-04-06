#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define screen_width 80
#define screen_height 22

const float theta_spacing = 0.07;
const float phi_spacing = 0.02;

const float R1 = 1;
const float R2 = 2;
const float K2 = 5;

const float K1 = screen_width * K2 * 3 / (8 * (R1 + R2));

int main() {
    float A = 0, B = 0;

    while (1) {
        char output[screen_width * screen_height];
        float zbuffer[screen_width * screen_height];

        memset(output, ' ', screen_width * screen_height);
        memset(zbuffer, 0, screen_width * screen_height * sizeof(float));

        float cosA = cos(A), sinA = sin(A);
        float cosB = cos(B), sinB = sin(B);

        for (float theta = 0; theta < 2 * M_PI; theta += theta_spacing) {
            float costheta = cos(theta), sintheta = sin(theta);

            for (float phi = 0; phi < 2 * M_PI; phi += phi_spacing) {
                float cosphi = cos(phi), sinphi = sin(phi);

                float circlex = R2 + R1 * costheta;
                float circley = R1 * sintheta;

                float x = circlex * (cosB * cosphi + sinA * sinB * sinphi)
                        - circley * cosA * sinB;

                float y = circlex * (sinB * cosphi - sinA * cosB * sinphi)
                        + circley * cosA * cosB;

                float z = K2 + cosA * circlex * sinphi + circley * sinA;
                float ooz = 1 / z;

                int xp = (int)(screen_width / 2 + K1 * ooz * x);
                int yp = (int)(screen_height / 2 - K1 * ooz * y);

                int idx = xp + yp * screen_width;

                float L = cosphi * costheta * sinB
                        - cosA * costheta * sinphi
                        - sinA * sintheta
                        + cosB * (cosA * sintheta - costheta * sinA * sinphi);

                if (L > 0) {
                    if (idx >= 0 && idx < screen_width * screen_height && ooz > zbuffer[idx]) {
                        zbuffer[idx] = ooz;
                        int luminance_index = (int)(L * 8);
                        output[idx] = ".,-~:;=!*#$@"[luminance_index];
                    }
                }
            }
        }

        printf("\x1b[H");

        for (int i = 0; i < screen_width * screen_height; i++) {
            putchar(i % screen_width ? output[i] : '\n');
        }

        A += 0.04;
        B += 0.02;

        usleep(30000);
    }

    return 0;
}