SStart -> Start < EOF > 

Start -> Tokens Skip Rules

Tokens -> < TOKENS > < { > Token NextToken < } >

NextToken -> < , > Token NextToken

NextToken -> ε

Token -> < ID > < : > < ID >

Skip -> < SKIP > < { > < " > < REGEX_EXPR > < " > SkipExpression < } >

Skip -> ε

SkipExpression -> < , > < " > < REGEX_EXPR > < " > SkipExpression

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

Annotation -> < { > < " > < MATCH_ALL > < " > < : > AnnotationOption < } >

Annotation -> ε

AnnotationOption -> < " > < MATCH_ALL > < " >

AnnotationOption -> < LAST >