#pragma once
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>
#include "aimbot.h"
#include "game/classes.hpp"
#include "settings.h"
#include "global.h"


//bullet velocity, bullet gravity, bullet drag

std::unordered_map<std::string, std::unordered_map<std::string, std::vector<float>>> weapon_array = {
    {"rifle.ak", {
        {"ammo.rifle", {375.f, 1.f, 0.6f}},
        {"ammo.rifle.hv", {450.f, 1.f, 1.f}},
        {"ammo.rifle.explosive", {225.f, 1.25f, 0.6f}},
        {"ammo.rifle.incendiary", {225.f, 1.f, 0.6f}}
    }},
    {"nmlmg", {
        {"ammo.rifle", {375.f, 1.f, 0.6f}},
        {"ammo.rifle.hv", {450.f, 1.f, 1.f}},
        {"ammo.rifle.explosive", {225.f, 1.25f, 0.6f}},
        {"ammo.rifle.incendiary", {225.f, 1.f, 0.6f}}
    }},
    {"rifle.lr300", {
        {"ammo.rifle", {375.f, 1.f, 0.6f}},
        {"ammo.rifle.hv", {450.f, 1.f, 1.f}},
        {"ammo.rifle.explosive", {225.f, 1.25f, 0.6f}},
        {"ammo.rifle.incendiary", {225.f, 1.f, 0.6f}}
    }},
    {"rifle.bolt", {
        {"ammo.rifle", {656.f, 1.f, 0.6f}},
        {"ammo.rifle.hv", {788.f, 1.f, 1.f}},
        {"ammo.rifle.explosive", {394.f, 1.25f, 1.f}},
        {"ammo.rifle.incendiary", {394.f, 1.f, 0.6f}}
    }},
    {"rifle.l96", {
        {"ammo.rifle", {1125.f, 1.f, 0.6f}},
        {"ammo.rifle.hv", {1350.f, 1.f, 1.f}},
        {"ammo.rifle.explosive", {675.f, 1.25f, 0.6f}},
        {"ammo.rifle.incendiary", {675.f, 1.f, 0.6f}}
    }},
    {"rifle.m39", {
        {"ammo.rifle", {469.f, 1.f, 0.6f}},
        {"ammo.rifle.hv", {563.f, 1.f, 1.f}},
        {"ammo.rifle.explosive", {281.f, 1.25f, 0.6f}},
        {"ammo.rifle.incendiary", {281.f, 1.f, 0.6f}}
    }},
    {"rifle.semiauto", {
        {"ammo.rifle", {375.f, 1.f, 0.6f}},
        {"ammo.rifle.hv", {450.f, 1.f, 1.f}},
        {"ammo.rifle.explosive", {225.f, 1.25f, 0.6f}},
        {"ammo.rifle.incendiary", {225.f, 1.f, 0.6f}}
    }},
    {"lmg.m249", {
        {"ammo.rifle", {487.5f, 1.f, 0.6f}},
        {"ammo.rifle.hv", {585.f, 1.f, 1.f}},
        {"ammo.rifle.explosive", {293.f, 1.25f, 0.6f}},
        {"ammo.rifle.incendiary", {293.f, 1.f, 0.6f}}
    }},
    {"smg.mp5", {
        {"ammo.pistol", {247.5f, 1.f, 1.f}},
        {"ammo.pistol.hv", {320.f, 1.f, 1.f}},
        {"ammo.pistol.incendiary", {180.f, 1.f, 0.7f}}
    }},
    {"smg.2", {
        {"ammo.pistol", {247.5f, 1.f, 1.f}},
        {"ammo.pistol.hv", {320.f, 1.f, 1.f}},
        {"ammo.pistol.incendiary", {180.f, 1.f, 0.7f}}
    }},
    {"pistol.m92", {
        {"ammo.pistol", {300.f, 2.2f, 1.f}},
        {"ammo.pistol.hv", {400.f, 2.2f, 1.f}},
        {"ammo.pistol.incendiary", {225.f, 1.f, 0.7f}}
    }},
    {"pistol.python", {
        {"ammo.pistol", {300.f, 1.f, 1.f}},
        {"ammo.pistol.hv", {400.f, 1.f, 1.f}},
        {"ammo.pistol.incendiary", {225.f, 1.f, 0.7f}}
    }},
    {"pistol.revolver", {
        {"ammo.pistol", {300.f, 1.f, 1.f}},
        {"ammo.pistol.hv", {400.f, 1.f, 1.f}},
        {"ammo.pistol.incendiary", {225.f, 1.f, 0.7f}}
    }},
    {"pistol.semiauto", {
        {"ammo.pistol", {300.f, 1.f, 1.f}},
        {"ammo.pistol.hv", {400.f, 1.f, 1.f}},
        {"ammo.pistol.incendiary", {225.f, 1.f, 0.7f}}
    }},
    {"smg.thompson", {
        {"ammo.pistol", {300.f, 1.f, 1.f}},
        {"ammo.pistol.hv", {400.f, 2.2f, 1.f}},
        {"ammo.pistol.incendiary", {225.f, 1.f, 0.7f}}
    }},
    {"pistol.prototype17", {
        {"ammo.pistol", {300.f, 2.2f, 1.f}},
        {"ammo.pistol.hv", {400.f, 2.2f, 1.f}},
        {"ammo.pistol.incendiary", {225.f, 1.f, 0.7f}}
    }},
    {"bow.hunting", {
        {"arrow.hv", {50.f, 1.19f, 0.005f}}, //gravity old = 1.19
        {"arrow.bone", {50.f, 1.19f, 0.01f}}, //gravity old = 1.19
        {"arrow.wooden", {50.f, 1.19f, 0.005f}}, //gravity old = 1.19
        {"arrow.fire", {50.f, 1.f, 0.01f}} //gravity old = 1.19
    }},
    {"bow.compound", {
        {"arrow.hv", {100.f, 0.5f, 0.005f}},
        {"arrow.bone", {100.f, 1.19f, 0.01f}},
        {"arrow.wooden", {100.f, 1.19f, 0.005f}},
        {"arrow.fire", {100.f, 1.f, 0.01f}},
    }},
    {"crossbow", {
        {"arrow.hv", {75.f, 0.5f, 0.005f}},
        {"arrow.bone", {75.f, 1.19f, 0.01f}},
        {"arrow.wooden", {75.f, 1.19f, 0.005f}}, //{"arrow.wooden", {55.f, 0.75f, 6.6f}},
        {"arrow.fire", {75.f, 1.f, 0.01f}}
    }},
    {"pistol.nailgun", {
        {"ammo.nailgun.nails", {50.f, 0.75f, 0.005f}},
    }},
};

