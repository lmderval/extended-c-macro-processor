include local.am

AM_YFLAGS = -Wno-yacc -Wcounterexamples

CLEANFILES = $(GENERATED)

all-local: $(GENERATED)

$(LEXER_CC): lexer.ll
	$(AM_V_GEN)$(LEX) -t $< 1>$@
	$(AM_V_at)$(SED) -i \
		-e 's:lex.yy.c:$(@F):g' \
		-e 's:$<:gen/$<:g' \
		$@

$(PARSER_CC): parser.yy
	$(AM_V_GEN)$(YACC) $(AM_YFLAGS) -o /tmp/parse.cc -H --locations $<
	$(AM_V_at)mv /tmp/parse.cc $@
	$(AM_V_at)$(SED) -i \
		-e 's:/tmp/parse.cc:$(@F):g' \
		-e 's:parse.hh:$(notdir $(PARSER_HH)):g' \
		-e 's:$<:gen/$<:g' \
		$@

$(PARSER_HH): $(PARSER_CC)
	$(AM_V_GEN)mv /tmp/parse.hh $@
	$(AM_V_at)$(SED) -i \
		-e 's:/tmp/parse.hh:$(@F):g' \
		-e 's:\(parser.yy\):gen/\1:g' \
		$@

$(LOCATION_HH): location.hh.in parser.yy
	$(AM_V_GEN)$(YACC) $(AM_YFLAGS) -o /tmp/parse-loc.cc -H --locations parser.yy
	$(AM_V_at)mv /tmp/location.hh $@
	$(AM_V_at)$(SED) -i \
		-e 's:/tmp/location.hh:$(@F):g' \
		-e 's:\(parser.yy\):gen/\1:g' \
		$@
	$(AM_V_at)echo -e '\n#line 1 "gen/$<"' 1>>$@
	$(AM_V_at)$(CAT) $< 1>>$@
