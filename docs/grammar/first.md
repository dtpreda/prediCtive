For any terminal X: 

FIRST(X) = { X }

For non-terminals: 

FIRST(Start) = { < TOKENS > }

FIRST(Tokens) = { < TOKENS > }

FIRST(Token) = { < ID > }

FIRST(Skip) = { < SKIP > }

FIRST(Rules) = { < ID > }

FIRST(Rule) = { < ( >, < < >, < ID > }

FIRST(Closure) = { < * >, < + > }

FIRST(BasicRule) = FIRST(RuleElement) = { < < >, < ID >}

FIRST(RuleElement) = FIRST(RuleBlock) = { < < >, < ID >}

FIRST(RuleBlock) = { < < >, < ID > }

FIRST(Annotation) = { < { > }

FIRST(AnnotationOption) = { < LAST >, < " >}