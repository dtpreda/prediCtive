# prediCtive

`prediCtive` is a C++ parser generator for LL(1) grammars. It aims to provide users with a simple syntax to express their own grammars and generate parsers for those same grammars.

## Details

### Grammar Definition

New grammars can be defined using the `prediCtive`'s syntax, defined [here](/docs/grammar/grammar.md). This syntax forces you to define your language as closely as possible to an LL(1) language: you must write every rule from scratch, which, by being more cumbersome, allows you to more easily spot ambiguities.

An example grammar, used for tests, is provided [here](/test/assets/simpleGrammar.cg) along with [a file written in that same grammmar](/test/assets/testFile.sg)

### Regex Expressions

All regex expressions **must** be written in the ECMASscript syntax. For documentation on the syntax, see [here](https://cplusplus.com/reference/regex/ECMAScript/).

### Annotations

The `prediCtive` grammar allows you to annotate every node generated, which provides a richer parsing tree. The keyword `last_token` may be used as an annotation value in order to fetch the last token parsed and inject it into the node.

## Disclaimer

`prediCtive` is a fun side-project, not aimed for professional use. It has not big emphasis on performance, nor it is thoroughly tested. Use it at your own risk and will.

If you find anything you think could be made better, feel free to fork the repository and send a pull request. Thank you for dropping by!
