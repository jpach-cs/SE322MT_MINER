#pragma once

/* hud.hpp
 *
 * View: drawing only. May read the Model; never changes it.
 *
 * The stamina bar. Today: drawStaminaBar in main.c. */
#include "../model/config.hpp"

/** Layout of the stamina bar on screen. Defaults are the main.c constants from config.hpp. */
struct StaminaBarLayout {
    float x        = static_cast<float>(STAMINA_BAR_X);
    float y        = static_cast<float>(STAMINA_BAR_Y);
    float width    = static_cast<float>(STAMINA_BAR_W);
    float height   = static_cast<float>(STAMINA_BAR_H);
    int   textSize = STAMINA_BAR_TEXT_SIZE;
    float textGap  = 5.0f;
};

/** Draws the stamina bar and its label. */
class StaminaBar {
public:
    explicit StaminaBar(const StaminaBarLayout &layout = StaminaBarLayout{});

    /** Pixels of the bar to fill for current out of max; pure, so it is unit tested. */
    static int fillWidth(float current, float max, float barWidth);

    /** Draws the bar for current out of max. */
    void draw(float current, float max) const;

    const StaminaBarLayout &layout() const { return layout_; }

private:
    StaminaBarLayout layout_;
};
