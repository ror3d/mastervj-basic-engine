# mastervj-basic-engine

## How to Git

Everyone will have a personal branch at the upstream repo. Their local repo's master
branch will point to that personal branch, and when changes have to be made, they will
be pull-requested from that branch to the development branch.

Git Cheat Sheet: https://training.github.com/kit/downloads/github-git-cheat-sheet.pdf

Git Workflow Commands: http://www.ndpsoftware.com/git-cheatsheet.html


## Visual Studio Tips and Configuration

In `Tools > Options...`

- Set `Text Editor > C/C++ > Advanced > IntelliSense > Use Forward Slash in #include Auto Complete`
to True

- Set `Environment > Documents > Save documents as Unicode when data cannot be saved in codepage`


## Debugging

Set Working Directory to `$(SolutionDir)Data\` for the exe to take the correct Data directory.
