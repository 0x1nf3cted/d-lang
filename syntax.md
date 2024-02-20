## match

set a = match x with
    |0 -> "Zero"
    |(_ < 0)=> "Negative"
    |none  -> "Positive"


## Pointers

set x: *u8

set y: u8 = 10

x= &y

# Variables

i8, i16, i32, i64, u8, u16, u32, u64

## struct

struct {
    name: str,
    age: u8,
    eat: (food: str) -> none
}

## Variables

set x: u8 = 42

## Constants

const PI = 3.14159

## Array

set arr: [2]<f8> = [1.2, 75.4]

## Function declaration

@main fn add = (x, y): u8 {
return x + y
}

## Function call

set result = add(5, 7)

## Importing modules

get math
get io

## Using imported functions

set result = io::read()
set sum = math::sum([1, 2, 3, 4, 5])

## Imperative-style loop

for (u8 i = 1, i < 10, i++) {
    io::print(i)
}

## While loop
while (true) {
    #
}

## Control flow

if(){

}
else if(){

}
else{

}


## Algebraic Data Types (Sum Types and Product Types)

type Shape =
| Circle of Float
| Rectangle of { width: Float, height: Float }




@rec: function is recursive
@inline: function won't be called but it's code will be replaced when it occure
@macro: define a macro



#grammar

variable         := initialized_var | uninitialized_var | identifier '=' value ';'
initialized_var := "set" identifier ':' variable_type '=' value ';'
uninitialized_var := "set" identifier ':' variable_type ';'

pointer_assign   := identifier '=' '&' variable ';'

pointer          := "set" identifier ':' '*' variable_type ';'

deref_pointer    := variable ':' '*' variable_type ';'

variable_type    := "char" | "u8" | "u16" | "u32" | "u64" | "f32" | "f64" | "str"

identifier       := [a-zA-Z_]

value            := variable | pointer | word | number

number           := [0-9] number | [0-9]
word             := identifier word_part | identifier
word_part        := [a-zA-Z_0-9] word_part | [a-zA-Z_0-9]