Vector2 Lerp(const Vector2& start, const Vector2& end, float t) {
    return start + (end - start) * t;
}

namespace aimbot {
    float dynamic_fov = std::get<float>(ui::element["aimbot_fovCount"].value);
    Vector3 aim_pos = Vector3{1,1,1};
    std::clock_t pred_last = global::getCurrentTimeMillis();

    FovResult InFov(float fov, bool ignore) {
        FovResult result;
        result.last_distance = 6516;
        result.saved_player = nullptr;
        result.head_pos;
        result.in_fov = false;

        for (int i = 0; i < variables::players.size(); i++) {
            auto player = variables::players[i];
            bool should_continue = true;
            if (!std::get<bool>(ui::element["player_npc"].value) && player.npc == true || ignore && player.npc == true) {
                should_continue = false;
            }
            if (!std::get<bool>(ui::element["player_sleep"].value) && player.sleeping == true) {
                should_continue = false;
            }
            if (player.teammate == true && std::get<bool>(ui::element["aimbot_teamcheck"].value) || player.teammate == true && ignore) {
                should_continue = false;
            }
            if (player.bot == true && ignore) {
                should_continue = false;
            }
            if (should_continue) {
                Vector2 head2D;
                Vector3 hitboxPos = ((rust::BasePlayer*)player.entity)->get_bone_position(47);
                if (functions::WorldToScreen(hitboxPos, head2D)) {
                    double distanceToCenter = Math::Calc2D_Dist(Vector2(variables::screen_width / 2, variables::screen_height / 2), head2D);
                    if (result.last_distance > distanceToCenter) {
                        result.last_distance = distanceToCenter;
                        result.saved_player = ((rust::BasePlayer*)player.entity);
                        result.head_pos = hitboxPos;
                    }
                    if (!ignore) {
                        if (std::get<bool>(ui::element["aimbot_fovdynamic"].value)) {
                            auto f_distance = Math::Calc3D_Dist(variables::local_position, hitboxPos);
                            float max_range = std::get<float>(ui::element["aimbot_fovdynamicrange"].value);

                            if (max_range > f_distance) {
                                float dynamic_count = std::get<float>(ui::element["aimbot_fovdynamicCount"].value);
                                float scaled_multiplier = (max_range - f_distance) / max_range;
                                dynamic_count *= scaled_multiplier / 100.0f;  // Scale down from 0 to 100 to 0 to 1
                                fov = fov + (fov * dynamic_count);
                            }
                        }
                        dynamic_fov = fov;
                    }

                    if (distanceToCenter <= fov) {
                        result.in_fov = true;
                    }
                }
            }
        }

        return result; // Return the struct with the results
    }

