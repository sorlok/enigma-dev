grammar Egm;

@header {
 package hetu.seth.gen;
 
 import java.util.HashMap;
 import java.util.Deque;
 import java.util.ArrayDeque;
}

@rulecatch {
   catch (RecognitionException e) {
    throw e;
   }
}

@members {
  Deque<String> obj_scope = new ArrayDeque<>();    //Which object (e.g., "self") are we dealing with?
}

//Each snippet of code (object, timeline, etc.), just dives right in to statements.
//For the purpose of lexical scope, we consider statements to be a block-body.
rootRule
    :   {obj_scope.push("self");} blockBody EOF
    ;

//These come up later.
block
    :   ('with' '(' Identifier ')')? {obj_scope.push($Identifier!=null?$Identifier.text:obj_scope.peek());} '{' blockBody '}' {obj_scope.pop();}
    ;

blockBody
    locals [
 	  HashMap<String, String> writeSymbols = new HashMap<>(),  //Symbols this block sets  {name -> type (self/other/unknown)}
 	  HashMap<String, String> readSymbols = new HashMap<>(),   //Symbols this block reads {name -> type (self/other/unknown)}
 	]
    : statement*
    ;

//Statements vary, and can include blocked statements.
statement
    :   block
    |   expression (';'?)
    ;

expression
    :  primary                                    #Prim
    |  expression ('+'|'-'|'*'|'/') expression    #BinOp
    |  <assoc=right> (access=Identifier '.')? variable=Identifier {$blockBody::writeSymbols.put($variable.text, $access!=null?$access.text:obj_scope.peek());} ('=') expression  #Assign   //Keep this last so it can catch all.
    |  Identifier '(' paramList? ')'     #Func
    ;

paramList
    :  expression (',' expression)*   //TODO: We don't want assignment, but any other expression should work.
    ;

primary
    : '(' expression ')'
    | literal
    | (access=Identifier '.')? variable=Identifier {$blockBody::readSymbols.put($variable.text, $access!=null?$access.text:"unknown");}
    ;

literal
    :   IntegerLiteral
    |   FloatingPointLiteral
    |   StringLiteral
    ;

IntegerLiteral
    :   '-'?[0-9]+
    ;

FloatingPointLiteral
    :  '-'?[0-9.]+
    ;

StringLiteral
    :  '"' (~["])+ '"'
    ;
    
Identifier
    :   [a-zA-Z][a-zA-Z0-9_]* 
    ;


//Whitespace, comments.
WS  :  [ \t\r\n\u000C]+ -> skip
    ;

COMMENT
    :   '/*' .*? '*/' -> skip
    ;

LINE_COMMENT
    :   '//' ~[\r\n]* -> skip
    ;
