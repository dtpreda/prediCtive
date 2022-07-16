SStart -> Start < EOF > 

Start -> Tokens Skip? Rules

Tokens -> < TOKENS > < { > Token ( < , > Token)* < } >

Token -> < ID > < : > < ID >

Skip -> < SKIP > < { > < REGEX > (, REGEX)* < } >

Rules -> (< ID > < -> > Rule < ; >)+

Rule -> < ( > Rule < ) > Closure

Rule -> BasicRule

Closure -> < * >

Closure -> < + >

BasicRule -> (RuleElement)+

RuleElement -> RuleBlock (Annotation)?

RuleBlock -> < < > < ID > < > >

RuleBlock -> < ID >

Annotation -> < { > < ID > < : > AnnotationOption < } >

AnnotationOption -> < " > < MATCH_ALL > < " >

AnnotationOption -> < LAST >