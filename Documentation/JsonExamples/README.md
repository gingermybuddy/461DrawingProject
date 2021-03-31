# JSON Examples
This directory contains examples and templates of the JSON objects that are sent
between the client and the server.

An example (designated by \_example) contains a fully-written example of what
the specified object should look like.

A template (designated by \_template) contains angle-bracket notation to denote
which fields are data fields that should be changed or modified per date.

## Shape Notes
### Common
All shapes can contain color; if it can contian multiple colors, like an outline
AND a fill, then they are designated as seperate fields in `data`. All colors 
should be in hexadecimal format (`#rrggbb`, where each digit is 0-F).

### Latex
The Latex shape is similar to the text shape, with the difference of the `shape`
field being called latex and the `data.text` field should begin and end with a $.

## Extra Notes
The example in 'latex_example' is from 
[baeldung.com/cs/latex-drawing-graphs](https://www.baeldung.com/cs/latex-drawing-graphs),
because I wanted an example of a graph made in latex but did not know either
how complex of one to make or how to make one.
