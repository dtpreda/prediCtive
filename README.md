# prediCtive - A generic purpose C++ parser generator

`prediCtive` is a C++ parser generator for LL languages. It aims to provide users with a simple grammar to express their own grammars and generate parsers for those same grammars.

The program will return a tree comprised of nodes related to the rules inputed by the user. Those same nodes can be annotated, so the parsing tree contains more meaningful information for other programs to use.

## Main Features

- A simple grammar to build other grammars;
- Capacity to annotate nodes;
- Error information upon inputing not LL(1) languages.

## To-Do

- ~~Define grammar language~~;
- Parse ~~tokens~~ and ignores;
- ~~Parse basic rules~~;
- Parse options;
- ~~Add annotations~~;
- Parse complex rules (conditional);
- Add look ahead.
