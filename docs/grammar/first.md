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

FIRST(Rule) = { < ( >, < < >, < ID > }

FIRST(Closure) = { < * >, < + > }

FIRST(RuleBlock) = { < < >, < ID >, < ( > }

FIRST(NextRuleBlock) = { < < >, < ID >, < ( > }

FIRST(Annotation) = { < { > }

FIRST(AnnotationOption) = { < LAST >, < " >}