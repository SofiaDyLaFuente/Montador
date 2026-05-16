SECTION DATA
dois: CONST  0x12
old: SPACE
new: SPACE
temp: SPACE         ;      Este é um comentário com bastante espaço        
SECTION TEXT

INPUT old
LOAD old
l1: 
DIV dois
STORE  new ;Esse é um comentário
MULT dois ;Este é outro comentário
STORE temp            
         
LOAD old
SUB temp
STORE TMP_DATAD
    OUTPUT temp
COPY new, old
LOAD old
    JMPP l1
STOP    