    double ToRad(double degree)
    {
        double pi = 3.14159265359;
        return (degree * (pi / 180));
    }
    float CAngle(float a, float b) {
        float diff = std::fmod(b - a + 180.0f, 360.0f) - 180.0f;
        return (diff < -180.0f) ? diff + 360.0f : diff;
    }

    // Function to linearly interpolate angles while considering angle wrapping.
    float LerpAngle(float a, float b, float t) {
        float delta = b - a;
        if (delta > 180.0f)
            b -= 360.0f;
        else if (delta < -180.0f)
            b += 360.0f;
        return a + t * (b - a);
    }

    Vector2 SmoothInterpolate(const Vector2& startAngles, const Vector2& targetAngles, float smoothing) {
        smoothing = std::fmaxf(0.0f, std::fminf(smoothing, 100.0f));
        float t = smoothing / 100.0f;

        float angleDifferenceX = CAngle(startAngles.x, targetAngles.x);
        float angleDifferenceY = CAngle(startAngles.y, targetAngles.y);

        Vector2 interpolatedAngles;
        interpolatedAngles.x = LerpAngle(startAngles.x, startAngles.x + angleDifferenceX, t);
        interpolatedAngles.y = LerpAngle(startAngles.y, startAngles.y + angleDifferenceY, t);

        return interpolatedAngles;
    }

    Vector3 Prediction(rust::BasePlayer* Player, Vector3 bone_pos)
    {
        auto weapon = variables::local_player->get_held_item()->get_held_entity();
        float bulletvelocity = 200.f;
        float bulletgravity = 1.f;

        if (!variables::local_player->get_held_item()->is_gun())
            return bone_pos;

        rust::BaseProjectile* base_projectile = reinterpret_cast<rust::BaseProjectile*>(weapon);
        if (!base_projectile)
            return bone_pos;

        rust::BaseProjectile::Magazine* magazine = base_projectile->get_primaryMagazine();
        if (!magazine)
            return bone_pos;

        rust::ItemDefinition* ammo_type_definition = magazine->get_ammotype();
        if (!ammo_type_definition)
            return bone_pos;

        std::wstring ammo_name = ammo_type_definition->get_shortname();

        if (weapon_array.find(variables::lcp.weapon_name) != weapon_array.end()) {
            if (weapon_array[variables::lcp.weapon_name].find(functions::ws2s(ammo_name)) != weapon_array[variables::lcp.weapon_name].end()) {
                bulletvelocity = weapon_array[variables::lcp.weapon_name][functions::ws2s(ammo_name)][0];
                bulletgravity = weapon_array[variables::lcp.weapon_name][functions::ws2s(ammo_name)][1];
            }
        }

        float Dist = Math::Calc3D_Dist(variables::local_position, bone_pos);

        float BulletTime = Dist / (bulletvelocity * bulletgravity);
        bone_pos.y += (4.905f * BulletTime * BulletTime);

        Vector3 velocity = Player->get_model()->get_velocity();
        bone_pos.x += (velocity.x * BulletTime);
        bone_pos.y += (velocity.y * BulletTime);

        aim_pos = bone_pos;
        pred_last = global::getCurrentTimeMillis();
        return bone_pos;
    }

    float CalculateAngleDifference(const Vector2& currentAngles, const Vector2& targetAngles) {
        float yawDifference = targetAngles.x - currentAngles.x;
        float pitchDifference = targetAngles.y - currentAngles.y;

        // Ensure the angle differences are within the range of -180 to 180 degrees
        while (yawDifference > 180.0f) {
            yawDifference -= 360.0f;
        }

        while (yawDifference < -180.0f) {
            yawDifference += 360.0f;
        }

        while (pitchDifference > 180.0f) {
            pitchDifference -= 360.0f;
        }

        while (pitchDifference < -180.0f) {
            pitchDifference += 360.0f;
        }

        return std::sqrt(yawDifference * yawDifference + pitchDifference * pitchDifference);
    }

