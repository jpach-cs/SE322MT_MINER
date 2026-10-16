# SE322 – Software Engineering
# Montana Tech – Fall 2026
# Instructor: Dr. Jakub Pach

---

# Project Introduction and Team Assignment

## A Message From Your Instructor

Welcome to the active phase of SE322.

Over the past several weeks we have covered the foundational tools
of modern software development – Git, GitHub, Markdown, and the
basics of working with an existing codebase. You are now ready to
apply those skills in a context that reflects how software is actually
built in the industry.

Before we begin I want to be transparent with you about several
decisions I have made and the reasoning behind them.

---

## On Team Composition

I have assigned each of you to one of two teams.

This was not random. It was not based on personal preference or
prior relationships. It was based on my observation of your skills,
your communication style, and your ability to work under uncertainty
across multiple courses.

I want to explain why this matters.

In a professional software organization, teams are not assembled
randomly. Every person on a team earned their position through a
competitive hiring process – interviews, technical assessments, and
a track record of demonstrated work. Senior engineers are paired
with junior engineers deliberately, because mentorship is considered
part of the senior's professional responsibility, not an optional
courtesy.

The closest analogy I can offer is two captains picking sides for
a football match on a playground. The strongest players are chosen
first. The less experienced players are chosen last. No one pretends
otherwise. What matters is that once the teams are set, everyone
plays together.

I have built your teams with the same logic. The composition of
each team is intentional. You will understand the reasoning more
clearly as the semester progresses.

---

## On Individual Accountability Within a Team

I was a student once. I know exactly how team assignments work in
practice. Someone writes the code. Someone reviews it at the last
minute. Someone buys pizza and calls it a contribution. Everyone
receives the same grade.

I will not allow that here.

This course has two goals that exist in tension with each other,
and I want to name that tension directly rather than pretend it
does not exist.

The first goal is to give you an authentic experience of
collaborative software development – the kind that happens in
real organizations, with real coordination problems, real
disagreements, and real shared ownership of a codebase.

The second goal is to verify that each of you individually has
acquired the competencies described in the course learning outcomes.
A team grade tells me nothing about what you personally can do.

To satisfy both goals, every significant piece of work in this
course will be structured so that your individual contribution
is visible, traceable, and assessable. Git does not lie.
Every commit has an author. Every Pull Request has a history.
Every code review comment has a name attached to it.

You will be assessed on what you personally wrote, reviewed,
and documented – not on what your team produced as a whole.

---

## Robert Jackson – Senior Advisor

Robert Jackson has agreed to serve in an advisory role for
both teams this semester, and I want to say plainly that
I am glad he said yes.

Robert brings eleven years of professional software development
experience to this course. He has seen codebases in far worse
shape than this one, shipped real products under real deadlines,
and made the kind of decisions that only become obvious in
hindsight. That experience is not something a textbook can
replicate, and having it in the room with us is genuinely
valuable.

His role this semester is not to write your code or solve your
problems. It is to share the perspective of someone who has
lived the consequences of the decisions you are about to practice
making. He will offer that perspective when it is useful and
hold back when the struggle itself is the lesson.

Treat his input as you would treat advice from a trusted
senior colleague. You are not obligated to follow it blindly,
but you should take it seriously and understand it before
you decide to go a different way.

---

## The Project

You have already seen the repository:

```
https://github.com/jpach-cs/SE322MT_MINER
```

You have read the `readme.txt` left behind by the developer
who was assigned to this project before you. You know the
situation. A junior developer was given a specification,
made meaningful progress, and then left before the work
was finished. The codebase is yours now.

Your senior advisor has reviewed the code. Based on that review,
the first directive from project leadership is the following:

---

## Instructor's Assessment

*The following is a summary of the current codebase prepared
by Dr. Pach prior to the start of the assignment.*

The existing implementation has a working movement and physics
system. The player character moves, jumps, and collides with
a floor. The sprite animation cycles correctly for the available
frames. Error handling for missing assets is present.

However, the project cannot move forward in its current state
for the following reasons.

The codebase uses a fixed 800×450 resolution with no separation
between display resolution and game logic resolution. This will
create compounding problems as the map system is introduced.
All game-world measurements need to be expressed in terms of
a consistent tile unit before any map or collision work begins.

The magic numbers scattered through the code – `800`, `450`,
`5`, `0.5f`, `-12.0f` – make the relationships between values
invisible. Change one and you cannot predict what breaks.
Every constant needs a name before anyone touches the physics
or the map.

The sprite loading and animation system is functional but it
introduces a dependency on an external asset file that is not
currently in a stable state. Before the map system is built,
the player representation should be decoupled from the sprite
so that movement, collision, and map logic can be verified
independently of asset quality.

These are not cosmetic concerns. They are prerequisites.
Nothing else on the backlog should be started until these
three issues are resolved.

---

## Your First Assignment

You are joining this project as a junior developer.
You are working individually for now – team coordination
comes later. Your first responsibility is to understand
what exists and to make the changes that unblock everything else.

**Before you write a single line of code, read every line
of the existing `main.c` carefully.**

This code is not highly complex, but it likely contains
patterns you have not worked with before. A simplified
physics simulation – gravity, jumping, velocity – is
present in the loop. The rendering system draws to the
screen sixty times per second. The sprite is controlled
through a rectangle that changes dimensions to achieve
a mirror effect.

Do not guess at what the code does. Understand it.
Then read the assignment instructions.

The tutorials accompanying the assignments explain not only
what to do but why each decision exists. Read them before
you begin. They were written specifically to address the
questions that the code raises.

Your assignments are in the files:

```
docs/assignment-01.md
docs/assignment-02.md
```

Read Assignment 01 completely before starting.
Complete Assignment 01 before opening Assignment 02.

---

## A Final Note

This project is small. The problems it presents are solvable.
The technology involved is accessible.

What is not small is the habit of mind this course is trying
to build – the habit of reading before writing, of understanding
before modifying, of making your work visible and defensible
to the people who depend on it.

That habit is what separates a developer who can be trusted
with a production codebase from one who cannot.

You have twelve weeks to practice it.

Dr. Jakub Pach
