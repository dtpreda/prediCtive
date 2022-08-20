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

Rules -> < ID > < -> > Expansion < ; > NextRule

NextRule -> < ID > < -> > Expansion < ; > NextRule

NextRule -> ε

Expansion -> ExpansionBlock Annotation NextExpansionBlock

NextExpansionBlock -> ExpansionBlock Annotation NextExpansionBlock

NextExpansionBlock -> ε

ExpansionBlock -> < < > < ID > < > >

ExpansionBlock -> < ID >

ExpansionBlock -> < ( > Expansion < ) > Closure

Closure -> < * >

Closure -> < + >

Annotation -> < { > < QUOTE_EXPRESSION > < : > AnnotationOption < } >

Annotation -> ε

AnnotationOption -> < QUOTE_EXPRESSION >

AnnotationOption -> < LAST >