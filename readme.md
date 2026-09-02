This project uses [raylib](https://www.raylib.com) 
licensed under the zlib license.

---

# SE322 – Montana Tech Miner

## Background

You have just joined a small development team.
Before you arrived, a junior developer was assigned to this project.
He had dropped out of his CS program halfway through and was hired
to get things moving. He made some progress, then quit.

Nobody has heard from him since.

A colleague managed to recover his working directory before his
laptop was wiped and did you a favor – copied the files into a
repository and pushed it to GitHub. That repository is this one.

The only documentation you have is `notes.txt`, a plain text file
found in the root of the directory. It appears to be personal notes
the previous developer kept for himself. It was not written for you.

Your employer wants the project finished.
You have been assigned a team to work with.
None of you have been here from the beginning.
None of you wrote this code.

That is the job.

---

## What You Are Looking At

A partially implemented game built in C using raylib.
The original requirements are roughly described in `notes.txt`.
The code is in `main.c`.
It compiles and runs.
Beyond that, you are on your own.

Read `notes.txt` first.
Then read the code.
Then talk to your team.

---

## Your Task

Your employer has not given you a detailed specification.
The expectation is that your team will:

- understand what was built and why
- identify what is missing, broken, or unclear
- agree on what needs to be done
- divide the work
- finish it

How you organize yourselves is up to you.

---

## Requirements

- [w64devkit](https://github.com/skeeto/w64devkit/releases)
- [VSCode](https://code.visualstudio.com/) + C/C++ extension

## First Run

1. Fork this repo
2. Clone your fork
3. Open the folder in VSCode
4. Check the path in `.vscode/settings.json`:

Change it if you have w64devkit installed in a different location.

---

## Building and Running

In the VSCode terminal (w64devkit):

| Command      | Action                   |
|--------------|--------------------------|
| `make`       | compile                  |
| `make run`   | compile and run          |
| `make clean` | remove compiled files    |

Or use VSCode shortcuts:
- `Ctrl+Shift+B` – build
- `F5` – debug

## Controls

| Key           | Action   |
|---------------|----------|
| ← →           | move     |
| Space         | jump     |

## Project Structure
```text
├───.vscode
├───bin
├───include
├───lib
├───resources
└───src