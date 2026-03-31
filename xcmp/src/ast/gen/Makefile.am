include local.am

CLEANFILES = $(GENERATED)

all-local: $(GENERATED)

$(GENERATED): nodes.toml
	$(AM_V_GEN)$(XCMGEN) -c $< -o $(@D) -f $(@F)
