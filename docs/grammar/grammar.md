SStart -> Start < EOF > 

Start -> Tokens Skip Rules

Tokens -> < TOKENS > < { > Token NextToken < } >

NextToken -> < , > Token NextToken

NextToken -> ε

Token -> < ID > < : > < QUOTE_EXPRESSION >

Skip -> < SKIP > < { > < QUOTE_EXPRESSION > SkipExpression < } >

Skip -> ε

SkipExpression -> < , > < QUOTE_EXPRESSION > SkipExpression

SkipExpression -> ε

Rules -> < ID > < -> > Rule < ; > NextRule

NextRule -> < ID > < -> > Rule < ; > NextRule

NextRule -> ε

Rule -> RuleBlock Annotation NextRuleBlock

NextRuleBlock -> RuleBlock Annotation NextRuleBlock

NextRuleBlock -> ε

RuleBlock -> < < > < ID > < > >

RuleBlock -> < ID >

RuleBlock -> < ( > Rule < ) > Closure

Closure -> < * >

Closure -> < + >

Annotation -> < { > < QUOTE_EXPRESSION > < : > AnnotationOption < } >

Annotation -> ε

AnnotationOption -> < QUOTE_EXPRESSION >

AnnotationOption -> < LAST >