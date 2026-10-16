# Assignment v0.4.0: Stamina System Logic + Debug Output

## Overview

Implement the stamina system. The player has stamina that decreases when jumping and regenerates when eating food. When stamina is low, movement and jump height are penalized.

---

## Functional Requirements

### 1. Stamina Variable
- Add `stamina` field to `Player` struct (use `float`).
- Stamina ranges from `0.0f` to `PLAYER_MAX_STAMINA`.
- Player starts with **full stamina** on spawn.

### 2. Jump Mechanics
- Define `JUMP_STAMINA_COST` constant (e.g., 15.0f).
- When player presses SPACE to jump:
  - **If stamina >= JUMP_STAMINA_COST:** Jump normally, stamina -= JUMP_STAMINA_COST.
  - **If stamina < JUMP_STAMINA_COST:** Cannot jump at all.
- *Note: Jumping and eating will be separate from movement key (Space for jump, not included in this assignment).*

### 3. Stamina Regeneration
- Stamina regenerates **only when eating food** (FOOD tile collision).
- Define `STAMINA_FROM_FOOD` constant (e.g., 40.0f).
- When player touches a FOOD tile:
  - `stamina += STAMINA_FROM_FOOD`
  - Cap at `PLAYER_MAX_STAMINA` (do not overflow).

### 4. Movement/Jump Penalties
- When `stamina < JUMP_STAMINA_COST`:
  - **Movement speed reduced by 25%** (multiply velocity.x by 0.75).
  - **Jump height reduced by 50%** (multiply jump velocity by 0.5).
  - **No additional stamina cost** (only costs when jump is successful).

### 5. Debug Output
- Display in top-left corner (using `DrawText`):
  - `Stamina: X.X / 100.0`
  - Example: `Stamina: 42.5 / 100.0`
- Update every frame.
- Use small font (18-20px), white color, positioned at (15, 15).

---

## Non-Functional Requirements

1. **Code Quality**
   - No magic numbers. Use `#define` constants.
   - Functions: `UpdatePlayerStamina()`, `RegenerateStaminaFromFood()`.
   - File header with authorship declaration (syllabus requirement).

2. **Compatibility**
   - Work seamlessly with existing player movement and tilemap.
   - No performance degradation.

3. **Testing**
   - Jump multiple times. Verify stamina decreases.
   - Walk into FOOD tile. Verify stamina increases.
   - Jump when stamina < cost. Verify no jump occurs.
   - Verify penalties apply correctly.

---

## Definition of Done

- [ ] `Player` struct has `stamina` field (float).
- [ ] Constants defined: `PLAYER_MAX_STAMINA`, `JUMP_STAMINA_COST`, `STAMINA_FROM_FOOD`.
- [ ] `UpdatePlayerStamina()` function drains stamina on jump.
- [ ] `RegenerateStaminaFromFood()` function handles food collision.
- [ ] Penalties (movement -25%, jump -50%) apply when stamina low.
- [ ] Debug text visible and accurate every frame.
- [ ] Code compiles without errors or warnings.
- [ ] Tested: Jump drains, food regenerates, penalties work, cannot jump if insufficient stamina.
- [ ] Pull Request created to `alpha/feature/stamina_system/[surname]` or `beta/feature/stamina_system/[surname]`.
- [ ] Assigned teammate as reviewer.
- [ ] Code reviewed and approved.

---

## Branch Naming Convention

```text
alpha/feature/stamina_system/kowalski   (if in Team Alpha)
beta/feature/stamina_system/smith       (if in Team Beta)
```
---

## Technical Hints

- **Stamina as float:** Allows fractional values for smooth calculations.
- **Check stamina before jump:** Use `if (stamina >= JUMP_STAMINA_COST)` before allowing jump.
- **Food tile detection:** Check if `tile_at_player_position == FOOD`. If so, regenerate.
- **Penalty calculation:** `effective_speed = base_speed * 0.75f` when stamina low.
- **Keep updating debug text:** Call `DrawDebugStamina(player)` every frame in main loop.

---

## Deliverables

1. **Branch:** `alpha/feature/stamina_system/[surname]` or `beta/feature/stamina_system/[surname]`
3. **PR Description:** Clearly explain what you implemented and how to test it.
4. **Code Review:** Assigned teammate reviews and approves.

