CFLAGS = -g

compilador: compilador.c sintatico.tab.c lexico.yy.c ast.c util.c errormsg.c symbol.c table.c env.c tradMepa.c label.c types.c semant.c geraCodigoMepa.c

sintatico.tab.c sintatico.tab.h: sintatico.y
	bison -Werror -tvd -r all sintatico.y

lexico.yy.c: lexico.l sintatico.tab.h
	flex -o lexico.yy.c lexico.l

clean:
	@$(RM) compilador sintatico.tab.c sintatico.tab.h sintatico.output lexico.yy.c