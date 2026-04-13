# Expansion of East Asian Skycultures and Explanation of Coordinate Visualization Function

[Bilingual 双语](README.md) | English | [中文](README_zh.md)

This is a customized version of [Stellurium](https://github.com/Stellarium/stellarium), mainly modified in [Chinese skyculture enhancement](https://github.com/Guanjin0562/stellarium/tree/chinese-skyculture-enhancement) Branch.

## Main Functions
- Expansion of skycultures: More East Asian skycultures, including China, Korea, and Japan. Chinese skycultures include the skycultures of ethnic minorities (Manchu, Yi).
- Coordinate visualization: By loading a JSON coordinate data file, the coordinates can be visualized in Stellurium, and the icons, colors, and sizes of the labels can be customized in the JSON file. Mainly used for studying ancient star observation data from both the East and the West, supporting all coordinate systems with Stellurium coordinate search function, as well as the traditional equatorial coordinate system in ancient China.
- Star catalog data: The folder [starcatalog](https://github.com/Guanjin0562/stellarium/tree/chinese-skyculture-enhancement/starcatalog) contains JSON data for both Eastern and Western star catalogs, which can be directly visualized using coordinate visualization function.

## Effect Showcase

<img width="2550" height="1322" alt="Image" src="https://github.com/user-attachments/assets/ed75c0f3-504d-4720-899e-324f9d0be419" />

## Quick Start
```
$ git clone -b chinese-skyculture-enhancement https://github.com/Guanjin0562/stellarium.git
```

## Branches
- `master`: A pure branch synchronized with the official
- `chinese-skyculture-enhancement`: **Main custom branch** (recommended)
- Other branches...

## Chinese Community and Discussion
- **Zhihu Column** 📖 [观星邀月](https://www.zhihu.com/column/c_1644824280952295424) - Contains articles related to ancient Chinese astronomy.

## Maintainer
[Kwantsin] - [GitHub](https://github.com/Guanjin0562) | [Zhihu](https://www.zhihu.com/people/zhan-shi-ying-lu-78)

---
*This branch is developed based on Stellarium/Stellarium and follows the GPL-2.0 protocol*

**(Below this line is the original README content)**

# Stellarium
[![GitHub release](https://img.shields.io/github/release/Stellarium/stellarium.svg)](https://github.com/Stellarium/stellarium/releases/latest)
[![GitHub Release Date](https://img.shields.io/github/release-date/Stellarium/stellarium.svg)](https://github.com/Stellarium/stellarium/releases/latest)
[![Github All Releases](https://img.shields.io/github/downloads/Stellarium/stellarium/total.svg)](https://github.com/Stellarium/stellarium/releases)
[![Backers and sponsors](https://img.shields.io/opencollective/all/stellarium.svg?style=flat)](https://opencollective.com/stellarium)
[![CI](https://github.com/Stellarium/stellarium/actions/workflows/ci.yml/badge.svg)](https://github.com/Stellarium/stellarium/actions/workflows/ci.yml)
[![Build status](https://ci.appveyor.com/api/projects/status/sw8j9l8q95ejkalo?svg=true)](https://ci.appveyor.com/project/alex-w/stellarium)
[![Coverage Status](https://coveralls.io/repos/github/Stellarium/stellarium/badge.svg)](https://coveralls.io/github/Stellarium/stellarium)
[![CodeFactor](https://www.codefactor.io/repository/github/stellarium/stellarium/badge)](https://www.codefactor.io/repository/github/stellarium/stellarium)

[![DOI:10.1558/jsa.17822](http://img.shields.io/badge/DOI-10.1558/jsa.17822-blue.svg)](https://doi.org/10.1558/jsa.17822)
[![DOI:10.5281/zenodo.8377210](http://img.shields.io/badge/DOI-10.5281/zenodo.8377210-blue.svg)](https://doi.org/10.5281/zenodo.8377210)
[![ASCL:2603.001](https://img.shields.io/badge/ascl-2603.001-blue.svg?colorB=262255)](https://ascl.net/2603.001)

Stellarium is a free open source planetarium for your computer. It shows a realistic sky
in 3D, just like what you see with the naked eye, binoculars or a telescope.

If you are new to Stellarium, go to [www.stellarium.org](https://www.stellarium.org) for loads of additional information.

## Installation Instructions & Quick Start

Please refer to the [User Guide, Getting Started section](https://github.com/Stellarium/stellarium/releases/download/v26.1/stellarium_user_guide-26.1-1.pdf).

## Get & build the code

See instructions to [building Stellarium from source code](BUILDING.md).

## Full References and Credits

See the [full credit file](CREDITS.md).

## Contributing to Stellarium

See the [contributing guideline](CONTRIBUTING.md).

## Contributors

This project exists thanks to all the people who contribute! List of contributors [on Github](https://github.com/Stellarium/stellarium/graphs/contributors) (code contributors) and [on Open Collective page](https://opencollective.com/stellarium#contributors) (financial contributors).

## Our backers & sponsors

Thank you to all [our backers and sponsors](BACKERS.md)!  Become a [backer](https://opencollective.com/stellarium#backer) or [sponsor](https://opencollective.com/stellarium#sponsor).

## Code Signing
Windows packages of this program uses free code signing provided by [SignPath.io](https://signpath.io?utm_source=foundation&utm_medium=github&utm_campaign=stellarium), and a free code signing certificate by the [SignPath Foundation](https://signpath.org?utm_source=foundation&utm_medium=github&utm_campaign=stellarium)

## SAST Tools
[PVS-Studio](https://pvs-studio.com/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code.
