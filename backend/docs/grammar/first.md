For any terminal X: 

FIRST(X) = { X }

For non-terminals:

FIRST(SStart) = First(Start) = { < TOKENS > } 

FIRST(Start) = { < TOKENS > }

FIRST(Tokens) = { < TOKENS > }

FIRST(NextToken) = { < , > }

FIRST(Token) = { < ID > }

FIRST(Skip) = { < SKIP > }

FIRST(SkipExpression) = { < , > }

FIRST(Rules) = { < ID > }

FIRST(NextRule) = { < ID > }

FIRST(Expansion) = { < ( >, < < >, < ID > }

FIRST(Closure) = { < * >, < + > }

FIRST(ExpansionBlock) = { < < >, < ID >, < ( > }

FIRST(NextExpansionBlock) = { < < >, < ID >, < ( > }

FIRST(Annotation) = { < { > }

FIRST(AnnotationOption) = { < LAST >, < QUOTE_EXPRESSION >}