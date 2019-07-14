# README #

The Mezzanine is a set of multi platform C++ libraries designed to help create high performance
videos games. The Mezzanine is focused on games requiring 3d physics, 3d graphics, 3d sounds and
common facilities that such games need. This is just one component of many.

## About this Library ##

This library provides some simple local filesystem access and query utilities. At the time of this
writing, not all platforms support std::filesystem cleanly. Even after it is supported on all 
compilers we care about this library offers some extra utilities std::filesystem doesn't cover.

## Build Status ##

The current Travis CI (Apple - GCC/Clang) build status is:
[![Build Status](https://travis-ci.org/BlackToppStudios/Mezz_PackageName.svg?branch=master)](https://travis-ci.org/BlackToppStudios/Mezz_PackageName)

The current Appveyor (Windows - MinGW/Msvc) build status is:
[![Build Status](https://ci.appveyor.com/api/projects/status/github/BlackToppStudios/Mezz_PackageName?branch=master&svg=true)](https://ci.appveyor.com/project/Sqeaky/mezz-packagename)

The current code coverage for tests is:
[![codecov](https://codecov.io/gh/BlackToppStudios/Mezz_PackageName/branch/master/graph/badge.svg)](https://codecov.io/gh/BlackToppStudios/Mezz_PackageName)

The current Jenkins, which covers Linux (Emscripten, Rasberry Pi, Ubuntu and Fedora), old Mac OS X (High Sierra) and old windows (7 64 bit msvc and mingw), build status is available on the [BTS Jenkins Instance](http://blacktopp.ddns.net:8080/blue/organizations/jenkins/Mezz_PackageName/activity). The current status is: [![Build Status](http://blacktopp.ddns.net:8080/job/Mezz_PackageName/job/master/badge/icon)](http://blacktopp.ddns.net:8080/blue/organizations/jenkins/Mezz_PackageName/activity)
