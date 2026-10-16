# Assignment: Code Review in Pairs (Two Steps)

## Goal

Practice creating a Pull Request and conducting a code review as a peer reviewer. You will play both roles: **Author** and **Reviewer**.

---

## Step 1: As AUTHOR — Create a Pull Request to Your Team's Branch

1.  **Identify your team:**
    - Go to **Canvas** → **People**.
    - Find your team: **Team Alpha** or **Team Beta**.

2.  **Create a Pull Request:**
    - Go to GitHub, repository `SE322MT_MINER`.
    - Click **"Pull requests"** → **"New pull request"**.
    - Set the branches:
        - `base:` **`feature/alpha/tilemap`** (if you are in Alpha)
        - `base:` **`feature/beta/tilemap`** (if you are in Beta)
        - `compare:` Your personal branch `feature/[surname]-tilemap`

3.  **Fill in the PR description:**
    - **Title:** Describe what you did (e.g., `Refactor: Extract magic numbers to constants`)
    - **Description:** Use this template:
        ```
        ### What was done?
        - [list of changes]
        
        ### How to test?
        1. [testing instructions]
        
        ### Known limitations
        - [limitations, if any]
        ```

4.  **Assign a reviewer:**
    - Go to Canvas → People → Your team.
    - Sort all members **alphabetically by surname**.
    - Find your position on the list.
    - **The person AFTER you alphabetically** is your reviewer.
    - **If you are last on the list,** the reviewer is the **first person** on the list.
    - Assign them in the "Reviewers" section on GitHub.

5.  **Click "Create pull request".**

Congratulations! Now you wait for your review.

---

## Step 2: As REVIEWER — Conduct a Code Review

1.  **Wait for notification:**
    - You will receive an email and a GitHub notification that you have been assigned to review someone's PR.

2.  **Enter the Pull Request:**
    - Go to the **"Files changed"** tab.

3.  **Start a review ("Start a review"):**
    - Leave **at least 3 valuable comments**:
        - 🟡 One `should` comment (something that could be better)
        - 🔵 One `nit` comment (something stylistic)
        - ✨ One positive comment (something done well)
    - **Important:** Use **"Start a review"** so your comments are "pending" (not published immediately).

4.  **Summarize your review:**
    - Click **"Review changes"** button (top right).
    - Write a general comment (e.g., "Good work! A few suggestions below.").

5.  **Issue your verdict:**
    - ✅ **Approve** — If the code is good and ready to merge.
    - ❌ **Request changes** — If you found something that must be fixed.

6.  **Click "Submit review".**

**IMPORTANT: Do NOT merge the PR! All changes will be merged during the next lecture, on the projector.**
