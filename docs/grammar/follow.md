FOLLOW(SStart) = { < EOF > }

FOLLOW(Start) = { < EOF > }

FOLLOW(Tokens) = { < SKIP >, < ID > }

FOLLOW(NextToken) = { < } > }

FOLLOW(Token) = { < } >, < , >}

FOLLOW(Skip) = { < ID > }

FOLLOW(SkipExpression) = { < } > }

FOLLOW(Rules) = { < EOF > }

FOLLOW(NextRule) = { < EOF > }

FOLLOW(Expansion) = { < ) >, < ; > }

FOLLOW(Closure) = { < ( >, < < >, < ID >, < { >, < ) >, < ; > }

FOLLOW(ExpansionBlock) = { < ( >, < < >, < ID >, < { >, < ) >, < ; > }

FOLLOW(NextExpansionBlock) = { < { >, < ) >, < ; > }

FOLLOW(Annotation) = { < ( >, < < >, < ID >, < ) >, < ; > }

FOLLOW(AnnotationOption) = { < } > }