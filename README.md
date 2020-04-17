# NIMOLIB - NIMO micro controller library

This library provides low level device drivers for supported micro controllers (currently ATSAMD21) via a common API. Whilst the library can be used as a standalone repository, it is intended to be included and configured via the umake tool (<https://github.com/nimo-labs/umake>).

## Library format

To be compatible with umake, libraries must follow a standard format.
The top level of a library consists of a GIT repository, umake needs to be be able to access this repository via a standard git clone command. umake doesn't currently provide any provision for passing parameters to the git command other than the libpath token provided in the projects umake file.
Currently the libName specified in the projects umakefile must match the repository name (e.g. if the git repository is called nimolib.git then libName must also be nimolib), bare this in mind when naming the repository. This restriction may be relaxed in the future.

## Books

Each library contains a number of books, which provide individual blocks of functionality. This may be a device driver such as a UART or RTC driver or alternatively a software function such as a FIFO library.
Each book is contained in it's own subdirectory within the repository and consists of a JSON file to describe the book and a number of source code files.

A json file inside the book directory is mandatory and must be named to reflect the book name. I.e. if the book (and therefore directory) name is gpio then the json file must be named gpio.json

umake expects to find the mandatory string field "book" within the json file whose value must match the book name. This provides an additional sanity check that umake is actually in the correct directory.

Optionally the book can also supply a list of source files (currently C and C++ are supported) that provide the books functionality. If source files are provided then umake expects to find an array of objects called "files" which contains fileName, language pairs for each source file.

If a particular book needs to define cflags and or ldflags then these can be added as arrays (named cflags and ldflags respectively) in the books json file.

By convention, a book should provide an overall .c (or .cpp) and .h file which specify the API of the book, with micro controller specific functionality included by way of ifdefs (see the gpio book for an example). If a book is entirely platform (micro controller and compiler) agnostic then it may be acceptable to implement the functionality within the same files as the API.

The directory name uC is reserved in a umake compatible library and contains json files that describe a particular micro controller and compiler configuration and micro controller specific data such as linker scripts, register descriptions etc.
