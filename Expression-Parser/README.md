Grammar:

- statements -> expression; EOI | expression; statement

- expression -> term expression'
- expression' -> + term expression' | E

- term -> factor term'
- term' -> * factor term' | E

- factor -> NUM_OR_ID | (expression)