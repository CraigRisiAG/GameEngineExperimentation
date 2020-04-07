# How to contribute efficiently

This is a modified version of the Godot Engine not intended for actual use. If you do wish to contribute towards it though, please follow the guidelines set out below.
 

## Contributing pull requests

If you want to add new engine features, please make sure that:

- You understand the code and how it works and have checked that the requied code does not appear elsewhere.
- Sufficient tests are in place to ensure your code does not break any other part of the code
- You are prepared to offer support if your fix breaks anything critical
- Changes and code are well documented detailing the different steps of the code and why the change is necessary
- Even if it doesn't get merged, your PR is useful for future work by another
  developer.

Similar rules can be applied when contributing bug fixes - it's always best to
discuss the implementation in the bug report first if you are not 100% about
what would be the best fix.


### Be nice to the Git history

Try to make simple PRs that handle one specific topic. Just like for reporting
issues, it's better to open 3 different PRs that each address a different issue
than one big PR with three commits.

When updating your fork with upstream changes, please use ``git pull --rebase``
to avoid creating "merge commits". Those commits unnecessarily pollute the git
history when coming from PRs.

Also try to make commits that bring the engine from one stable state to another
stable state, i.e. if your first commit has a bug that you fixed in the second
commit, try to merge them together before making your pull request (see ``git
rebase -i`` and relevant help about rebasing or amending commits on the
Internet).

This [Git style guide](https://github.com/agis-/git-style-guide) has some
good practices to have in mind.

### Format your commit messages with readability in mind

The way you format your commit messages is quite important to ensure that the
commit history and changelog will be easy to read and understand. A Git commit
message is formatted as a short title (first line) and an extended description
(everything after the first line and an empty separation line).

The short title is the most important part, as it is what will appear in the
`shortlog` changelog (one line per commit, so no description shown) or in the
GitHub interface unless you click the "expand" button. As the name says, try to
keep that first line under 72 characters. It should describe what the commit
does globally, while details would go in the description. Typically, if you
can't keep the title short because you have too much stuff to mention, it means
you should probably split your changes in several commits :)


**Note:** When using the GitHub online editor or its drag-and-drop
feature, *please* edit the commit title to something meaningful. Commits named
"Update my_file.cpp" won't be accepted.

Thanks for your interest in contributing!

