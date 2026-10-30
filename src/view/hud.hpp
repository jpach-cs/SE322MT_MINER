#pragma once

/** Layout of the stamina bar on screen. Defaults match the old main.c constants. */
struct StaminaBarLayout {
    float x        = 15.0f;   
    float y        = 35.0f;   
    float width    = 100.0f;  
    float height   = 15.0f;   
    int   textSize = 18;    
    float textGap  = 20.0f; 
};

/** Draws the stamina bar and its label. */
class StaminaBar {
public:
    explicit StaminaBar(const StaminaBarLayout &layout = StaminaBarLayout{});

   
    static int fillWidth(float current, float max, float barWidth);

    /** Draws the bar for current out of max. */
    void draw(float current, float max) const;

    const StaminaBarLayout &layout() const { return layout_; }

private:
    StaminaBarLayout layout_;
};
