/* hud.cpp
 *
 * View: drawing only. May read the Model; never changes it.
 *
 * Implementation of hud.hpp. */
#include "hud.hpp"

#include "raylib.h"

StaminaBar::StaminaBar(const StaminaBarLayout &layout) : layout_(layout) {}

int StaminaBar::fillWidth(float current, float max, float barWidth) {
    if (max <= 0.0f || barWidth <= 0.0f) return 0;

    float fraction = current / max;
    if (fraction < 0.0f) fraction = 0.0f;
    if (fraction > 1.0f) fraction = 1.0f;

    return static_cast<int>(fraction * barWidth);
}

void StaminaBar::draw(float current, float max) const {
    const Rectangle background = {layout_.x, layout_.y, layout_.width, layout_.height};
    const Rectangle fill = {layout_.x, layout_.y,
                            static_cast<float>(fillWidth(current, max, layout_.width)),
                            layout_.height};

    DrawRectangleRec(background, DARKGRAY);
    DrawRectangleRec(fill, BLUE);
    DrawRectangleLinesEx(background, 1.0f, WHITE);

    DrawText(TextFormat("stamina: %.0f / %.0f", current, max),
             static_cast<int>(layout_.x), static_cast<int>(layout_.y + layout_.textGap),
             layout_.textSize, WHITE);
}
