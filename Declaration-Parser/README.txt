grammar1 (not legal):

- Declaration -> type name
- type -> listMod base
- listMod -> mod listMod | E
- mod -> const | volatile | unsigned | signed |long | short
- base -> float | double | int | char

grammar2 (legal combinations) practical:
Declaration ::= Type Name ";"

Type ::= Modifiers SignOpt LengthOpt BaseType

/* Modificatori multipli */
Modifiers ::= { "const" | "volatile" }

/* Sign opzionale */
SignOpt ::= "signed" | "unsigned" | ε

/* Lunghezza opzionale */
LengthOpt ::= "short"
            | "long" [ "long" ]
            | ε

/* Tipo base dipende dal contesto */
BaseType ::= ( LengthOpt = "short" )   → "int"
           | ( LengthOpt = "long" )    → "int" | "double"
           | ( SignOpt ≠ ε )           → "int" | "char"
           | ( LengthOpt = ε ∧ SignOpt = ε ) 
                → "int" | "char" | "float" | "double"

Name ::= IDENT

grammar2 (legal combinations) theoretical:
- Declaration -> type name;
- type -> mod (intType | charType | floatType)

- mod -> const mod | volatile mod | E

- intType -> sign len int

- charType -> sign char

- len -> short | long | long long | E
- sign -> signed | unsigned | E

- floatType -> float | double | long double