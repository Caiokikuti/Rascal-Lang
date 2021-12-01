# # compilador: compilador.c sintatico.tab.c lexico.yy.c ast.c util.c

# sintatico.tab.c sintatico.tab.h: sintatico.y
# 	bison -Werror -tvd -r all sintatico.y

# lextest: driver.o lexico.yy.o errormsg.o util.o
# 	cc -g -o lextest driver.o lexico.yy.o errormsg.o util.o

# driver.o: driver.c tokens.h errormsg.h util.h
# 	cc -g -c driver.c

# errormsg.o: errormsg.c errormsg.h util.h
# 	cc -g -c errormsg.c

# lexico.yy.o: lexico.yy.c tokens.h errormsg.h util.h
# 	cc -g -c lexico.yy.c

# lexico.yy.c: lexico.l
# 	flex -o lexico.yy.c lexico.l

# util.o: util.c util.h
# 	cc -g -c util.c

# clean:
# 	@$(RM) compilador sintatico.tab.c sintatico.tab.h sintatico.output lexico.yy.c

CFLAGS = -g

compilador: compilador.c sintatico.tab.c lexico.yy.c ast.c util.c errormsg.c symbol.c table.c env.c translate.c types.c semant.c

sintatico.tab.c sintatico.tab.h: sintatico.y
	bison -Werror -tvd -r all sintatico.y

lexico.yy.c: lexico.l sintatico.tab.h
	flex -o lexico.yy.c lexico.l

clean:
	@$(RM) compilador sintatico.tab.c sintatico.tab.h sintatico.output lexico.yy.c