    Vector4 silentPosition(Vector3 Euler) {

        double heading = ToRad(Euler.x);
        double attitude = ToRad(Euler.y);
        double bank = ToRad(Euler.z);

        double c1 = cos(heading / 2);
        double s1 = sin(heading / 2);
        double c2 = cos(attitude / 2);
        double s2 = sin(attitude / 2);
        double c3 = cos(bank / 2);
        double s3 = sin(bank / 2);
        double c1c2 = c1 * c2;
        double s1s2 = s1 * s2;
        Vector4 Quat;
        Quat.w = c1c2 * c3 - s1s2 * s3;
        Quat.x = c1c2 * s3 + s1s2 * c3;
        Quat.y = s1 * c2 * c3 + c1 * s2 * s3;
        Quat.z = c1 * s2 * c3 - s1 * c2 * s3;
        Vector4 yourmom;
        yourmom = { Quat.y, Quat.z, (Quat.x * -1), Quat.w };
        return yourmom;
    }

    void run(bool silent) {
        try {
            float smoothing = (1000 - std::get<float>(ui::element["aimbot_smoothingCount"].value)) / 100;

            FovResult result = InFov(std::get<float>(ui::element["aimbot_fovCount"].value), false);
            double last_distance = result.last_distance;
            rust::BasePlayer* player = result.saved_player;
            bool isInFov = result.in_fov;

            if (last_distance != 6516 && (isInFov && std::get<bool>(ui::element["aimbot_fov"].value) || !std::get<bool>(ui::element["aimbot_fov"].value))) {
                if (player == variables::local_player)
                    return;

                BoneList bone = BoneList::head;
                if (std::get<int>(ui::element["aimbot_hitbox"].value) == 1) {
                    bone = BoneList::spine3;
                }
                else if (std::get<int>(ui::element["aimbot_hitbox"].value) == 2) {
                    bone = BoneList::pelvis;
                }
                else {
                    //int closest_bone = getClosestBoneToCrosshair(player);
                    //if (closest_bone != 0) {
                    //    bone = static_cast<BoneList>(closest_bone);
                    //}
                    //crash
                }

                if (player->is_knocked())
                    return;
                if (player->is_sleeping())
                    return;

                Vector3 enemylock;

                if (std::get<bool>(ui::element["aimbot_prediction"].value)) {
                    enemylock = Prediction(player, player->get_bone_position(bone));
                }
                else {
                    enemylock = player->get_bone_position(bone);
                }

                bool is_visible = true;

                if (std::get<bool>(ui::element["aimbot_vischeck"].value)) {
                    if (variables::local_player->is_visible(player->get_bone_position(bone))) {
                        is_visible = true;
                    }
                    else {
                        is_visible = false;
                    }
                }
                if (is_visible) {
                    if (silent) {
                        Vector2 AngleToAim = Math::CalcAngle(variables::local_position, enemylock);
                        Math::Normalize(AngleToAim.y, AngleToAim.x);

                        if (!isnan(AngleToAim.x) && !isnan(AngleToAim.y)) {
                            Vector4 QuatAngles = silentPosition({ AngleToAim.x, AngleToAim.y, 0.0f });
                            variables::local_player->get_eyes()->set_body_rotation(QuatAngles);
                        }
                    }
                    else {
                        Vector2 currentAngles = variables::local_player->get_input()->get_view_angles();
                        Vector2 targetAngles = Math::CalcAngle(variables::local_position, enemylock); //variables::local_player->get_bone_position(BoneList::neck)

                        if (std::get<bool>(ui::element["aimbot_smoothing"].value)) {
                            Vector2 smoothedAngles = SmoothInterpolate(currentAngles, targetAngles, smoothing);
                            float angleDifference = CalculateAngleDifference(currentAngles, smoothedAngles);

                            if (std::abs(angleDifference) < 6.0f) {
                                variables::local_player->get_input()->set_view_angles(smoothedAngles);
                            }
                        }
                        else {
                            Math::Normalize(targetAngles.y, targetAngles.x);
                            variables::local_player->get_input()->set_view_angles(targetAngles);
                        }
                    }
                }
            }
            else {
                //std::cout << "CANT RUN AIMBOT." << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cout << "aimbot error." << std::endl;
        }
    }

}