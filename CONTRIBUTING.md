# Contributing to Adaptive AUTOSAR Linux Simulator

This document guides you about how to contribute to this project. For learning about the project code of conduct please refer to [this document](https://github.com/langroodi/Adaptive-AUTOSAR/blob/master/CODE_OF_CONDUCT.md).

## Table of contents

[Contributor mindset](#contributor-mindset)

[Reason to contribute and how to contribute](#reason-to-contribute-and-how-to-contribute)

- [Find a bug](#Find-a-bug)

- [Add a new feature/unit test or extend an existing feature/unit test](#add-a-new-featureunit-test-or-extend-an-existing-featureunit-test)

- [Just want to contribute](#just-want-to-contribute)

[Code conventions (C++)](#code-conventions-c)

- [Layout conventions](#layout-conventions)

- [Naming conventions](#naming-conventions)

- [Language guidelines](#language-guidelines)

[Maintenance](#maintenance)

- [Documentation](#documentation)

- [Unit test](#unit-test)

- [External dependencies](#external-dependencies)

- [Commit messages (optional)](#commit-messages-optional-inspired-by-atom)

## Contributor mindset
Firstly, thanks for showing interest in contributing to this project. Following are 7 tips to start contributing to this project:

1. **Keep smiling with it:**

> If you still don't know what you hate the most in this world, it means you never write a C++ code.
Developing this project in C++ can be a pain, but it is also fun (it depends on how you look at it).
When you get frustrated while contributing, give yourself a break and come back to your code with a fresh mind.
And always keep smiling while coding (Trust me! It will help you to generate good-quality codes).

2. **Be critical about it:**

> The first solution that comes to your mind is not necessarily the best solution.
The aim of development is not to finish a piece of code, but it is learning how to code today better than yesterday.
Definitely, it is not usually possible to find the best solution at the same day you start your development, but it is always worth to think for several minutes to see whether there is any room for improvement or not.
Besides that, it is highly recommended to double check your changes before committing them.

3. **Stop assuming it:**

> If you are not sure how a code block performs, do not assume how it will perform. Test it, search about it, or ask a question about it (e.g., in the [project Discussions board](https://github.com/langroodi/Adaptive-AUTOSAR/discussions)).
Before copying-and-pasting a code snippet to your own code, try to do understand the logic behind that snippet.
Never hire a mechanism (e.g., a protocol, a package, etc.) in your project without knowing the general idea about how it works.

4. **Document it:**

> You find a nice idea as a start point to develop your code, then it is better write a short description about it before it becomes too late.
You may think that idea may change during the development phase, then it should be alright to modify the written document accordingly.
You can put these documents in [the project Wiki](https://github.com/langroodi/Adaptive-AUTOSAR/wiki). Write them in a way that a stranger may read them rather than yourself.

5. **Refactor it:**

> If you encouter several issues from a certian project class, or you find it difficult to debug issues from that class, it is the time to refactor the class rather than patching it (for instance with ugly if-statements).
If you don't have a time to refactor it now, then create a new [maintenance issue](https://github.com/langroodi/Adaptive-AUTOSAR/issues/new?assignees=&labels=enhancement&template=feature_request.md&title=) out of it, so you won't forget to pick it up later.

6. **Make it portable:**

> Respect [the project dependecies](https://github.com/langroodi/Adaptive-AUTOSAR#dependecies) and avoid using instructions that may only work on your own local system both software-wise and hardware-wise.
For instance, if you employ an AVX512 intrinsic instruction, you make the code dependent on a specific hardware requirement.

7. **Keep it readable:**

> Try to avoid being smart and write the codes that no one else than you can understand (something like *fast inverse square root*).
If from performance perspective you have no choice to do that, put a comment on the code and clarify your smart idea for others.

## Reason to contribute and how to contribute

Regardless of any reason for contributing to this project, the simple rule for contribution is to fork the repository, apply your changes, and create a [pull request](https://github.com/langroodi/Adaptive-AUTOSAR/pulls).
You may like to contribute to this project for several reasons as follows:

### Find a bug

If you find a bug in this project, there is no security policy that you need to follow. So, just report the bug via [the Issues](https://github.com/langroodi/Adaptive-AUTOSAR/issues/new?assignees=langroodi&labels=bug&template=bug_report.md&title=).
If you also want to fix the bug, then after creating the issue, fix it on your own fork, and create the pull request while [mentioning the issue](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) in it.

### Add a new feature/unit test or extend an existing feature/unit test

If you find interesting to have a new feature/unit test or extend an existing one in this project, first create a new [discussion](https://github.com/langroodi/Adaptive-AUTOSAR/discussions/new) to see other community members find that interesting as well or not.
If the majority of the community agrees with that, you can create a new issue out of it, and the rest of the way should be clear to you.

### Just want to contribute

If you are interested in the project while you don't know how to contribute, then the first place you can check is [the Issues board](https://github.com/langroodi/Adaptive-AUTOSAR/issues).
If you didn't find anything suitable for you, then you can start a new [discussion](https://github.com/langroodi/Adaptive-AUTOSAR/discussions/new). So, the community will try to find a suitable task for you.
Please do not contribute to the project by fixing the code formatting, typos, and/or adding comments to the code. PRs related to these types of contribution will be rejected.

## Code conventions (C++)

Please read the conventions in the following domains to have a consistent coding style among the all project contributors:

### Layout conventions

- Try to keep the column limit to 80 characters.

- Put the braces at a separate new line ([Allman or BSD style](https://en.wikipedia.org/wiki/Indentation_style#Allman_style)):
``` c++
if (a > b)
{

}
```

- Try to avoid having multiple statements in each line.

### Naming conventions

- Class, struct, protected/public member/method, and template parameter names should be `PascalCase`:

``` c++
template <typename TParam>
class MyClass
{
protected:
  int Member;
public:
  void Method();
};

struct MyStruct
{
  int Member;
};
```

- Namespaces should be `lowercase`:
``` c++
namespace myapplication
{
  namespace myclasses
  {
    // ...
  }
}
```

- Class or struct file names should be generated by splitting their `PascalCase` names and re-joining the `lowercase` sections by `_`.
For example, the header file name of `MyClass` will be `my_class.h`.

- Private member/method, function argument, and local variable names should be `camelCase`:

``` c++
template <typename TParam>
class MyClass
{
private:
  void method(int argument)
  {
  }
};
```

- No need to indicate the variable type by its name, such as adding `_ptr` suffix after a pointer name.

- Static constant, private member, and local variable names should have `c`, `m`, and `_` prefixes respectively:

``` c++
class MyClass
{
private:
  static const int cNumber;
  int mNumber;
  
  void method(int argument)
  {
    int _temp = argument;
  }
};
```

- Use a plural name for arrays and other container types (e.g., `std::vector`):
``` c++
std::array<int> numbers
std::vector<char> characters;
```

### Language guidelines

- Use the `auto` keyword only in iterator-based loops or when the type is explicitly mentioned:

``` c++
auto foo = std::make_shared<int>(1);
for(auto number : numbers)
{
  // Better to avoid using 'auto' keyword in the following line:
  int _result = GetResult();
}
```

- Use C++11/14 new features like `using` keyword instead of its older C/C++ alternative `typedef`.

- Absolutely avoid unsafe C functions (e.g., `memcpy`) and use their safe alternative (e.g., `memcpy_s`) based on [this list](https://github.com/intel/safestringlib/wiki/SDL-List-of-Banned-Functions).

## Maintenance

Please keep in mind the following domains while contributing to ensure the project stability and maintainability in long term:

### Documentation

All the protected and public entities in interfaces (header files) should be documented based on [Doxygen](https://www.doxygen.nl/index.html).
Then [Doxygenize action](https://github.com/langroodi/doxygenize) takes care of the rest and publishes the code documentation to [GitHub pages](https://langroodi.github.io/Adaptive-AUTOSAR/).
If the code is not documented properly, it will be visible from [the action log](https://github.com/langroodi/Adaptive-AUTOSAR/actions).

### Unit test

It is strongly recommended to create a unit test for each project entity. The automated workflow executes the unit tests after each commit.

### External dependencies

Currently, the project only depends on the standard C++14 and in-house libraries. If you want to add a 3rd party dependency to the project, first create a [new discussion](https://github.com/langroodi/Adaptive-AUTOSAR/discussions/new) about it.
The discussion won't be about forcing you to reinvent the wheel, but to avoid probable security and portability issues (e.g., supply chain attack).

### Commit messages (optional: inspired by [Atom](https://github.com/atom/atom/blob/master/CONTRIBUTING.md#git-commit-messages))

Start your commit message with the appropriate emoji code based on the following categories:

| Commit category | Emoji code | Emoji icon |
| --------------- | ---------- | ---------- |
| Feature adding/extending | `:beer:` | :beer: |
| Unit test adding/extending | `:microscope:` | :microscope: |
| Bug fixing | `:syringe:` | :syringe: |
| Code documenting | `:scroll:` | :scroll: |
| Code refactoring | `:shower:` | :shower: |
| Code cleaning/removing | `:toilet:` | :toilet: |
| Performance enhancing | `:rocket:` | :rocket: |
| Workflow improving/updating | `:traffic_light:` | :traffic_light: |
| Compiler optimizing/warning fixing | `:construction:` | :construction: |



