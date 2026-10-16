# Assignment v0.4.1: Stamina Bar Visualization

## Overview

Build upon v0.4.0 by replacing debug text with a visual stamina bar. The bar will be blue and positioned on the HUD.

---

## Functional Requirements

### 1. Stamina Bar Rectangle
- Draw a **blue rectangle bar** on the HUD (top-left area, below any other HUD elements).
- Bar position: (15, 35) or wherever suits your HUD layout.
- Bar dimensions:
  - Width: 100 pixels (or configurable via constant).
  - Height: 15 pixels.

### 2. Bar Fill (Dynamic)
- The bar's **fill width** represents current stamina percentage.
- Formula: `fill_width = (current_stamina / PLAYER_MAX_STAMINA) * BAR_WIDTH`
- Fill color: **Blue** (e.g., `BLUE` in raylib or custom blue).
- Background (empty part) color: **Dark gray** (e.g., `DARKGRAY`).

### 3. Bar Border
- Draw a **1px white border** around the entire bar for definition.

### 4. Stamina Text
- Display text next to or below the bar:
  - Format: `Stamina: X.X / 100.0`
  - Position: (15, 55) or next to bar.
  - Font size: 18-20px.
  - Color: White.

### 5. Keep Debug Text (Optional)
- You may remove the debug text from v0.4.0 if the bar is clear enough.
- Or keep both for verification purposes during development.

---

## Non-Functional Requirements

1. **Code Quality**
   - Extract bar drawing into dedicated function: `DrawStaminaBar(const Player *player)`.
   - No magic numbers. Use `#define` constants: `STAMINA_BAR_X`, `STAMINA_BAR_Y`, `STAMINA_BAR_WIDTH`, `STAMINA_BAR_HEIGHT`.
   - File header with authorship declaration.

2. **Performance**
   - Bar redraw should not impact framerate (simple rectangle drawing).

3. **Compatibility**
   - v0.4.1 builds on top of v0.4.0. All v0.4.0 logic remains unchanged.
   - If working with teammates, ensure your branch is based on the approved v0.4.0.

---

## Definition of Done

- [ ] `DrawStaminaBar()` function created.
- [ ] Blue rectangle bar renders correctly.
- [ ] Bar fill width updates dynamically with stamina changes.
- [ ] White border visible.
- [ ] Stamina text next to or below bar, accurate.
- [ ] Constants defined for bar position and dimensions.
- [ ] Code compiles without errors or warnings.
- [ ] Tested: Bar depletes on jump, fills on food pickup.
- [ ] Pull Request created to `alpha/feature/stamina_bar/[surname]` or `beta/feature/stamina_bar/[surname]`.
- [ ] Assigned teammate as reviewer (ideally same person who reviewed v0.4.0).
- [ ] Code reviewed and approved.

---

## Branch Naming Convention

```text
alpha/feature/stamina_bar/kowalski   (if in Team Alpha)
beta/feature/stamina_bar/smith       (if in Team Beta)
```

**Important:** This branch should be **based on the approved v0.4.0 branch** from the same team member or merged v0.4.0 from the team branch.

---

## Technical Hints

- **Stamina percentage:** `percentage = stamina / PLAYER_MAX_STAMINA`
- **Dynamic width:** `fill_width = percentage * STAMINA_BAR_WIDTH`
- **raylib functions:**
  - `DrawRectangle(x, y, width, height, color)` - filled rectangle
  - `DrawRectangleLines(x, y, width, height, color)` - outline only
  - `DrawText(text, x, y, fontSize, color)` - text drawing
- **Color constants in raylib:** `BLUE`, `DARKGRAY`, `WHITE`, etc.

---

## Deliverables

1. **Branch:** `alpha/feature/stamina_bar/[surname]` or `beta/feature/stamina_bar/[surname]`
3. **PR Description:** Explain the visual design and how it represents stamina.
4. **Code Review:** Assigned teammate reviews and approves.
