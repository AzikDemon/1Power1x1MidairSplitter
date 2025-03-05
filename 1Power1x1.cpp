#include <iostream>
#include <cmath>
#include <format>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

#define drag 0.98f
#define gravity (-0.04f)

#define print(x) cout << x << endl;
#define sq(x) pow(x, 2)
#define gua(a, b, c) (float) sqrt(sq(a) + sq(b) + sq(c))

/*
        1 Power 1x1 Midair Splitter
        - Azik
*/

struct Entity {
    double x;
    double y;
    double z;
};

Entity calculate(
    const double float_velo, const double splitter_exp, const double projectile_exp, const double sand_exp,
    const int min_power, const int max_power, const int max_splitter, const int ticks) {

    Entity splitter_velo{};
    Entity projectile_velo{};
    Entity sand_velo{};

    Entity projectile_initial_pos{};
    Entity sand_initial_pos{};

    // only care about z, dx = 0, y initial = 254.02f
    // change dy accordingly to how many ticks out.

    ofstream outputFile("RatiosLadder.txt");

    for (int k = min_power; k < max_power + 1; k++) {

        splitter_velo.z = splitter_exp * k;

        projectile_velo.z = projectile_exp * k;
        sand_velo.z = sand_exp * k;

        double midair_proj_dx = 0;
        double midair_proj_dy = -gravity;
        double midair_proj_dz = splitter_velo.z - projectile_velo.z;

        double midair_proj_dist = gua(midair_proj_dx, midair_proj_dy, midair_proj_dz);
        double midair_proj_exp = (1 - (midair_proj_dist / 8)) / midair_proj_dist;
        if (midair_proj_dist <= 0 || midair_proj_dist >= 8)
            midair_proj_exp = 0;

        double midair_sand_dx = 0;
        double midair_sand_dy = -gravity;
        double midair_sand_dz = splitter_velo.z - sand_velo.z;

        double midair_sand_dist = gua(midair_sand_dx, midair_sand_dy, midair_sand_dz);
        double midair_sand_exp = (1 - (midair_sand_dist / 8)) / midair_sand_dist;
        if (midair_sand_dist <= 0 || midair_sand_dist >= 8)
            midair_sand_exp = 0;

        Entity initial_velo{};

        initial_velo.z = projectile_velo.z * drag;

        for (int i = 1; i < max_splitter + 1; i++) {
            projectile_initial_pos.y = 254.02f;
            projectile_velo.y = midair_proj_exp * midair_proj_dy * i;

            projectile_velo.z = initial_velo.z;
            projectile_initial_pos.z = projectile_velo.z;

            projectile_velo.z -= midair_proj_exp * midair_proj_dz * i;

            sand_initial_pos.y = 254.02f;
            sand_velo.y = midair_sand_exp * midair_sand_dy * i;

            sand_velo.z = initial_velo.z;
            sand_initial_pos.z = sand_velo.z;

            sand_velo.z -= midair_sand_exp * midair_sand_dz * i;

            for (int j = 0; j < ticks + 1; j++) {
                projectile_velo.y += gravity;
                projectile_initial_pos.y += projectile_velo.y;

                sand_velo.y += gravity;
                sand_initial_pos.y += sand_velo.y;

                if (projectile_initial_pos.y == 255.0) print("255.0");
                if (sand_initial_pos.y == 255.0) print("255.0");

                if (projectile_initial_pos.y > 255.0 or projectile_initial_pos.y < 254.0)  break;
                if (sand_initial_pos.y > 255.0 or sand_initial_pos.y < 254.0)  break;

                if (254.02f > projectile_initial_pos.y && projectile_initial_pos.y > 254.0 &&
                    254.02f > sand_initial_pos.y && sand_initial_pos.y > 254.0) {
                    if (abs(sand_initial_pos.z + sand_velo.z) > 4720) {
                        if (abs(sand_initial_pos.z) < 4480) {

                            outputFile << format(
                                "Power: {} Splitter: {} Tick: {}\n",
                                k, i, j);
                            outputFile << format(
                "Between: {} - {} Distance: {} Float: {}\n",
                abs(sand_initial_pos.z),
                abs(sand_initial_pos.z + sand_velo.z), abs(sand_velo.z), abs(float_velo * k)
            );
                            outputFile << format(
                                "-----------------------------------------------------------------------------------\n");
                        }
                    }
                }

                projectile_initial_pos.z += projectile_velo.z;
                projectile_velo.y *= drag;
                projectile_velo.z *= drag;

                sand_initial_pos.z += sand_velo.z;
                sand_velo.y *= drag;
                sand_velo.z *= drag;
            }
        }
    }

    outputFile.close();

    return splitter_velo;
}

int main() {
    cout << setprecision(16);

    /*
        For following velocity values, input position in (barrel - position of tick of impact) per tnt.
        In this case, the first gametick of projectile and sand, and second gametick of splitter.
        Use position, not velocity for both, since cannon debug velocity and position are different.
        Below are preset values for my personal setup.
        Float doesn't matter at all.
        If you want accurate values for how far it floats in your cannon, look at which of the 3 projectiles floats the least and input that.
    */

    double projectile_z_velo = -0.7411661687708147;
    double splitter_z_velo = -0.7332239059142921; // ladder
    // double splitter_z_velo = -0.7461639068575823; // trapdoor
    double sand_z_velo = -0.7411678680587829;

    double float_velo = -0.18158571488300124;

    constexpr int min_power = 0;
    constexpr int max_power = 1000;
    constexpr int max_splitter = 200;
    constexpr int max_ticks = 13;

    calculate(float_velo, splitter_z_velo, projectile_z_velo, sand_z_velo, min_power, max_power, max_splitter, max_ticks);

}
